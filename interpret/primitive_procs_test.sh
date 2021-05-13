#!/bin/bash

PROGA=proga

if test -x "$PROGA"; then

    echo "LENGTH TESTS"
    echo "(length '(1 2 3 4 5))" | ./proga
    echo "(length '(1 (2 3) 4 5))" | ./proga
    echo "(length '(1 (2 3 4 5)))" | ./proga
    echo "(length '(1 2 3 (4 5)))" | ./proga
    echo "(length (cons 1 2))" | ./proga
    echo "(length 'd)" | ./proga
    echo "(length '())" | ./proga
    echo "(length '((1)))" | ./proga
    echo "(length \"string\")" | ./proga

    echo "ASSOC TESTS"
    echo "(assoc 'a (list (cons 'a 1) (cons 'b 2) (cons 'd 3)))" | ./proga
    echo "(assoc 'b (list (cons 'a 1) (cons 'b 2) (cons 'd 3)))" | ./proga
    echo "(assoc 'c (list (cons 'a 1) (cons 'b 2) (cons 'd 3)))" | ./proga
    echo "(assoc 'c (list 1 2 3 4))" | ./proga

    echo "APPEND TESTS"
    echo "(append '(1 2 3 4) '(5 6 7 8))" | ./proga
    echo "(append '() '(5 6 7 8))" | ./proga
    echo "(append '(1 2 3 4) '())" | ./proga
    echo "(append '(1 2 3 4) (cons 5 6))" | ./proga
    echo "(append '() '())" | ./proga
    echo "(append '(1 2 3 (4 5)) (list 6 7 (append '(8 9) '(10 11) )))" | ./proga

    echo "LAST_PAIR TESTS"
    echo "(last-pair (append '(1 2 3 4) '(5 6 7 8)))" | ./proga
    echo "(last-pair 'd)" | ./proga
    echo "(last-pair 2)" | ./proga
    echo "(last-pair '(1 2 (3 4) (5 6)))" | ./proga

    echo "LIST TESTS"
    echo "(list '(1 2 3 4) '(5 6 7 8))" | ./proga
    echo "(list 'd)" | ./proga
    echo "(list 2)" | ./proga
    echo "(list 1 2 '(3 4) '(5 6) \"string\" '+)" | ./proga

else
    echo "файл $PROGA не существует\n"
fi
