This mini-project is a limited compiler that
compiles code from scheme to c and writes the output code to a
file c-code-file.txt which will automatically appear in your current
directory.

The compiler supports compilation:
- arithmetic operations
- conditional expressions (using the if operator only)
- definitions and function calls (including recursive ones)
- definitions and calls of lambda functions
- defining variables and calculating their values

If you want to compile multiple expressions, then combine them into
a single block using begin.

Important: the compiled code file is overwritten after each
compilation. If you enter an expression, it is immediately
compiled and written to a file. Therefore, you should combine
expressions into a single side using begin, if you want all of them
to get into the output file, and not just the last one.

It also follows from this that if you once defined some
function or variable and compiled it, and then decided
to add something to your program, then you must enter all the definitions again,
combine them into a single block and compile it that way. Otherwise
during the compilation of your system code, you will encounter
unrecognized variables.

For a quick start:
- merge the entire project
- install mit-scheme - interpreter of the scheme language - if you have
  he's not here yet.
  You can download it from here
  https://www.gnu.org/software/mit-scheme/

  Other interpreters may use slightly different syntax,
  therefore, trying to run a project under them will cause errors.

- run mit-scheme
- copy all the code from the code.scheme file to a working interpreter
- enter (driver-loop)
- enter any expression
- open the file c-code-file.txt
- transfer the code from it to a file with the *.c permission
- compile the file as gcc primitives.c your_file.c -o your_file

All project documentation is contained in the file doc.org .
