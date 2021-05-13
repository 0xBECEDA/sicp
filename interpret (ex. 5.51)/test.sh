#!/bin/bash

# make
# DELAY=5

# spawn ./test_interpret.sh

# expect "3"
# expect "3"

VAR=0

while true
      echo "цикл начался"
do

    ./test2.sh > test2.txt

    # tail -n 1 -f test2.txt LINE
    # echo "прочитанная строка $LINE"

    # LINE=`tail -n 1 -f test2.txt`
    # LINE=$(echo -n "read_input: ввод законечен")
    LINE=$(tail -n 1 test2.txt)
    # if read line; then
    echo "прочитанная строка $LINE"
    # echo "Hi!"
    if [[ "$LINE" == 'read_input: ввод законечен' ]]; then
        echo "закончили"
        break

    else
        echo "не та строка"
         break
    fi
        echo "цикл продолжается"

done
