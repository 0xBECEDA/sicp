This mini-project is a solution for Exercise 5.51 of
the sikpa: Writing a scheme interpreter in c.

<img src="https://github.com/0xBECEDA/sicp/blob/master/interpret/demo.gif"
width="700" height="500" />

This interpreter supports calculation:
- primitives: cons, car, cdr, set-car, set-cdr, length, list, etc.
- arithmetic operations
- values of variables (only defined using define)
- conditional expressions (with if and cond operators)
- lambda functions
- named functions (including recursive ones)

To launch:
- download the entire project
- compile it using Makefile (type make in the console). After executing this command, the interpret file should appear in your folder.
- run interpret: to do this, type in the console./interpret

Experiment!
As ideas for the first experiments, you can use
expressions from the file test.org , which you will find in the same catalog.

Copy the expression you like into the interpreter - follow the
brackets - and see what happens.
