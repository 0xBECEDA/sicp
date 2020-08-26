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


int atom_predicate (int type_num) {
    if ((TYPE_CHAR == type_num ) ||
        (TYPE_INT == type_num )) {
        return 1;
    }
        return 0;
}


void pprint(val* param); /* forward declaration */


void wrap_brackets_if_not_atom (val* car) {
    if ( atom_predicate( car->type_num ) ) {
        pprint( car );
    } else {
        printf("(");
        pprint( car );
        printf(")");
    }
}

/* выводит список */
void pprint(val* param) {
    int tmp_int;
    char tmp_char[1];
    val* car;
    val* cdr;
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
        car = param->uni_val.cell_val->car;
        cdr = param->uni_val.cell_val->cdr;
        if  (         ( TYPE_NIL == cdr->type_num ) ) {
            /* это одноэлементный список (cdr = NIL) */
            wrap_brackets_if_not_atom( car );
        } else if (   ( TYPE_CHAR == cdr->type_num ) ||
                      ( TYPE_INT  == cdr->type_num ) ) {
            /* это точечная пара (cdr = ATOM)  */
            pprint( car );
            printf(" . ");
            pprint( cdr );
        } else if (   ( TYPE_CELL == cdr->type_num ) ) {
            /* это список (cdr = CELL) */
            if ( atom_predicate( car->type_num ) ) {
                pprint( car );
            } else {
                printf("(");
                pprint( car );
                printf(")");
            }
            printf(" ");
            pprint( cdr );
        } else {
            printf( "\nERR: UNIMPLEMENTED CELL %d : %d", car->type_num, cdr->type_num);
        }
        return;
    default:
        printf( "\nERR: UNIMPLEMENTED");
    }
}

/* вызывает вывод списка, оборачивая скобками и todo:устанавливая отступы */
void ipprint (val* param) {
    printf( "(" );
    pprint( param );
    printf( ")" );
}


int main (void) {

    /* инициализируем структуры, которые будут значениями ячеек списка */
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

    /* соберем вложенный список (4 (4 5) 5) */
    val* baz = cons( a_val, cons ( cons( a_val,
                                            cons( c_val, d_val )),
                                      cons( c_val, d_val )));

    /* распечатаем */
    ipprint( baz );
    printf( "\n" );

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                           cons( c_val, d_val )),
                                          d_val),
                                   d_val));
    /* распечатаем */
    ipprint( bazo );
    printf( "\n" );

    /* соберем список из одного элемента NIL - тут надо поправить */
    val* bazon = cons( d_val, d_val );
    /* распечатаем */
    ipprint( bazon );
    printf( "\n" );


    return 0;
}
