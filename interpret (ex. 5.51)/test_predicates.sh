#!/bin/bash

PROGA=proga

if test -x "$PROGA"; then

    echo "TRUE? TESTS"
    echo "(true? '(1 2))" | ./proga
    echo "(true? 2)" | ./proga
    echo "(true? '())" | ./proga
    echo "(true? 'd)" | ./proga
    echo "(true? \"2\")" | ./proga
    echo "(true? (cons 1 2))" | ./proga

    echo "FALSE? TESTS"
    echo "(false? '(1 2))" | ./proga
    echo "(false? '())" | ./proga
    echo "(false? 2)" | ./proga
    echo "(false? \"2\")" | ./proga
    echo "(false? 'd)" | ./proga
    echo "(false? (cons 1 2))" | ./proga

    echo "NULL? TESTS"
    echo "(null? '(1 2))" | ./proga
    echo "(null? '())" | ./proga
    echo "(null? 2)" | ./proga
    echo "(null? 'd)" | ./proga
    echo "(null? \"2\")" | ./proga
    echo "(null? (cons 1 2))" | ./proga

    echo "PAIR? TESTS"
    echo "(pair? '(1 2))" | ./proga
    echo "(pair? '())" | ./proga
    echo "(pair? 'd)" | ./proga
    echo "(pair? 2)" | ./proga
    echo "(pair? \"2\")" | ./proga
    echo "(pair? (cons 1 2))" | ./proga

    echo "ATOM? TESTS"
    echo "(atom? '(1 2))" | ./proga
    echo "(atom? '())" | ./proga
    echo "(atom? 'd)" | ./proga
    echo "(atom? (cons 1 2))" | ./proga
    echo "(atom? \"2\")" | ./proga
    echo "(atom? 2)" | ./proga

    echo "SUMBOL? TESTS"
    echo "(symbol? '(1 2))" | ./proga
    echo "(symbol? '())" | ./proga
    echo "(symbol? 'd)" | ./proga
    echo "(symbol? (cons 1 2))" | ./proga
    echo "(symbol? \"2\")" | ./proga
    echo "(symbol? 2)" | ./proga

    echo "STRING? TESTS"
    echo "(string? '(1 2))" | ./proga
    echo "(string? '())" | ./proga
    echo "(string? 'd)" | ./proga
    echo "(string? (cons 1 2))" | ./proga
    echo "(string? \"2\")" | ./proga
    echo "(string? 2)" | ./proga

    echo "DOTPAIR? TESTS"
    echo "(dotpair? '(1 2))" | ./proga
    echo "(dotpair? '())" | ./proga
    echo "(dotpair? 'd)" | ./proga
    echo "(dotpair? (cons 1 2))" | ./proga
    echo "(dotpair? \"2\")" | ./proga
    echo "(dotpair? 2)" | ./proga

else
    echo "файл $PROGA не существует\n"
fi
