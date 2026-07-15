# Object-Oriented Design

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 19. See epilight_cpp_new.pdf for the full text. All exposition below is original commentary on the chapter's topics.*

## Overview

A **class** bundles data together with the methods that operate on that data. This matches how we naturally think about computation, and it delivers two concrete engineering benefits:

- **Encapsulation** — the internals of a class are hidden behind an interface, which shrinks the amount of context a programmer must hold in mind while writing or reading code.
- **Reuse** — inheritance and polymorphism let new code build on existing code instead of duplicating it.

The caution: object orientation is not automatically good design. Applied naively — deep inheritance towers, god objects, gratuitous indirection — OO constructs produce code that is *harder* to maintain, not easier.

A **design pattern** is a general, repeatable solution to a commonly occurring design problem. It is not a finished design you can paste into a codebase; it is a *description of an approach* that applies across many situations. In the OO context, patterns target both reuse and maintainability, and the mechanism by which they achieve this is almost always the same: **they let one part of a system vary independently of the others.** When you can swap an implementation, an algorithm, or an object-creation policy without touching client code, change becomes cheap.

The guiding wisdom (from the Gang of Four): expert designers do not solve every problem from first principles — they recognize a recurring situation and reach for a known, named solution. Knowing the names also gives teams a shared vocabulary: "make it a builder" communicates a whole design in three words.

