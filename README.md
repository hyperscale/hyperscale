The Hyperscale Programming Language [![Build Status](https://img.shields.io/travis/hyperscale/hyperscale/master.svg)](https://travis-ci.org/hyperscale/hyperscale)
===================================

This is the main source code repository for Hyperscale. It contains the compiler, standard library, and documentation.

This project is EXPERIMENTAL. The behaviour of this project, its name, and surrounding documentation may change without notice in a future release.

Compile and run
---------------

~~~
mkdir build
cd build/
cmake ../
make
./bin/hyperscale debug lexer ./test.hs
~~~

Features and roadmap
--------------------

- [ ] Command Line
    - [ ] Debug : Command for debugging compiler during development 
        - [x] Lexer
        - [ ] Parser
    - [ ] Build : Command for build project written in hyperscale
    - [ ] Test : Command for test project written in hyperscale
    - [ ] Lint : Command for linting project written in hyperscale
    - [ ] Package : Command for manage package in project written in hyperscale
- [ ] Compiler 
    - [x] Lexer
    - [ ] Parser
    - [ ] AST
    - [ ] Code Generator (LLVM)
- [ ] Standard Library
    - [ ] Spec
- [ ] Documentation
    - [ ] Grammar/Lexical Structure
    - [ ] Command Line
    - [ ] Standard Library


License
-------

See the [LICENSE](LICENSE.md) file for license rights and limitations (MIT).
