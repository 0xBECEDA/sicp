#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* сборка и запуск: */
/* gcc test.c -o test */
/* ./test */

/* возможные типы значений */
enum list_of_types {
               TYPE_INT,
               TYPE_CHAR,
               TYPE_NIL,
               TYPE_CELL,
};

/* тип списка */
typedef struct cell {
    struct val * car;
    struct val * cdr;
} cell;


/* тип значения */
typedef struct val {
    int type_num;
    /* значение может быть любого перечисленного типа */
    union uni_val {
        int* int_val;
        char* char_val;
        cell* cell_val;
    } uni_val;
} val;


/* конструктор значения-ссылки на копию числа */
val* int_val_constructor ( int* val ) {
    struct val * retval = malloc(sizeof(val));
    int* pnt = malloc(sizeof(int));
    retval->uni_val.int_val = val; /* copy val, may be not need (todo) */
    retval->type_num = TYPE_INT;
    return retval;
}

/* конструктор значения-ссылки на копию буквы */
val* char_val_constructor ( char* val ) {
    struct val * retval = malloc(sizeof(val));
    char* pnt = malloc(sizeof(char));
    retval->uni_val.char_val = val; /* copy val, may be not need (todo) */
    retval->type_num = TYPE_CHAR;
    return retval;
}

/* конструктор NIL-значения */
val* nil_constructor () {
    struct val * retval = malloc(sizeof(cell));
    retval->type_num = TYPE_NIL;
    return retval;
}

/* конструктор значения-ссылки на ячейку */
val* cell_val_constructor ( cell* val ) {
    struct val * retval = malloc(sizeof(cell));
    retval->uni_val.cell_val = val;
    retval->type_num = TYPE_CELL;
    return retval;
}

/* конструктор списка */
val* cons  (val* car, val* cdr ) {
    cell *cell = malloc(sizeof(cell));
    cell->car = car;
    cell->cdr = cdr;
    return cell_val_constructor( cell );
}

/* возвращает car ячейки */
val* car (val* cell) {
    return cell->uni_val.cell_val->car;
}

/* возвращает cdr ячейки */
val* cdr (val* cell) {
    return cell->uni_val.cell_val->cdr;
}

/* возвращает длину списка */
int length (val* cell) {
    if ( cell->type_num == TYPE_NIL ) {
        return 0;
    } else {
        return (1 + (length( cdr( cell ))));
    }
}

/* возвращает последнюю пару списка */
val* last_pair (val* cell) {
    if ( cell->type_num == TYPE_NIL ) {
        printf( "\nERR LAST_PAIR: EMPTY CELL %d ", cell->type_num);

    } else {
        val* cdr_pnt = cdr( cell );
        if ( cdr_pnt->type_num == TYPE_NIL) {
            return cell;

        } else {
            last_pair (cdr_pnt);
        }
    }
}

/* склеивает 2 списка вместе */
val* append (val* cell1, val* cell2) {

    if ( cell1->type_num == TYPE_NIL ) {
        return cell2;

    } else if ( cell2->type_num == TYPE_NIL) {
        return cell1;

    } else {
        val* last_pair_cell1 = last_pair( cell1 );
        last_pair_cell1->uni_val.cell_val->cdr = cell2;
        return cell1;
    }
}

int atom_predicate (int type_num) {
    if ((TYPE_CHAR == type_num ) ||
        (TYPE_INT == type_num )) {
        return 1;
    }
        return 0;
}


void pprint(val* param); /* forward declaration */

/* печатает и элемент в скобки, если это не атом */
void wrap_brackets_if_not_atom (val* car) {
    if ( atom_predicate( car->type_num ) ) {
        pprint( car );
    } else {
        printf("(");
        pprint( car );
        printf(")");
    }
}

/* печатает и элемент в скобки, если это не атом и не пустой список*/
void wrap_brackets_if_not_atom_or_empty_cell (val* car) {
    if ( atom_predicate( car->type_num ) ||
         car->type_num == TYPE_NIL)  {
        pprint( car );
    } else {
        printf("(");
        pprint( car );
        printf(")");
    }
}

/* вызывает вывод списка, оборачивает скобками, если param != атом
   и todo:устанавливая отступы */
void ipprint (val* param) {
    wrap_brackets_if_not_atom (param);
}

/* выводит список */
void pprint(val* param) {
    int tmp_int;
    char tmp_char[1];
    val* car_pnt;
    val* cdr_pnt;
    switch ( param->type_num ) {
    case TYPE_INT:
        tmp_int = *param->uni_val.int_val;
        printf( "%d", tmp_int );
        return;
    case TYPE_CHAR:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), 1 );
        printf( "%s", tmp_char );
        return;
    case TYPE_NIL:
        printf( "NIL" );
        return;
    case TYPE_CELL:
        car_pnt = car( param );
        cdr_pnt = cdr( param );

        if  (         ( TYPE_NIL == cdr_pnt->type_num ) ) {
            /* это одноэлементный список (cdr = NIL) */
            wrap_brackets_if_not_atom_or_empty_cell ( car_pnt );
        } else if (   ( TYPE_CHAR == cdr_pnt->type_num ) ||
                      ( TYPE_INT  == cdr_pnt->type_num ) ) {
            /* это точечная пара (cdr = ATOM)  */
            pprint( car_pnt );
            printf(" . ");
            pprint( cdr_pnt );
        } else if (   ( TYPE_CELL == cdr_pnt->type_num ) ) {
            /* это список (cdr = CELL) */
            if ( atom_predicate( car_pnt->type_num ) ) {
                pprint( car_pnt );
            } else {
                printf("(");
                pprint( car_pnt );
                printf(")");
            }
            printf(" ");
            pprint( cdr_pnt );
        } else {
            printf( "\nERR: UNIMPLEMENTED CELL %d : %d", car_pnt->type_num,
                    cdr_pnt->type_num);
        }
        return;
    default:
        printf( "\nERR: UNIMPLEMENTED");
    }
}

