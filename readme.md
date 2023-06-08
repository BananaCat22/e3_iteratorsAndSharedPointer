# Example of Algebraic/Axiomatic Definition of a Simple Abstract Data Type (ADT)
An ADT is a collection of data values and a collection of operations upon the collection.

Formal/mathematical notation called **axiomatic semantics** may be used to define the meaning, or behavior, of an ADT. The method here is attributed to John Guttag, who not only pioneered the notation/approach, but also defined the systematic procedure of applying the notation so that we are sure all possible important behavior of the type gets defined in the axioms.

This example applies Guttag's method to define semantics for the [Stack ADT](Stack_8h.html).

## Method

1) Define the functional signatures of the operations in the ADT. 

A signature is a function name, then the types of the parameters, then the type of the return value. Parameters are not mutated by functions, and functions may not have side effects that are not specified by Axioms. 

[Stack function signatures](group__StackADT.html)


2) Pick canonical constructors. 

Constructors are functions that return a value of type Stack. These are stack(), push(), and pop(). Of these constructors, a subset are sufficient to build all possible Stack instances. The subset comprises the **Canonical Constructors**, and the others are non-canonical a.k.a. Observers. 

[Canonical Constructors](group__StackADT.html)

3) Write [Axioms](AXIOM.html) to define behavior. 

An axiom is a statement of equivalence. We write one by calling an Observer operation on the Stack produced by a canonical constructor (the Left Hand Side, or LHS), and then creating a Right Hand Side (RHS) expression that we will declare to be equal to (or the same as) the LHS. 
For example,
   size(stack()) == 0
stack() is a canonical constructor, it creates a Stack. size() is an observer. The value for that size() we set as RHS and declare the two expressions to be "==". Another:
   size(push(S,X)) = size(S) + 1
The LHS is the STACK made by push(S,X) passed to the examiner size. Here, S and X are variables representing some arbitrary other Stack value and some arbitrary element of type X.

## Axioms Define Complete Semantics for the ADT

It is theoretically possible that an [Automated Theorum Prover](https://en.wikipedia.org/wiki/Automated_theorem_proving) can automaticly translate the [Axioms](AXIOM.html) into a working implementation of the specified ADT.

An implementation of the ADT may also be created by hand and verified to be correct using one or more test cases corresponding to each axiom. This example provides a [comprehensive set of test cases](tests_8cpp.html) to verify that all axioms are true for the implementation provided. The tests verify that all semantics are implemented and all code within the implementation of the ADT is executed when testing the axioms. 

## Instructions to Build

1) in the directory containing `CMakeLists.txt`, create a `build` directory, change to that directory, and run `cmake ..`:

`mkdir build && cd build && cmake ..`

2) Build and run the ADT and tests:

`make coverage`

3) Generate the documentation:

`make document`

### Build Products

- An executable named `tests` in the `build` directory.
- Also in the `build` directory, code coverage reports documenting how much code was executed by the tests and which specific lines executed: [coverageSummary.txt](../../build/coverageSummary.txt) and [coverageLines.html](../../build/coverageLines.html)
- A `docs/html` directory in the same directory that contains `CMakeLists.txt`. Open `docs/html/index.html` in a web browser to view the generated documentation.
