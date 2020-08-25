#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


/* сборка и запуск: */
/* gcc test.c -o test */
/* ./test */


/* структура значения ячейки*/
typedef struct val {

    void * val;
    int typeNum;

}val;

/* структура ячейки */
typedef struct cell {

    struct val * value;
    struct cell * next;

}cell;

// globals
int a = 4;
char b[1] = "b";
int c = 5;

/* создает пустой список  */
cell* makeEmptyList() {

    cell *lst = (cell*)malloc(sizeof(cell));
    lst->value = NULL;
    lst->next = NULL;
    return lst;
}
/* создает первую ячейку списка */
cell* initList(val *value) {
    cell *lst = (cell*)malloc(sizeof(cell));
    lst->value = value;
    lst->next = NULL;
    return lst;
}


/* печатает пустой список */
int printEmptyList () {
    puts("()");
}

/* создает список */
cell* makelist (int amountElts, ...) {

    cell *lst, *curCell, *prevCell;
    val * value = (val*)malloc(sizeof(val));

    va_list values;
    va_start(values, amountElts);
    *value = va_arg(values, val);
    /* проинициализировали список первым элементов */
    lst = initList(value);

    amountElts--;
    prevCell = lst;

    for (int i = 0; i <= amountElts; i++) {
        /* элементы кончились */
        if (i == amountElts) {
            /* создаем путую ячейку и присоединяем ее к списку */
            curCell = (cell*)malloc(sizeof(cell));
            curCell = makeEmptyList();
            prevCell->next = curCell;
            va_end(values);
        } else {
            /* иначе создаем новую ячейку */
            curCell = (cell*)malloc(sizeof(cell));
            /* выделили память под структуру значения */
            value = (val*)malloc(sizeof(val));
            /* записали в нее структуру значения */
            *value = va_arg(values, val);
            /* записали структуру как значение ячейки */
            curCell->value = value;
            /* создали пустой указатель на след. ячейку */
            curCell->next = NULL;
            /* связали предыдущую и текущую ячейки указателем */
            prevCell->next = curCell;
            /* запомнили текущую структуру */
            prevCell = curCell;
        }
    }
    return lst;
}

/* печатает значение всех ячеек списка */
int printListValues (cell *lst) {

    /* если список пустой или у текущей ячейки нет значения */
    if (lst == NULL || lst->value == NULL) {
        /* выход */
        return 0;

        /* иначе */
    } else {

        /* получили структуру значения ячейки */
        cell * curCell = lst;
        val * curValue = curCell->value;

        /* поулучили порядковый номер типа и завели пару указателей */
        int typeNum = curValue->typeNum;
        int *pnt;
        char *pointer;

        switch (typeNum) {

        /* значение изначально было интом */
        case 0:
            /* печатаем как инт */
            pnt = (int*)curValue->val;
            printf("%d ", *pnt);
            printListValues(curCell->next);
            break;

         /* значение изначально было char */
        case 1:
            pointer = (char*)curValue->val;
            printf("%s ", pointer);
            printListValues(curCell->next);
            break;

        default:
            printf("printListValues: не распознан тип значения");
        }
    }
}
/* печатает список в лисповом виде */
int printList (cell *lst) {

    if (lst == NULL) {
        printf("printList: передан указатль NULL\n");
        /* return -1; */

    /* это пустой список */
    }else if (lst->value == NULL) {

        printEmptyList();

    } else {
        cell * curCell = lst;
        printf("(");
        printListValues (curCell);
        printf(")\n");
        return 0;
    }
}

/* тест печати */
int testPrintList (cell *lst) {

    printf("testPrintList: lst %p\n", lst);
    printList(lst);
    return 0;
}

/* тест создания списка и его печати */
/* возвращает указатель на список */
cell* testMakeList() {

    /* инициализируем структуры, которые будут значениями ячеек списка */
    /* в мэйне есть точно такие же */

    val *aStruct = (val*)malloc(sizeof(val));
    val *bStruct = (val*)malloc(sizeof(val));
    val *cStruct = (val*)malloc(sizeof(val));

    aStruct->val = (void*)&a;
    bStruct->val = (void*)&b;
    cStruct->val = (void*)&c;

    /* помечаем типы порядковым номером, */
    /* чтоб можно было привести значения к изначальным типам */

    /* типу инт соответствует 0 */

    aStruct->typeNum = 0;
    cStruct->typeNum = 0;
    bStruct->typeNum = 1;

    val struct1 = *aStruct;
    val struct2 = *bStruct;
    val struct3 = *cStruct;

    /* создаем список */
    cell * lst1 = makelist(3, struct1, struct2, struct3);

    printf("printList from testMakeList:\n");

    testPrintList(lst1);

    testPrintList(lst1);

    testPrintList(lst1);

    return lst1;
}

int main (void) {

    /* инициализируем структуры, которые будут значениями ячеек списка */
    val *aStruct = (val*)malloc(sizeof(val));
    val *bStruct = (val*)malloc(sizeof(val));
    val *cStruct = (val*)malloc(sizeof(val));

    aStruct->val = (void*)&a;
    bStruct->val = (void*)&b;
    cStruct->val = (void*)&c;

    /* помечаем типы порядковым номером, */
    /* чтоб можно было привести значения к изначальным типам */

    /* типу инт соответствует 0 */
    aStruct->typeNum = 0;
    cStruct->typeNum = 0;
    /* типу char соответствует 1 */
    bStruct->typeNum = 1;


    val struct1 = *aStruct;
    val struct2 = *bStruct;
    val struct3 = *cStruct;

    /* вызов построения списка */
    cell * lst1 = makelist(3, struct1, struct2, struct3);

    /* при отладке здесь все печатается правильно */
    printf("main: lst1 %p\n", lst1);
    printf("main: testPrintList lst1 from main: \n");
    testPrintList(lst1);
    testPrintList(lst1);
    testPrintList(lst1);


    /* вызов построения нового списка */
    cell * lst2 = testMakeList();

    /* при отладке здесь все печатается НЕ правильно */
    printf("main: testPrintList lst2 from main: \n");
    testPrintList(lst2);

    testPrintList(lst2);

    testPrintList(lst2);

    return 0;
}
