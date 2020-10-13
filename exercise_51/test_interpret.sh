#!/bin/bash
PROGA=proga
PIPE=/tmp/testpipe

DELAY=5
if test -x "$PROGA"; then

    if [[ ! -p "$PIPE" ]]; then
        mkfifo "$PIPE"
    else

        if [[ ! -p "$PIPE" ]]; then
            echo "Reader not running"
            exit 1
        else

            ./proga < "$PIPE" &
            ID=$!

            echo "(+ 1 2)" > "$PIPE"
            echo "(+ 1 2 4 5 6)" > "$PIPE"
            echo "(+ 1 2 (* 4 5) 6)" > "$PIPE"

            echo "(define (factorial n)
                   (if (= n 0)
                       1
                       (* n (factorial (- n 1)))))" > "$PIPE"

            echo "(factorial 3)" > "$PIPE"
            echo "(factorial 6)" > "$PIPE"
            echo "(define n 5)" > "$PIPE"
            echo "n" > "$PIPE"
            echo "(factorial n)" > "$PIPE"

            echo "(define (my-reverse lst)
                  (define (my-reverse lst new-lst)
                   (if (null? lst)
                       new-lst
                       (my-reverse (cdr lst) (cons (car lst) new-lst))))
                  (my-reverse lst '()))" > "$PIPE"

            echo "(my-reverse '(1 2 3 4))" > "$PIPE"
            echo "(my-reverse '(1 (2 3) 4))" > "$PIPE"
            echo "(my-reverse '(1 (2 3) 4) 'd)" > "$PIPE"


            echo "(define a 3)" > "$PIPE"
            echo "(define b 4)" > "$PIPE"

            echo "(if (and (> b a) (< b (* a b)))
                     b
                     a)"  > "$PIPE"

            echo "(+ 2
                  (if (> b a)
                      b
                      a))" > "$PIPE"

            echo "(define (fib n)
                  (cond ((= n 0) 0)
                   ((= n 1) 1)
                   (else (+ (fib (- n 1))
                          (fib (- n 2))))))" > "$PIPE"

            echo "(fib 8)" > "$PIPE"
            echo "(fib 0)" > "$PIPE"

            echo "(quite)" > "$PIPE"

            sleep "$DELAY"
            # kill "$ID"
            echo "done"

        fi
    fi
else
    echo "исполняемый файл $PROGA не существует"
fi
