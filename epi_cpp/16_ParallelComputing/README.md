# Parallel Computing

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 16. See epilight_cpp_new.pdf for the full text.*

## Overview

Parallel hardware is everywhere: consumer machines ship with multiple cores sharing one memory, and heavy workloads run on clusters of networked machines. Interview questions in this space test whether you can reason about interleavings, not whether you can memorize APIs.

**Why parallelize?** Five recurring motivations:
- *Performance* — more workers usually finish sooner.
- *Resource utilization* — one computation proceeds while another blocks on disk or network.
- *Fairness* — several users or jobs share a machine instead of queuing behind one long job.
- *Decomposition convenience* — some systems are simply easier to express as cooperating concurrent activities than as one monolithic controller.
- *Fault tolerance* — in a serving cluster, surviving machines absorb the load of a failed one.

**Where it shows up.** GUIs keep a dedicated UI thread responsive while workers do network I/O; language runtimes run garbage collection on its own thread alongside user code; web servers give each client request a logical thread; scientific codes shard giant matrix multiplies across a cluster; search engines parallelize crawling, indexing, and retrieval across machines.

**Two models.** In the *shared-memory* model every processor reads and writes a common address space — the natural model for multicore, and the model this chapter's problems assume. In the *distributed-memory* model processors exchange explicit messages — the realistic model for clusters.

**Why it is hard.** Concurrent components interact in subtle ways:
- *Data races* — two concurrent accesses to one memory location, at least one a write.
- *Starvation* — a thread waits forever for a resource it needs.
- *Deadlock* — thread A holds lock L1 and wants L2 while thread B holds L2 and wants L1; neither can proceed.
- *Livelock* — threads stay busy endlessly retrying an operation that keeps failing.

These bugs hide from tests: they surface only under particular schedules and loads, and often cannot be reproduced on demand. Performance disappointments are equally common — a serial bottleneck caps speedup no matter how many cores you add, and the cost of shipping intermediate results between workers can swamp the gains.

## Boot camp

