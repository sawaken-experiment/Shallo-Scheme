Shallo-Scheme
==============
Shallo-Scheme is my own implementation of Scheme interpreter.

Many essential elements of Scheme language are implemented.

(But, this implementation does NOT meet any official Scheme SPEC such as R5RS, R6RS. So, Shallo-Scheme is not Scheme exactly.)


### Features ###
    Continuation (call/cc)
    Macro (not hygienic)
    Tail Call Optimization
    Exception System
    Garbage Collector

### Usage ###
    $ git clone https:/github.com/sawaken/Shallo-Scheme.git
    $ cd Shallo-Scheme/src
    $ make
    $ ./scheme [source file]

### Try REPL (Read-Eval-Print-Loop) after make ###
    $ cd Shallo-Scheme/sample_program
    $ ../src/scheme repl.scm


What is shallow?
----------------
Of course my knowledge and technique of language-implementation and C-language-programming are.
This is my first time of implementing programming-language-interpreter, so I have done almost all things by feel (my own way).

Bugs
----
There are countless Bugs in Shallo-Scheme...
And surprisingly, there are no Tests for this program...

Destination
-----------
This was developed completely for my learning.
I feel like updating (or remaking) and documenting this in the future when having time.
