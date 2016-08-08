# JavaScript AST

A simple JavaScript parser producing simple AST. Play with AST with whatever way
you want. It's quite fast and also so simple! Just create an AST visitor.

The project aims at generating AST and analysing it for bugs that are otherwise difficult to
find. This project is still under development and can take lot of time to reach its beta stage.
There may be bugs, memory leaks etc. So beware before using it.

### Grammar
jast uses Recursive Descent parser to parse JavaScript. It can parse ECMA-262 5.1 grammar.

### Requirements
 + CMake
 + A C++ compiler supporting C++-11 standard
 + `flex` may be required if Lexical Analyser is not already generated.

### Building
Project is developed on Linux so will work on it. It may or may not work on other platforms

    mkdir build (in case you don't want to pollute parser/ directory)
    cd build
    cmake -DTEST_JAST=1 ..
    # you can run samples as
    ./samples/parse
    # to run tests
    ./tests/unittest

### Samples
See `samples/`

### Documentation
Documentation is also under development but you can find that `./docs/`