A **semaphore** is a general synchronization primitive maintaining a pool of permits. `Acquire()` blocks until a permit is free, then claims it; `Release()` returns a permit and wakes waiters. Building one from a mutex and a condition variable demonstrates the fundamental pattern behind nearly all blocking synchronization. (In real code, use the standard library's `std::counting_semaphore` — but be ready to implement one when asked.)

```cpp
class Semaphore {
 public:
  explicit Semaphore(int max_permits)
      : max_permits_(max_permits), in_use_(0) {}

  void Acquire() {
    std::unique_lock<std::mutex> lock(mu_);
    cv_.wait(lock, [this] { return in_use_ < max_permits_; });
    ++in_use_;
  }

  void Release() {
    {
      std::lock_guard<std::mutex> lock(mu_);
      --in_use_;
    }
    cv_.notify_one();
  }

 private:
  const int max_permits_;
  int in_use_;
  std::mutex mu_;
  std::condition_variable cv_;
};
```

Three details carry over to every condition-variable design:
1. The waiting thread re-checks its predicate every time it wakes (the predicate-form `wait` loops internally) — mandatory because wakeups can be spurious and because another thread may snatch the permit first.
2. All shared state (`in_use_`) is touched only under the mutex.
3. Notification happens after the state change that could unblock a waiter.

## Top tips

- Begin with a design that locks coarsely and is *obviously* correct; only then carve out concurrency, keeping every critical section protected. Correct-then-fast beats fast-then-maybe-correct.
- Reason against a worst-case scheduler: it may run one thread over and over, preempt at the least convenient instruction, alternate two threads pathologically, or starve a thread outright. If the code is correct under that adversary, it is correct.
- Work at the highest available level of abstraction: use the platform's semaphores, thread pools, and deferred-execution facilities rather than rolling your own — while still knowing how they work inside, since implementing them is a classic interview ask.

## 16.1 Implement a Timer class

A calendar server must fire a job (send an email or SMS) exactly when each upcoming event occurs. Design the facility that manages such deferred tasks: a timer class whose constructor accepts a runnable object with a name, supporting (1) scheduling a named task to start at a given future time and (2) cancelling a task by name, where cancellation is ignored if the task already started.

*Hint: the design has two separable concerns — which data structures, and which locking discipline.*

**Strawman — one thread per scheduled task.** Spawn a thread per task that sleeps until its launch time. Simple, but thousands of pending tasks mean thousands of sleeping threads (heavy), and cancellation requires interrupting a sleeping thread — clumsy and error-prone. All the sleeping can be centralized.

**Data structures.** Two, kept consistent with each other:
1. A **min-heap** of (launch time → task) pairs, ordered by launch time, holding everything pending. A single *dispatch thread* services it: sleep until the earliest launch time; on waking, pop and run every task whose time has arrived; then sleep until the new minimum. Because entries can be added or removed while it sleeps, the dispatch thread must be woken early when the minimum changes so it can shorten (or extend) its nap — and it must tolerate waking to find nothing due.
2. A **hash table** from task name to that task's heap entry, so cancellation and lookup are O(log n) instead of a linear heap scan: cancel = look up the entry, remove it from the heap, and if it was the minimum, nudge the dispatch thread.

Insertion mirrors cancellation: push into the heap, record in the hash table, and wake the dispatcher if the new task became the minimum.

**Locking.** The heap and hash table are shared between client threads (schedule/cancel) and the dispatch thread. The simplest correct discipline is a single mutex guarding *every* read and write of both structures, plus a condition variable the dispatcher waits on with a deadline — client mutations signal it after changing the minimum. Tasks themselves run *outside* the lock so a slow task cannot block scheduling.

```cpp
class Timer {
 public:
  Timer() : dispatcher_([this] { DispatchLoop(); }) {}

  ~Timer() {
    {
      std::lock_guard<std::mutex> lock(mu_);
      shutdown_ = true;
    }
    wakeup_.notify_all();
    dispatcher_.join();
  }

  using Clock = std::chrono::steady_clock;

  void Schedule(const std::string& name, Clock::time_point when,
                std::function<void()> run) {
    std::lock_guard<std::mutex> lock(mu_);
    pending_[name] = {when, std::move(run)};
    if (when < next_deadline_) wakeup_.notify_all();  // new minimum: re-nap
  }

  void Cancel(const std::string& name) {
    std::lock_guard<std::mutex> lock(mu_);
    pending_.erase(name);  // no-op if already started (entry removed then)
  }

 private:
  struct Task {
    Clock::time_point when;
    std::function<void()> run;
  };

  void DispatchLoop() {
    std::unique_lock<std::mutex> lock(mu_);
    while (!shutdown_) {
      auto due = std::min_element(  // stand-in for the heap's top
          pending_.begin(), pending_.end(),
          [](auto& a, auto& b) { return a.second.when < b.second.when; });
      if (due == pending_.end()) {
        next_deadline_ = Clock::time_point::max();
        wakeup_.wait(lock);                       // nothing pending
        continue;
      }
      next_deadline_ = due->second.when;
      if (wakeup_.wait_until(lock, next_deadline_) == std::cv_status::timeout) {
        auto it = pending_.find(due->first);      // may have been cancelled
        if (it != pending_.end() && it->second.when <= Clock::now()) {
          auto run = std::move(it->second.run);
          pending_.erase(it);                     // now uncancellable
          lock.unlock();
          run();                                  // execute outside the lock
          lock.lock();
        }
      }
    }
  }

  std::mutex mu_;
  std::condition_variable wakeup_;
  std::unordered_map<std::string, Task> pending_;
  Clock::time_point next_deadline_ = Clock::time_point::max();
  bool shutdown_ = false;
  std::thread dispatcher_;  // last member: starts after state is initialized
};
```

(The sketch uses a `min_element` scan for brevity; the production version keeps the tasks in an actual `priority_queue`/indexed heap alongside the name map, making every operation logarithmic.)

**Worked scenario.** At 10:00, tasks are scheduled for 10:05 ("digest") and 10:30 ("reminder"); the dispatcher sleeps until 10:05. At 10:01 a client schedules "alert" for 10:02 — that beats the current minimum, so the scheduler signals the condition variable; the dispatcher wakes, sees the new top, and re-sleeps until 10:02. At 10:02 it runs "alert". At 10:04 a client cancels "digest"; at 10:05 the dispatcher wakes, finds "digest" gone, finds nothing due, and sleeps until 10:30 — exactly the "wake up and find nothing to do" case the design must tolerate.

**Complexity.** With a real heap: O(log n) per schedule and per cancel, O(n) space for n pending tasks, and no busy-waiting anywhere — the dispatcher consumes no CPU between deadlines.

**Variants:** none appear in the sampler for this problem.
