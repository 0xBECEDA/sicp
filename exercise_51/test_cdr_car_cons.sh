#!/bin/bash

PROGA=proga

if test -x "$PROGA"; then

# car tests

echo "CAR TESTS"

echo "(car '(1 2 3))" | ./proga

echo "(car '((1 2 3) 4 5))" | ./proga

echo "(car '(d 4 5))" | ./proga

echo "(car 'd)" | ./proga

echo "(car 1)" | ./proga

echo "(car (cons 1 2))" | ./proga

echo "(car (cons 'd 2))" | ./proga


# cdr tests
echo "CDR TESTS"

echo "(cdr '(1 2 3))" | ./proga

echo "(cdr '((1 2 3) 4 5))" | ./proga

echo "(cdr '(d 4 5))" | ./proga

echo "(cdr 'd)" | ./proga

echo "(cdr 1)" | ./proga

echo "(cdr (cons 1 2))" | ./proga

echo "(cdr (cons 'd 2))" | ./proga


# car_cdr tests
echo "CAR AND CDR TESTS"

echo "(car (cdr '(1 2 3)))" | ./proga

echo "(car (cdr '(1 2 3))))" | ./proga

echo "(cdr (car '((1 2 3) 4 5)))" | ./proga

echo "(cdr( cdr '(1 2 3 (4 (5 6)))))" | ./proga

echo "(car (car '((1 2 3) 4 5)))" | ./proga


# cons tests
echo "CONS TESTS"

echo "(cons '(1 2 3) '(4 5))" | ./proga

echo "(cons 'd 'f)" | ./proga

echo "(cons 'd 'f 's)" | ./proga

echo "(cons 'd)" | ./proga

echo "(cons 1 2)" | ./proga

echo "(cons 1 (cons 2 3))" | ./proga

echo "(cons (cons 1 2) 3)" | ./proga

echo "(cons (cons 1 (cons 2 3)) 4)" | ./proga

else
    echo "файл $PROGA не существует\n"
fi
