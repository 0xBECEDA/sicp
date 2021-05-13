#!/bin/bash
PROGA=proga
DELAY=5
INPUTPIPE=/tmp/testpipe

make

if test -x "$PROGA"; then

    if [[ ! -p "$INPUTPIPE" ]]; then
        mkfifo "$INPUTPIPE"

    fi

    # создать пустой файл - файл не создается позднее, даже если перенапрвить
    # в него вывод интерпретатора, я не знаю почему

    touch output.txt

    # открыть интерпретатор в тестовом режиме, перенаправить ввод в именованный пайп,
    # вывод перенаправить в файл
    # stdbuf -oL ./proga 0 < "$INPUTPIPE" > output.txt &
    ./proga 0 < "$INPUTPIPE" > output.txt &

    # сохранить десриптор дочернего процесса
    ID=$!

    # передать выражение интерпретатору
    echo "(+ 1 2)" > "$INPUTPIPE"

    # подождать, пока он его выполнит
    sleep $DELAY

    # прочитать 1 строку с конца выходного файла - результат выполнения выражения
    # в 90% случаев не читается ничего. Файл просто пустой. Иногда в нем что-то появляется
    # при этом интерпретатор отдельно от теста работает стабильно
    OUTPUT=$(tail -n 1 output.txt)

    # echo "new output $OUTPUT"

    if [[ "$OUTPUT" == '3' ]]; then
        echo "Результат вычисления (+ 1 2):"
        echo "$OUTPUT"

    else
        echo "test (+ 1 2) failed: expected result 3, but got $OUTPUT"
        kill "$ID"
        exit 1
    fi


    # следующий тест, механизм работы аналогичен предыдущему
    echo "(+ 1 2 (* 4 5) 6)" > "$INPUTPIPE"

    sleep $DELAY

    OUTPUT=$(tail -n 1 output.txt)

    # echo "new output $OUTPUT"

    # в 90% случаев выходной файл не перезаписался (какого-то хера) после прошлого
    # запуска данного тестового файла test3.sh. Поэтому tail считывает неверный результат
    if [[ "$OUTPUT" == '29' ]]; then
        echo "Результат вычисления (+ 1 2 (* 4 5) 6):"
        echo "$OUTPUT"
    else
        echo "test (+ 1 2 (* 4 5) 6) failed: expected result 29, but got $OUTPUT"
        kill "$ID"
        exit 1
    fi


    # следующий тест, механизм работы аналогичен предыдущему
    echo "(define n 5)" > "$INPUTPIPE"

    sleep $DELAY

    OUTPUT=$(tail -n 1 output.txt)

    echo "new output $OUTPUT"

    if [[ "$OUTPUT" == 'ok' ]]; then
        echo "Результат вычисления (define n 5):"
        echo "$OUTPUT"
        kill "$ID"
        exit 0
    else
        echo "test (define n 5) failed: expected result ok, but got $OUTPUT"
        kill "$ID"
        exit 1
    fi


else
    echo "исполняемый файл $PROGA не существует"
fi