void test_car_and_cdr () {

    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = char_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                                 cons( c_val, d_val )),
                                          d_val),
                                   d_val));

    val* cart = car( bazo );
    ipprint( cart );
    printf( "\n" );

    val* cdrt = cdr( bazo );
    ipprint( cdrt );
    printf( "\n" );

    val* cadrt =  car(cdr( bazo ));
    ipprint( cadrt );
    printf( "\n" );


}
/* тест функции append - дотестить */
void test_append () {

    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = char_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    val* appended_lst;

    /* теперь соберем список (4 5) */
    val* bar = cons( a_val, cons( c_val, d_val ));

    /* теперь соберем список (4 b 5) */
    val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* соберем вложенный список (4 (4 5) 5) */
    val* baz = cons( a_val, cons ( cons( a_val,
                                         cons( c_val, d_val )),
                                   cons( c_val, d_val )));

    /* создадим точечную пару (4 . 5) */
    val* dotpair = cons( a_val, c_val );

    /* соберем список из одного элемента NIL */
    val* bazon = cons( d_val, d_val );

    /* собираем список (4 b 5 ()) */
    appended_lst = append (foobar, bazon);

    printf( "appended_lst bazon + foobar \n" );
    ipprint( appended_lst );
    printf( "\n" );

    /* восстановили значение foobar */
    foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* собираем (4 5 4 b 5) */
    appended_lst = append (bar, foobar);

    printf( "appended_lst bar + foobar \n" );
    ipprint( appended_lst );
    printf( "\n" );

    /* собираем список из (4 b 5) и () */
    appended_lst = append (foobar, d_val);

    printf( "appended_lst foobar + d_val \n" );
    ipprint( appended_lst );
    printf( "\n" );

    /* восстановили значение foobar */
    foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* собираем список ((()) 4 b 5) - здесь неправильно печатается/аппендится*/
    appended_lst = append (bazon, foobar);

    printf( "appended_lst bazon + foobar \n" );
    ipprint( appended_lst );
    printf( "\n" );

    /* собираем список из  (4 . 5) и () - вообще-то так нельзя. todo предикат!*/
    appended_lst = append (dotpair, d_val);

    printf( "appended_lst dotpair + d_val \n" );
    ipprint( appended_lst );
    printf( "\n" );
}

void test_ipprint() {

    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = char_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    /* распечатаем */
    pprint( a_val );
    printf( "\n" );
    pprint( b_val );
    printf( "\n" );
    pprint( c_val );
    printf( "\n" );
    pprint( d_val );
    printf( "\n" );

    /* создадим список из одного эл-та = точечную пару (4 . NIL) */
    val* single = cons( a_val, d_val );

    /* распечатаем  */
    ipprint( single );
    printf( "\n" );


    /* создадим точечную пару (4 . 5) */
    val* dotpair = cons( a_val, c_val );

    /* распечатаем её */
    ipprint( dotpair );
    printf( "\n" );


    /* свяжем её с пятеркой спереди (5 4 . 5) */
    val* foo = cons( c_val, dotpair );

    /* распечатаем */
    ipprint( foo );
    printf( "\n" );

    /* теперь соберем список (4 5) */
    val* bar = cons( a_val, cons( c_val, d_val ));

    /* распечатаем */
    ipprint( bar );
    printf( "\n" );

    /* теперь соберем список (4 b 5) */
    val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* распечатаем */
    ipprint( foobar );
    printf( "\n" );

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                                 cons( c_val, d_val )),
                                          d_val),
                                   d_val));
    /* распечатаем */
    ipprint( bazo );
    printf( "\n" );

    /* соберем список из одного элемента NIL */
    val* bazon = cons( d_val, d_val );

    /* распечатаем */
    ipprint( bazon );
    printf( "\n" );

    /* соберем список ((()))*/
    val* bazont = cons (cons( d_val, d_val ), d_val);

    /* распечатаем */
    ipprint( bazont );
    printf( "\n" );


}

void test_length() {
    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = char_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int length_lst;

    /* соберем вложенный список (4 (4 5) 5) */
    val* baz = cons( a_val, cons ( cons( a_val,
                                         cons( c_val, d_val )),
                                   cons( c_val, d_val )));

    length_lst = length( baz );
    printf( "length baz: %d \n", length_lst );

    /* список (4 b 5) */
    val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    length_lst = length( foobar );
    printf( "length foobar: %d\n", length_lst );

    length_lst = length( d_val );
    printf( "length d_val: %d\n",  length_lst );

}

int main (void) {

    /* тесты */
    test_ipprint();
    test_append ();
    test_car_and_cdr ();
    test_length();

    return 0;
}
