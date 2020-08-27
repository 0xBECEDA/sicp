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

/* forward declaration */
int atom_predicate (val* cell);

int dotpair_predicate (val* cell);

int pair_predicate (val* cell);

void pprint(val* param);

void error_handler(char* str);

void ipprint (val* param);

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

    if (!atom_predicate ( cell )) {
        return cell->uni_val.cell_val->car;
    }else {
        error_handler ("ERR CAR: ARG ISN'T A PAIR");
    }
}
/* возвращает cdr ячейки */
val* cdr (val* cell) {

    if (!atom_predicate ( cell )) {
        return cell->uni_val.cell_val->cdr;
    } else {
        error_handler ("ERR CDR: ARG ISN'T A PAIR");
    }
}

/* todo: додеделать */
void error_handler(char* str) {
    printf("\n %s \n", str);
    exit(0);
}

int length_rec (val* cell) {
    if ( cell->type_num == TYPE_NIL ) {
        return 0;
    } else {
        return (1 + (length_rec( cdr( cell ))));
    }
}

/* возвращает длину списка */
int length (val* cell) {
    if ( pair_predicate( cell )) {
        return length_rec( cell );
    }
    error_handler( "ERR LENGTH: ARG ISN'T A PAIR");
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
    /* printf("append\n"); */

    if ( dotpair_predicate(cell1) ||
         dotpair_predicate(cell2) ||
         atom_predicate( cell1 )) {
        error_handler( "APPEND ERR: ARGS SCHOULD BE LISTS");

    }else if ( cell1->type_num == TYPE_NIL ) {
        return cell2;

    } else if ( cell2->type_num == TYPE_NIL) {
        return cell1;

    } else {
        val* last_pair_cell1 = last_pair( cell1 );
        last_pair_cell1->uni_val.cell_val->cdr = cell2;
        return cell1;
    }
}

/* val* reverse_rec(val* cell, val* new_cell ) { */
/*     printf("\n reverse_rec cell: "); */
/*     ipprint( cell ); */
/*     printf("\n"); */

/*     printf("\n reverse_rec new_cell: "); */
/*     ipprint( new_cell ); */
/*     printf("\n"); */

/*     if ( cell->type_num == TYPE_NIL ) { */
/*         return new_cell; */
/*     } */

/*     printf("\n"); */
/*     ipprint(car ( cell )); */
/*     printf("\n"); */
/*     reverse_rec( cdr( cell ), */
/*                 append( new_cell, car ( cell ))); */
/* } */

/* здесь все валится */
val* reverse_rec(val* list, val* new_cell ) {
    printf("\n reverse_rec cell: ");
    ipprint( list );
    printf("\n");

    if ( list->type_num == TYPE_NIL ) {
        return new_cell;

    }else {
        val* car_cell = car ( list );
        printf("car\n");
        ipprint(car_cell );
        printf("\n");

        printf("car\n");
        ipprint( car_cell );
        printf("\n");

        val* car_new_cell;
        int* int_value;
        char* char_value;
        cell* cell_value;

        printf("%d\n", car_cell->type_num);
        printf("здесь \n");

        switch ( car_cell->type_num ) {

        case TYPE_INT:
            printf("\n reverse_rec int: ");
            ipprint( car_cell );

            *int_value  = *car_cell->uni_val.int_val;
            car_new_cell = int_val_constructor( int_value );

            printf("\n reverse_rec int car_new_cell: ");
            ipprint( car_new_cell );

            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        case TYPE_CHAR:
            printf("\n reverse_rec char: ");
            ipprint( car_cell );

            *char_value = *car_cell->uni_val.char_val;
            car_new_cell = char_val_constructor( char_value );

            printf("\n reverse_rec char car_new_cell: ");
            ipprint( car_new_cell );

            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        case TYPE_CELL:
            printf("\n reverse_rec cell: ");
            ipprint( car_cell );

            *cell_value = *car_cell->uni_val.cell_val;
            car_new_cell = cell_val_constructor( cell_value );

            printf("\n reverse_rec cell car_new_cell: ");
            ipprint( car_new_cell );

            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        default:
            printf("error\n");

        }
    }
}

