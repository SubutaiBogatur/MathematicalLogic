# MathematicalLogic

The repository contains homeworks for Mathematical Logic course in ITMO University, Autumn 2016. They are mostly parsers and processors of proofs of expressions.

* **Classical Logic Proof Checker** gets a file with *Hilbert-style* proof and checks, if its correct. Moreover, all lines in proof are being commented. It is supposed, that proof was made with 10 basic axioms of Classical Logic and also with hypotheses mentioned in the title of the file with proof. Proof Checker has parser inside it, which builds *Abstract Syntax Tree* and then checks its structure. See `samples` for some proof examples

* **Predicate Calculus Proof Processor** is a tool for working with proofs of expressions in formal arithmetics and in *Predicate Calculus*. It is an extension of first homework with some cool features. To begin with, it supports *Classical Logic* proofs. Moreover, it supports proofs in *Predicate Calculus* (ie with predicates and quantifiers). It is also able to process proofs in [formal arithmetics](https://en.wikipedia.org/wiki/Peano_axioms). Full list of axioms which can be used in proof can be seen in `axioms.cpp` file. See `samples` directory for correct proof examples. Another cool feature of the program is that it is able to use [deduction theorem](https://en.wikipedia.org/wiki/Deduction_theorem) for the given proof. So, it rebuilds the proof removing the last hypothesis. 

* **Formal Inequalities** is the last homework. The program written generates proofs of given statements in formal arithmetics. It can prove inequalities. It is funny to generate proofs in this program and then make them an input of the second homework to check their correctness or to rebuild the proof to get rid of hypotheses.
