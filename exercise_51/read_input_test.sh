#!/bin/bash

PROGA=proga

if test -x "$PROGA"; then

    ./proga | echo 'someshit'

else
    echo "файл $PROGA не существует\n"
fi
