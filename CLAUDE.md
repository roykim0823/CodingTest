# CLAUDE.md

EPI (Elements of Programming Interviews) study repo. Chapter 01 (`epi_cpp/01_PrimitiveType/`)
is the reference implementation of all conventions below — when working on another chapter,
make it look like chapter 01.

## Repo layout

- `epi_cpp/NN_ChapterName/` — one dir per chapter, numbered by the EPI *sampler* PDF
  (`epilight_cpp_new.pdf`): 01–21. Full-book chapter = sampler chapter + 3
  (e.g. sampler ch. 1 Primitive Types = book ch. 4).
- Inside each chapter: `README.md` + `cpp/` + `python/` (+ `rust/` planned).
- Shared, single-copy infrastructure (never copy per chapter):
  - `epi_cpp/test_framework/` — C++ test framework headers
  - `epi_cpp/include/` — C++ prototype headers (`list_node.h`, `binary_tree_node.h`, …)
  - `epi_cpp/pylib/` — Python `test_framework/` package + prototype modules
    (`list_node.py`, `bst_node.py`, …)
- `test_data/` (repo root) — language-agnostic `.tsv` test data.
- `epi_python/` — pool of not-yet-migrated Python solutions.
- `problem_mapping.js` — maps every problem to book chapter, problem number, and per-language
  file names. Use it to script migrations (boot camps are numbered `X.00`).

## File naming

- **C++**: plain `snake_case.cc`, no numeric prefixes — problem ordering lives in the chapter
  README and the `epi_cpp/README.md` index (chapter 01 is already unnumbered). Chapters not yet
  given the full README treatment still carry legacy `NN_` prefixes; strip them as part of
  bringing a chapter up to the ch-01 standard (update any `#include` of renamed files and the
  README code paths in the same pass).
- **Python**: canonical EPIJudge names, **no numeric prefixes** — solutions import each other
  by module name and `01_parity` is not a valid module name.
- Keep the `.tsv` names in `GenericTestMain(...)` / `generic_test_main(...)` unchanged —
  they must match `test_data/`.

## C++ conventions

- Multi-solution files use **method postfixes**: `Parity_brute_force`, `Parity_iter`,
  `Parity_lookup`, `Parity_xor` (not complexity-based names).
- Every solution function gets a uniform complexity comment directly above it:
  `// O(n) time, O(1) space, n = word size` — format `O(time), O(space), var = meaning`.
- Test `main` runs *all* variants through `GenericTestMain`.
- Reusing another solution: `#define main _main` / `#include "01_other.cc"` / `#undef main`;
  cross-chapter includes use relative paths like `../../04_LinkedLists/cpp/01_sorted_lists_merge.cc`.
- Build is hierarchical *and* standalone: `epi_cpp/CMakeLists.txt` add_subdirectory()s every
  chapter; each chapter's CMakeLists is 4 lines (`cmake_minimum_required` + `project(NN_Name)`
  + `include(../cmake/chapter.cmake)`) so it also configures on its own. All shared logic
  (flags, include dirs, glob `cpp/*.cc`, one executable per file named by stem) lives in
  `epi_cpp/cmake/chapter.cmake`. Chapter-specific exclusions: set `EPI_EXCLUDE_SRCS` to a
  regex before the include (see `15_Graphs` for its main-less LeetCode snippets).

## Python conventions

- Each chapter `python/` dir contains symlinks into `../../pylib/`: `test_framework` plus the
  six prototype modules — this makes bare `python3 file.py` work from inside the dir.
- `epi_python/` has the same symlinks, so unmigrated solutions also run in place.
- Cross-chapter solution imports (e.g. heaps importing `sorted_arrays_merge`): symlink the
  imported module into the importing chapter's `python/` dir (mirrors the C++ relative include).
- Migrating a chapter from `epi_python/`: read the file list from `problem_mapping.js`,
  `git mv` into `NN_Chapter/python/`, create the seven symlinks (+ any cross-import
  symlinks), verify by running each file in place.

## README.md format (per chapter — mirror `01_PrimitiveType/README.md`)

Ordered sections:
1. `# Chapter Name` + italic attribution line referencing the sampler PDF.
2. `## Overview`
3. `## Boot camp: ...` — problem, idea, code, example, complexity.
4. `## Top tips`, `## Know your <library>`.
5. One `## N.M Title` per problem, containing in order:
   - `**Implementations:** [C++](cpp/NN_file.cc) · [Python](python/file.py)` (Rust later).
   - `**Problem.**` statement, then `*Hint: ...*`.
   - `### Approach ...` headings ordered brute force → optimized, each with complexity in
     the heading, prose explanation, and a worked example or ASCII diagram where it helps.
   - `***cpp/NN_file.cc***` (bold-italic code path) on its own line before every code block;
     the code block must match the source file **exactly** (function names included).
   - `**Complexity.**` line per approach.
   - `**Variants:**` list; `**Python note.**` only where the languages genuinely diverge
     (e.g. Python's non-negative `%`, arbitrary-precision ints needing explicit masking).

**Content policy:** READMEs are original exposition — never transcribe or lightly rephrase
the book's text; write explanations independently and keep only the user's own code.

## Verify before claiming done

- C++: top-level `cmake -S epi_cpp -B <scratch>/build && cmake --build <scratch>/build`
  builds every chapter (`epi_cpp/CMakeLists.txt` uses `add_subdirectory`); a chapter also
  configures standalone. Run binaries with `--test-data-dir <repo>/test_data` — expect
  "passed ALL tests". (The old flat-layout `Makefile` was removed.)
- Python: bare `python3 file.py` from inside the `python/` dir must pass.
- If a README code block and its source file drift, fix the source or the README — they
  must stay identical.

## Current state (2026-07-15)

- Chapters 01–02 fully done (README, unnumbered cpp files, python merged, symlinks, tests green).
- Chapters 03–21: READMEs exist as study-note summaries (not yet full ch-01 treatment);
  Python still in `epi_python/`; cpp files still carry legacy `NN_` prefixes.
- Nothing committed yet — all work is staged/unstaged in the working tree.