For further study, freely available university course material on design patterns (lecture notes, homework, labs — e.g., Adnan Aziz's design patterns course) complements this chapter well.

## 19.1 Creational patterns

**Question.** Explain each of these creational patterns: builder, static factory, factory method, and abstract factory.

Creational patterns all answer the same underlying question — *who constructs objects, and how much do callers need to know about it?* — but at different granularities: builder tames one complicated construction; static factory hides one class's instantiation policy; factory method defers one creation decision to subclasses; abstract factory defers a whole *family* of creation decisions.

### Builder

**Motivation.** Some objects are genuinely complicated to construct: many fields, most of them optional, some interdependent. A constructor forces all of that through one positional parameter list — unreadable at the call site and combinatorially explosive if you try to provide overloads (the "telescoping constructor" problem).

**Idea.** Split construction into *phases* carried out on a mutable companion object (the builder). Each setter is named, so call sites read like documentation; the final `Build()` validates the accumulated state and emits the finished product. The product itself can then be immutable and is never observable in a half-initialized state.

```cpp
class HttpRequest {
 public:
  class Builder;
 private:
  HttpRequest() = default;
  std::string url_, method_ = "GET", body_;
  int timeout_ms_ = 30000;
};

class HttpRequest::Builder {
 public:
  Builder& Url(std::string u)      { req_.url_ = std::move(u); return *this; }
  Builder& Method(std::string m)   { req_.method_ = std::move(m); return *this; }
  Builder& Body(std::string b)     { req_.body_ = std::move(b); return *this; }
  Builder& TimeoutMs(int t)        { req_.timeout_ms_ = t; return *this; }
  HttpRequest Build() { /* validate req_ here */ return std::move(req_); }
 private:
  HttpRequest req_;
};

// Call site: every argument is named, every optional stays defaulted.
auto req = HttpRequest::Builder{}.Url("https://api.example.com")
                                 .Method("POST")
                                 .Body("{}")
                                 .Build();
```

**Benefits.** Decomposes construction into named steps; handles long and mostly-optional parameter lists gracefully; avoids inconsistent intermediate states; enables immutable products.
**Costs.** Boilerplate — a second class per product — and slightly more allocation/copy traffic than a raw constructor.

### Static factory

**Motivation.** A constructor must be named after its class, must return a brand-new instance of exactly that class, and reveals the concrete type at every call site. All three restrictions are often unwanted.

**Idea.** Use an ordinary (usually static) function to produce instances. This buys three freedoms:

1. **A meaningful name.** `Duration::FromMillis(500)` says far more than `Duration(500)` — especially when several conceptually different constructions would otherwise collide on the same parameter types.
2. **No obligation to allocate.** The function may return a cached, shared instance — a *flyweight* — instead of constructing a new object every call (interned strings, boxed small integers, singleton empty collections).
3. **Freedom to choose the concrete type.** It can return a subtype specialized for the input — for instance, a set-of-integers factory can return a bit-packed implementation that stores the whole set in one machine word when the universe is small enough, and a general implementation otherwise. Callers never know or care.

```cpp
class IntSet {
 public:
  static std::unique_ptr<IntSet> Make(int universe_size) {
    if (universe_size <= 64) return std::make_unique<BitwordSet>();  // 1 word
    return std::make_unique<HashIntSet>();                           // general
  }
  virtual ~IntSet() = default;
  virtual void Add(int x) = 0;
  virtual bool Contains(int x) const = 0;
};
```

**Costs.** Slightly less discoverable than constructors, and if the class forbids direct construction, subclassing from outside becomes awkward.

### Factory method

**Motivation.** A base-class algorithm knows *when* objects must be created and *how they fit together*, but not *which concrete class* they should be. Hard-coding `new ConcreteThing()` in the base class would weld the algorithm to one variant.

**Idea.** The base class declares a virtual creation hook — the factory method — and writes its algorithm (a template method) in terms of that hook. Subclasses override the hook to choose the concrete product. The classic illustration is a maze game that comes in two modes, one with ordinary rooms and one with magic rooms; the game-assembly logic is identical, and only room creation varies:

```cpp
class MazeGameCreator {
 public:
  virtual ~MazeGameCreator() = default;
  virtual Room* MakeRoom() = 0;        // the factory method

  // Template method: fixed assembly logic, variable room type.
  MazeGame* CreateGame() {
    auto* game = new MazeGame();
    Room* r1 = MakeRoom();
    Room* r2 = MakeRoom();
    r1->Connect(r2);
    game->AddRoom(r1);
    game->AddRoom(r2);
    return game;
  }
};

class OrdinaryMazeGameCreator : public MazeGameCreator {
  Room* MakeRoom() override { return new OrdinaryRoom(); }
};

class MagicMazeGameCreator : public MazeGameCreator {
  Room* MakeRoom() override { return new MagicRoom(); }
};

// Usage: pick the creator, get the matching game.
MazeGame* ordinary = OrdinaryMazeGameCreator{}.CreateGame();
MazeGame* magic    = MagicMazeGameCreator{}.CreateGame();
```

**Benefits.** New product variants require only a new small subclass; the assembly algorithm is written once.
**Costs.** The pattern *consumes* your inheritance axis: because variation is expressed by subclassing the creator, further subclassing for other reasons becomes challenging, and every new variant adds another class to the hierarchy.

### Abstract factory

**Motivation.** Sometimes what varies is not one product but a *family* of related products that must stay mutually consistent — all widgets of one UI theme, all documents of one house style. Creating family members through scattered `new` calls makes it impossible to guarantee consistency or to switch families cleanly.

**Idea.** Define an interface with one creation method per product in the family; each concrete factory implements them all in a mutually consistent way. For example, a `DocumentCreator` interface might expose `CreateLetter()` and `CreateResume()`; a modern-style factory implements both with contemporary fonts and ragged-right layout, while a classic-style factory implements both with traditional fonts and right-flush layout. Client code receives *some* `DocumentCreator` and calls its methods — it never names a concrete class.

```cpp
class DocumentCreator {
 public:
  virtual ~DocumentCreator() = default;
  virtual std::unique_ptr<Letter> CreateLetter() const = 0;
  virtual std::unique_ptr<Resume> CreateResume() const = 0;
};

class ModernDocumentCreator : public DocumentCreator {
 public:
  std::unique_ptr<Letter> CreateLetter() const override {
    return std::make_unique<ModernLetter>();
  }
  std::unique_ptr<Resume> CreateResume() const override {
    return std::make_unique<ModernResume>();
  }
};

class ClassicDocumentCreator : public DocumentCreator { /* analogous */ };

// Client code is family-agnostic; the family can even change at runtime.
void ProduceDocs(const DocumentCreator& factory) {
  auto letter = factory.CreateLetter();
  auto resume = factory.CreateResume();
}
```

**Benefits.** Whole implementation families become interchangeable — even at runtime — without any change to client code, and family-internal consistency is enforced by construction.
**Costs.** This is the heaviest of the four patterns: it demands more planning and upfront code, and the extra layer of indirection can make the codebase harder to follow — readers must chase through the factory interface to discover what concrete objects actually exist.

### Choosing among them — a rule of thumb

| You need to… | Reach for |
|---|---|
| Tame one constructor with many/optional parameters | Builder |
| Name a construction, cache instances, or pick an optimized subtype | Static factory |
| Let subclasses decide one creation point inside a fixed algorithm | Factory method |
| Swap whole consistent families of products, possibly at runtime | Abstract factory |