/* возвращает инверсированную копию списка */
val* reverse(val* cell) {
    printf("\n reverse \n");
    ipprint( cell );
    if ( pair_predicate( cell )) {
        return reverse_rec( cell, nil_constructor() );
    }
    error_handler( "ERR REVERSE: ARG ISN'T A PAIR");
}

int atom_predicate (val* cell) {
    if ((TYPE_CHAR == cell->type_num ) ||
        (TYPE_INT == cell->type_num )) {
        return 1;
    }
        return 0;
}

int pair_predicate (val* cell) {
    /* printf("\n pair_predicate \n"); */
    if (cell->type_num == TYPE_CELL &&
        !dotpair_predicate( cell )) {
        return 1;

    }else {
        return 0;
    }
}

int dotpair_predicate (val* cell) {
    /* printf("\n dotpair_predicate \n"); */
    /* ipprint(cell); */

    if (cell->type_num == TYPE_CELL) {

        val* cdr_cell = cdr( cell );

        if( cdr_cell->type_num == TYPE_INT ||
            cdr_cell->type_num == TYPE_CHAR ) {
            return 1;
        } else {
            return 0;
        }
    } else{
        return 0;
    }
}

/* печатает и элемент в скобки, если это не атом */
/* void wrap_brackets_if_not_atom (val* car) { */
/*     if ( atom_predicate( car ) ) { */
/*         pprint( car ); */
/*     } else { */
/*         printf("("); */
/*         pprint( car ); */
/*         printf(")"); */
/*     } */
/* } */

/* печатает и элемент в скобки, если это не атом и не пустой список*/
void wrap_brackets_if_not_atom_or_empty_cell (val* car) {
    if ( atom_predicate( car ) ||
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
    wrap_brackets_if_not_atom_or_empty_cell (param);
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
        printf( "()" );
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
            if ( atom_predicate( car_pnt ) ) {
                pprint( car_pnt );
            } else {
                wrap_brackets_if_not_atom_or_empty_cell( car_pnt );
                /* printf("("); */
                /* pprint( car_pnt ); */
                /* printf(")"); */
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

void test_reverse() {
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

    val* reversed_lst;

    /* теперь соберем список (4 b 5) */
    val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* соберем вложенный список (4 (4 5) 5) */
    val* baz = cons( a_val, cons ( cons( a_val,
                                         cons( c_val, d_val )),
                                   cons( c_val, d_val )));
    reversed_lst = reverse (foobar);

    /* printf( "reversed_lst baz\n" ); */
    /* ipprint( reversed_lst ); */
    /* printf( "\n" ); */

    /* reversed_lst = reverse (foobar); */

    /* printf( "reversed_lst foobar\n" ); */
    /* ipprint( reversed_lst ); */
    /* printf( "\n" ); */
}

void test_pair () {
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

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                                 cons( c_val, d_val )),
                                          d_val),
                                   d_val));
    printf( "pair? bar %d\n", pair_predicate( bar));

    printf( "pair? bazo %d\n", pair_predicate( bazo));

    printf( "pair? dotpair %d\n", pair_predicate( dotpair));

    printf( "pair? d_val %d\n", pair_predicate( d_val));

    printf( "pair? c_val %d\n", pair_predicate( c_val));

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

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                                 cons( c_val, d_val )),
                                          d_val),
                                   d_val));

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

    /* восстановили значение foobar */
    foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* собираем список из (4 b 5) и () - должен вернуть первый список*/
    appended_lst = append (foobar, d_val);

    printf( "appended_lst foobar + d_val \n" );
    ipprint( appended_lst );
    printf( "\n" );

    /* восстановили значение foobar */
    foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* собираем список (() 4 b 5)  */
    appended_lst = append (bazon, foobar);

    printf( "appended_lst bazon + foobar \n" );
    ipprint( appended_lst );
    printf( "\n" );

    /* /\* собираем список из  (4 . 5) и () */
    /*    - ошибка, append не может работать с дотпарами*\/ */
    /* append(dotpair, d_val); */

    /* собираем список из (4 b 5 . 5) */
    appended_lst = append (foobar, c_val);

    printf( "appended_lst foobar + с_val \n" );
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
    /* test_reverse(); - пока не включать */
    test_pair();
    /* test_ipprint(); */
    /* test_append (); */
    /* test_car_and_cdr (); */
    /* test_length(); */

    return 0;
}
