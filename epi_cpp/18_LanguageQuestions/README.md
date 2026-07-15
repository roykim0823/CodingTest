# Language Questions

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 18. See epilight_cpp_new.pdf for the full text. All exposition below is original commentary on the chapter's topics.*

## Overview

Interviewers frequently open with short questions about the C++ language itself. The set in this chapter is not a comprehensive language review — it is a *preparedness test*. If these questions feel difficult, that is a strong signal to go back and study the language before interviewing; if they feel easy, you are probably calibrated correctly.

Language questions probe something different from algorithm questions: they check whether you understand the semantics underneath the syntax you write every day — the object model, the memory model, what the compiler does on your behalf, and why the language provides two mechanisms where one might seem to suffice. When answering, explain not only *what* a feature does but *why* it exists and what its alternatives cost.

## 18.1 References and pointers

**Question.** What is a reference? How does it differ from a pointer?

**Discussion.**

At first glance references and pointers look interchangeable: both designate some other target object, and both let a function operate on data it does not own. The deep difference is one of identity:

> A **pointer is a variable in its own right** — it occupies storage, holds an address as its value, and can be inspected and reassigned like any other variable. A **reference is an alias** — another name for an existing object, with no independent identity of its own in the language model.

Everything else follows from that distinction.

### Dereferencing and address-of

- **Pointer:** evaluating `ptr` yields the *address* it stores. Reaching the target's value requires an explicit dereference, `*ptr`. The indirection is visible at every use site.
- **Reference:** evaluating `ref` yields the *target's value* directly — the compiler silently inserts the dereference for you. The indirection is invisible. Conversely, if you want the address of the target, you must ask for it explicitly with the address-of operator, `&ref` (which gives the address of the referent, since the reference has no separate identity to take the address of).

```cpp
int x = 42;

int* p = &x;          // p stores x's address
int  v1 = *p;         // explicit dereference to read x

int& r = x;           // r is another name for x
int  v2 = r;          // implicit dereference: reads x directly
int* q = &r;          // &r yields x's address, i.e., q == p
```

### Rebinding and null

- **Pointer:** can be reseated to a different target at any time, and can be set to `nullptr` to indicate "points at nothing." Code that consumes pointers must therefore contend with the null case (and, in badly managed code, with dangling pointers).
- **Reference:** can only be *initialized*, never reseated. Writing `r = something` after initialization does not rebind `r`; it assigns through the alias into the original object. There is no null reference in well-formed C++ — a reference must be bound to an object at birth — which is precisely why APIs often use references to express "this argument is required" and pointers to express "this argument is optional."

```cpp
int a = 1, b = 2;

int* p = &a;
p = &b;          // fine: the pointer now points at b
p = nullptr;     // fine: the pointer points at nothing

int& r = a;      // r is bound to a, permanently
r = b;           // does NOT rebind r; it assigns b's value (2) into a
```

### A pointer is itself an object

Because a pointer is a real variable, it has an address of its own, and that address can be stored in another pointer — giving multi-level indirection:

```cpp
int   x  = 7;
int*  px  = &x;    // pointer to int
int** ppx = &px;   // pointer to pointer to int

**ppx = 8;         // writes through two levels: x == 8
```

No analogous construction exists for references: you cannot form a "reference to a reference," an array of references, or a pointer to a reference, because a reference is not an object with storage the language will let you name. (Attempting `int& arr[3]` or `int&* p` is ill-formed.)

### Summary table

| Property | Pointer | Reference |
|---|---|---|
| Own storage / identity | Yes — a variable holding an address | No — an alias for its target |
| Evaluating it yields | The stored address | The target's value (auto-deref) |
| Getting the target's value | Explicit `*ptr` | Automatic |
| Getting the target's address | `ptr` itself | Explicit `&ref` |
| Reseating to a new target | Allowed | Never — assignment goes to the referent |
| Null state | `nullptr` allowed | Must be bound at initialization |
| Indirection depth | Arbitrary (`int**`, `int***`, …) | One level only |

### Interview follow-ups worth anticipating

- *When would you choose each?* References for required, non-null function parameters and operator overloading (where pointer syntax would be unusable); pointers when reseating, null, or pointer arithmetic is needed, or when interfacing with C.
- *What is a dangling reference?* A reference outliving its target (e.g., returning a reference to a local variable) is undefined behavior — references are not lifetime-safe, only reseat-safe.
- *`const` interplay:* a `const T&` parameter accepts temporaries and avoids copies — the idiomatic read-only argument type for anything larger than a machine word.
