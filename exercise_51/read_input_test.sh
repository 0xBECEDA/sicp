#!/bin/bash

PROGA=proga

if test -x "$PROGA"; then


    echo "\"someshit"\" | ./proga

    sleep 1

    echo "\"(3)"\" | ./proga

    sleep 1

    echo "( 1 2 3)" | ./proga

    sleep 1

    echo "( 1 2 (3 4 ))" | ./proga

    sleep 1

    echo "( 1 2 '(3 4 ))" | ./proga

    sleep 1

    echo "34141" | ./proga

    sleep 1

    echo "\" Monday Friday Sommer Nacht "\" | ./proga

    sleep 1

    echo " \" 'Monday Friday Sommer Nacht \" ()" | ./proga

    sleep 1
    echo "( \"( '(dsac) \" 1 2 'd + \"\" x)" | ./proga

    sleep 1
    echo "( ' \"2\")" | ./proga

    sleep 1
    echo "( ')" | ./proga

    sleep 1
    echo "( )" | ./proga

    sleep 1
    echo "( ''d ) " | ./proga

    sleep 1
    echo " '''d " | ./proga

    sleep 1
    echo " ''() " | ./proga


else
    echo "файл $PROGA не существует\n"
fi
