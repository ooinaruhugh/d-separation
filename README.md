# Computing separation criteria in directed acyclic graphs

This repo contains implementations
- of algorithms from the paper "d-Separation: From theorems to algorithms" by Geiger, Verma and Pearl, 1990
- tests derived from Macaulay2 and its package `GraphicalModels` to test this implementation

Reasonable next steps:
- Do the Bayes-Ball algorithm from Ross D. Shachter, *Bayes-Ball: The Rational Pastime (for Determining Irrelevance and Requisite Information in Belief Networks and Influence Diagrams)* In Proceedings of the Fourteenth Conference in Uncertainty in Artificial Intelligence, p. 480–487, 1998.
- \*-separation
- I found (mention of) an applet at https://www.andrew.cmu.edu/user/scheines/tutor/d-sep.html, but it seems to be gone. So d

## Usage
To run anything in this repo, have C++ compiler ready that supports C++20. 
Then, you can use either CMake, so something like
```sh
cd ./build
cmake ..
cmake --build .
```
should do the job. Besides that, there's also a classical GNU Makefile, so just running `make` works and gives you executables for each target. 
To see what targets are available, check `TARGETS` in the [Makefile](Makefile).

## Literature
Dan Geiger and Tom S. Verma and Judea Pearl. *d-Separation: From Theorems to Algorithms* In Proceedings of the Fifth Conference on Uncertainty in Artificial Intelligence (UAI1989), p. 139-148, 1990.