#!/bin/bash

PROGA=proga

if test -x "$PROGA"; then

    echo "\"someshit"\" | ./proga

    sleep 1

    echo "(begin ( + a b) (define x 5) (set! s 4))" | ./proga

    sleep 1

    echo "(begin ( + a b))" | ./proga

    sleep 1

    echo "(set! s (factorial 3))" | ./proga

    sleep 1

    echo "(set! s (factorial 3)))" | ./proga

    sleep 1

    echo "(set! d)" | ./proga

    sleep 1

    echo "(factorial 3)" | ./proga

    sleep 1

    echo "factorial 3)" | ./proga

    sleep 1

    echo "2" | ./proga

    sleep 1

    echo "'2" | ./proga

    sleep 1

    echo "''2" | ./proga

    sleep 1

    echo "fib" | ./proga

    sleep 1

    echo "(lambda (b v d) (+ b v d) (let ((a 3) (b 4)) (+ a b)))" | ./proga

    sleep 1

    echo "(let ((a 3) (b 4)) (+ a b))" | ./proga

    sleep 1

    echo "(let ((a 3) '(b 4)) (+ a b))" | ./proga

    sleep 1

    echo "(define x 5)" | ./proga

    sleep 1

    echo "(define (test x) ( - x 5))" | ./proga

    sleep 1
    echo "(define (test 'x) ( - x 5))" | ./proga

    sleep 1

    echo "(if (null? e) x y )" | ./proga

    sleep 1

    echo "(if (null? e) (begin (+  y c) (+ d f)) nil )" | ./proga

    sleep 1

    echo "(if (null? e) (begin (+  y c) (+ d f)))" | ./proga

    sleep 1

    echo "(if (null? e) ((begin (+  y c) (+ d f)))" | ./proga


else
    echo "файл $PROGA не существует\n"
fi
