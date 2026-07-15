# Design Problems

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 17. See epilight_cpp_new.pdf for the full text. All exposition below is original commentary on the chapter's topics.*

## Overview

System design questions ask the candidate to architect a set of services or a complete system, frequently layered on top of an algorithm designed earlier in the interview. Unlike coding questions, they are deliberately open-ended — many of them could seed a multi-year software project — so the interviewer is not grading you against a single reference answer. What is being evaluated is the *conversation*: can you think creatively, articulate design trade-offs, and make progress on a problem you have never seen?

A strong answer does three things:

1. **Sketches the core data structures and algorithms.** Every large system has an algorithmic heart, and identifying it early anchors the rest of the discussion.
2. **Names a concrete technology stack** — programming language, libraries, operating system, hardware footprint, and third-party services — and justifies the choices.
3. **Surfaces the cross-cutting concerns** that distinguish an engineer from a coder: implementation time, extensibility, scalability, testability, security, internationalization, and intellectual-property constraints.

Keep expectations calibrated: an interview answer is a demonstration of good engineering judgment under time pressure, not a comprehensive state-of-the-art design document.

## The recurring design principles

Three principles cover most of the ground you will need in a design interview.

| Principle | What to demonstrate |
|---|---|
| Algorithms and data structures | Identify the fundamental algorithmic problem hiding inside the system, and the data structures that serve it. |
| Decomposition | Split functionality, architecture, and code into manageable, reusable components with clean interfaces. |
| Scalability | Partition the problem into subproblems solvable nearly independently on separate machines; shard data that does not fit on one box; decouple write paths from read paths and replicate the read side; cache repeated computation instead of redoing it. |

### Decomposition

Good decomposition is the single most reliable way to make progress on a system-level question, and it applies at three distinct levels.

- **Functional decomposition** — break the *goals* apart. A useful trick is to organize goals by stakeholder. For an online-advertising platform, advertisers, end users, and the platform operator each want different things, and enumerating those wants generates the requirements list almost automatically.
- **Architectural decomposition** — break the *system* apart. The same advertising platform splits naturally into a front-end (user and account management, web page design, reporting and analytics) and a back-end (middleware, storage, the database proper, scheduled/cron services, and the ad-ranking algorithms). Each box can then be designed, staffed, and scaled independently.
- **Code decomposition** — break the *implementation* apart. This is the home turf of object-oriented design patterns, the discipline of finding reusable shapes for code; patterns are conventionally grouped into creational, structural, and behavioral families. In practice, certain patterns appear over and over in real codebases without anyone planning for them — strategy objects, adapters, and builders in particular. For a readable, example-driven treatment, Freeman et al.'s *Head First Design Patterns* is a widely recommended starting point.

### Scalability

In interview terms, parallelism matters when the problem is *too big for one machine* — either the data does not fit, or a single machine would take unacceptably long. The insight the interviewer wants to see is a decomposition with two specific properties:

- each subproblem can be solved (almost) independently of the others, and
- the global answer can be assembled *cheaply* from the subproblem answers.

"Cheaply" is measured in whatever resource dominates: CPU time, RAM, network bandwidth, or the count of memory and database accesses.

**Worked example — sorting a petascale integer array.** One machine cannot hold, let alone sort, a petabyte of integers. Two strategies, chosen by what you know about the data:

- *Distribution known:* carve the value domain into equal-probability ranges and route each range to its own machine. Each machine sorts its range locally; because the ranges are ordered and disjoint, the final result is simply the concatenation of the machines' outputs in range order — the combine step is trivial.
- *Distribution unknown:* give every machine an arbitrary equal-sized slice of the input. Each machine sorts its slice, and the coordinator performs a k-way merge of the sorted streams, most naturally with a min-heap keyed on the head element of each stream. The combine step is now real work, but it is linear and streaming-friendly.

The general lesson: knowledge about the data distribution buys you a cheaper combine step; absent that knowledge, pay for a merge.

**Caching.** Whenever a computation repeats, store the result the first time and look it up thereafter. This one idea shows up at every scale:

- Dynamic programming is, at its core, caching the results of intermediate computations so overlapping subproblems are solved exactly once.
- Long-lived services see heavily repeated requests — web workloads are famously skewed toward a small set of hot queries — so a cache in front of the expensive path (computation, database, remote call) converts a throughput problem into a hit-rate problem.

## 17.1 Design a system for detecting copyright infringement

**Problem.** A popular video-sharing site (call it YouTV.com) is under pressure from movie studios who claim that much of the uploaded content infringes their copyrights. Design a feature that lets a studio submit a set **V** of videos it owns and receive back the set of videos in the site's database that match members of V.

*Hint: normalize every video to a common format, then compute signatures over the normalized form.*

**Solution discussion.**

*Why video matching is harder than document matching.* Two files containing the same movie are almost never byte-identical: the same content can be wrapped in different container formats, encoded at different resolutions, and compressed at different quality levels. Any design that compares raw bytes — or hashes of raw bytes — is dead on arrival.

*Step 1 — normalization.* Reduce the video problem to the (well-studied) duplicate-document problem by transcoding every video — both the studio's reference set and the site's corpus — into one canonical format, resolution, and compression level. Normalization does not make matching content bit-identical: the encoder's initial settings still influence the output, so two normalizations of the same movie differ in their bytes. What normalization buys is that a *content signature* computed on the normalized stream becomes meaningful.

*Step 2 — signatures.* A signature is a compact, comparison-friendly summary of content, designed so that perceptually identical videos yield identical (or near-identical) signatures. Signatures form a quality ladder:

- **Trivial:** one bit per frame — is the frame brighter or darker than the average? Cheap to compute and store, but many unrelated videos will collide.
- **Better:** a few bits per frame — e.g., a 3-bit measure of the red, green, and blue intensities of each frame. Still cheap, considerably more discriminating.
- **Better still:** per-frame features computed over *regions* of the frame, so the signature captures spatial structure rather than a single global average.

The design driver for climbing this ladder is the **false-match rate**. Every candidate match the system reports must be reviewed (ultimately by a human somewhere in the pipeline), so a more discriminating signature directly reduces review time and cost. This is the classic precision/recall trade-off: the signature must be robust to re-encoding noise (so true copies are found) yet specific enough that unrelated content rarely collides.

*Matching at scale.* Once every video is a sequence of per-frame signature values, matching V against the corpus becomes a sequence-similarity search, and standard document-matching machinery applies: index the corpus signatures, generate candidate matches cheaply by hashing signature windows, then verify candidates with a more expensive comparison. The two-phase structure — cheap candidate generation followed by expensive verification — is what keeps the system economical when the corpus is enormous and true matches are sparse.

*Non-algorithmic complements.* A production feature would not rely on signatures alone, and raising alternatives shows breadth:

- **Crowdsourcing:** let users flag infringing videos, possibly rewarding them for accurate flags.
- **Match against known infringers:** compare each new upload against videos previously adjudicated as infringing, so each work is litigated once rather than repeatedly.
- **Metadata inspection:** header fields, titles, durations, and similar metadata are weak signals individually but useful in combination.

Mentioning these demonstrates that you understand the algorithm is one component of a larger socio-technical system involving incentives, review workflows, and adversarial uploaders.

**Variant.** Design an online music-identification service — the audio analogue: given a short, noisy clip captured on a phone, identify the song. The same normalize-then-fingerprint skeleton applies, but the signature must survive microphone noise, distortion, and arbitrary time offset, which pushes the design toward robust audio fingerprints (features derived from the spectrogram) and an index keyed on fingerprint fragments so a few seconds of audio suffice for lookup.
