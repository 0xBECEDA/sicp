#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Этот файл содердит в себе примитивы и предикаты:
- cons, car, cdr, set_car, set_cdr, reverse, append, length, last-pair, make_list, map,
  add, sub, mul, division, assoc
- pair_predicate, atom_predicate, dotpair_predicate, null_predicate, number_predicate,
 symbol_predicate, true_predicate, false_predicate*/

/* возможные типы значений */
enum list_of_types {
    TYPE_INT,
    TYPE_STRING,
    TYPE_SYMBOL,
    TYPE_NIL,
    TYPE_CELL,
    TYPE_ERROR
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

/* forward declarations */
int error_predicate(val* cell);

int atom_predicate (val* cell);

int dotpair_predicate (val* cell);

int pair_predicate (val* cell);

int null_predicate (val* cell);

int number_predicate(val* cell);

int true_predicate(val* cell);

int false_predicate(val* cell);

int symbol_predicate(val* cell);

val* last_pair (val* cell);

val* reverse(val* cell);

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

/* /\* конструктор значения-ссылки на копию буквы *\/ */
/* val* char_val_constructor ( char* val ) { */
/*     struct val * retval = malloc(sizeof(val)); */
/*     char* pnt = malloc(sizeof(char)); */
/*     retval->uni_val.char_val = val; /\* copy val, may be not need (todo) *\/ */
/*     retval->type_num = TYPE_CHAR; */
/*     return retval; */
/* } */

val* symbol_val_constructor ( char* val ) {
    struct val * retval = malloc(sizeof(val));
    char* pnt = malloc(sizeof(char));
    retval->uni_val.char_val = val; /* copy val, may be not need (todo) */
    retval->type_num = TYPE_SYMBOL;
    return retval;
}

val* string_val_constructor ( char* val ) {
    struct val * retval = malloc(sizeof(val));
    char* pnt = malloc(sizeof(char));
    retval->uni_val.char_val = val; /* copy val, may be not need (todo) */
    retval->type_num = TYPE_STRING;
    return retval;
}

/* конструктор значения-ссылки на копию буквы */
val* error_val_constructor ( char* val ) {
    struct val * retval = malloc(sizeof(val));
    char* pnt = malloc(sizeof(char));
    retval->uni_val.char_val = val; /* copy val, may be not need (todo) */
    retval->type_num = TYPE_ERROR;
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
    if ( ( !atom_predicate ( cell ) ) &&
         ( !null_predicate( cell )) ){
        return cell->uni_val.cell_val->car;
    }else {
        error_handler ("ERR CAR: ARG ISN'T A PAIR");
    }
}
/* возвращает cdr ячейки */
val* cdr (val* cell) {
    if ( (!atom_predicate ( cell ) ) &&
         (!null_predicate ( cell ) ) ) {
        return cell->uni_val.cell_val->cdr;
    } else {
        error_handler ("ERR CDR: ARG ISN'T A PAIR");
    }
}

val* set_car (val* cell, val* new_value ) {
    cell->uni_val.cell_val->car = new_value;
    return cell;
}

val* set_cdr (val* cell, val* new_value ) {
    cell->uni_val.cell_val->cdr = new_value;
    return cell;
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
    if ( ( dotpair_predicate(cell1) ) ||
         ( dotpair_predicate(cell2) ) ||
         (atom_predicate( cell1 )) ) {
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

val* reverse_rec(val* list, val* new_cell ) {
    if ( null_predicate( list ) ) {
        return new_cell;

    }else {
        val* car_cell = car ( list );
        val* car_new_cell = malloc(sizeof(val));
        int* int_value = malloc(sizeof(int));
        char* char_value = malloc(sizeof(char[100]));
        cell* cell_value = malloc(sizeof(cell));

        switch ( car_cell->type_num ) {

        case TYPE_INT:
            *int_value  = *car_cell->uni_val.int_val;
            car_new_cell = int_val_constructor( int_value );
            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        case TYPE_SYMBOL:
            *char_value = *car_cell->uni_val.char_val;
            car_new_cell = symbol_val_constructor( char_value );
            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        case TYPE_STRING:
            *char_value = *car_cell->uni_val.char_val;
            car_new_cell = symbol_val_constructor( char_value );
            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        case TYPE_CELL:
            *cell_value = *car_cell->uni_val.cell_val;
            car_new_cell = cell_val_constructor( cell_value );
            reverse_rec(cdr( list ), cons( car_new_cell, new_cell ));
            break;

        default:
            error_handler("ERR REVERSE: WRONG TYPE OF LIST CELL");

        }
    }
}

/* возвращает инверсированную копию списка */
val* reverse(val* cell) {
    if ( pair_predicate( cell )) {
        return reverse_rec( cell, nil_constructor() );
    }
    error_handler( "ERR REVERSE: ARG ISN'T A PAIR");
}

val* add(int n, ...) {

    int* result = malloc(sizeof(int));
    *result = 0;
    val* result_val = int_val_constructor( result );

    if ( n == 0 ) {
        return result_val;

    } else {
        val* cur_arg = malloc(sizeof(val));
        va_list ptr;
        va_start(ptr, n);

        for (int i = 0; i < n; i++) {
            cur_arg = va_arg(ptr, val*);

            if ( number_predicate( cur_arg ) ){
                int the_sum = *result_val->uni_val.int_val;
                int cur_num = *cur_arg->uni_val.int_val;
                the_sum = the_sum + cur_num;
                *result_val->uni_val.int_val = the_sum;

            } else {
                error_handler( "ERR ADD: ARG ISN'T A NUMBER");
            }
        }
        va_end(ptr);
        return result_val;
    }
}

val* mul(int n, ...) {

    int* result = malloc(sizeof(int));
    *result = 1;
    val* result_val = int_val_constructor( result );

    if ( n == 0 ) {
        return result_val;

    } else {
        val* cur_arg = malloc(sizeof(val));
        va_list ptr;
        va_start(ptr, n);

        for (int i = 0; i < n; i++) {
            cur_arg = va_arg(ptr, val*);

            if ( number_predicate( cur_arg ) ){
                int the_sum = *result_val->uni_val.int_val;
                int cur_num = *cur_arg->uni_val.int_val;
                the_sum = the_sum * cur_num;
                *result_val->uni_val.int_val = the_sum;

            } else {
                error_handler( "ERR MUL: ARG ISN'T A NUMBER");
            }
        }
        va_end(ptr);
        return result_val;
    }
}

val* sub(int n, ...) {
    /* если аргументов нет, выдать ошибку */
    if ( n == 0 ) {
        error_handler( "ERR SUB: PROCEDURE CALLED WITH NO ARGS");

    } else {
        int* result = malloc(sizeof(int));
        *result = 0;
        val* result_val = int_val_constructor( result );
        val* cur_arg = malloc(sizeof(val));
        int cur_num;
        int difference;
        va_list ptr;
        va_start(ptr, n);

        /* аргумент один */
        if (n == 1) {
            cur_arg = va_arg(ptr, val*);

            if  (number_predicate( cur_arg ) ) {
                difference = *result_val->uni_val.int_val;
                cur_num = *cur_arg->uni_val.int_val;
                difference = difference - cur_num;
                *result_val->uni_val.int_val = difference;
                return result_val;

            }else {
                error_handler( "ERR SUB: ARG ISN'T A NUMBER");
            }
            /* кол-во аргументов >= 2 */
        } else {
            cur_arg = va_arg(ptr, val*);
            *result_val->uni_val.int_val = *cur_arg->uni_val.int_val;
            n--;

            for (int i = 0; i < n; i++) {
                cur_arg = va_arg(ptr, val*);

                if ( number_predicate( cur_arg ) ){
                    difference = *result_val->uni_val.int_val;
                    cur_num = *cur_arg->uni_val.int_val;
                    difference = difference - cur_num;
                    *result_val->uni_val.int_val = difference;

                } else {
                    error_handler( "ERR SUB: ARG ISN'T A NUMBER");
                }
            }
            va_end(ptr);
            return result_val;
        }
    }
}

val* division(int n, ...) {
    /* если аргументов нет, выдать ошибку */
    if ( n == 0 ) {
        error_handler("ERR DIVISION: PROCEDURE CALLED WITH NO ARGS");

    } else if (n == 1) {
        error_handler("ERR DIVISION: PROSEDURE NEEDS AT LEAST 2 ARGS");
        /* кол-во аргументов >= 2 */
    } else {
        int* result = malloc(sizeof(int));
        *result = 1;
        val* result_val = int_val_constructor( result );
        val* cur_arg = malloc(sizeof(val));
        int cur_num;
        int quotient;
        va_list ptr;
        va_start(ptr, n);

        cur_arg = va_arg(ptr, val*);
        *result_val->uni_val.int_val = *cur_arg->uni_val.int_val;
        n--;

        for (int i = 0; i < n; i++) {
            cur_arg = va_arg(ptr, val*);
            /* аргумент - число? */
            if ( number_predicate( cur_arg ) ) {
                quotient = *result_val->uni_val.int_val;
                cur_num = *cur_arg->uni_val.int_val;
                /* аргумент - ноль? */
                if (cur_num == 0) {
                    error_handler("ERR DIVISION: DIVISION BY ZERO");
                /* делимое больше делителя? */
                } else if (quotient >= cur_num) {
                    quotient = quotient / cur_num;
                    *result_val->uni_val.int_val = quotient;
                }else {
                    error_handler
                        ("ERR DIVISION: THE SYSTEM DOESN'T SUPPORT FRACTIONAL NUMBERS");
                }
            } else {
                error_handler("ERR DIVISION: ARG ISN'T A NUMBER");
            }
        }
        va_end(ptr);
        return result_val;
    }
}

/* принимает функцию и список аргумнтов */
/* применяет функцию к каждому аргументу и возвращает список результатов */
/* в отличие от map из схемы принимает только 1 список аргументов и функцию, принимающую
только 1 аргумент */
val* map(val* (*op)(val*),  val* arg_list) {
    if ((atom_predicate( arg_list )) ||
        (dotpair_predicate( arg_list )) ) {
        error_handler( "ERR MAP: ARG ISN'T A PAIR");

    }else if (null_predicate(arg_list)) {
        nil_constructor();

    }else {
        val* new_car = op(car (arg_list));
        return cons(new_car, map(op, cdr (arg_list)));
    }
}

/* принимает список точечных пар, ищет среди них пару, чей car совпадает с ключом */
val* assoc(val* key, val* args_list) {
    if( null_predicate( args_list ) ) {
        return nil_constructor();

    } else if ( pair_predicate( args_list ) ) {
        val* cur_arg = car( args_list );

        if ( dotpair_predicate ( cur_arg ) ) {
            val* car_cur_arg = car( cur_arg );
            /* если ключ и car текущей точечной пары - числа */
            if ( ( number_predicate( key ) )  &&
                 ( number_predicate( car_cur_arg ) ) &&
                 (*key->uni_val.int_val == *car_cur_arg->uni_val.int_val) ) {
                return cur_arg;
                /* если ключ и car текущей точечной пары - символы */
            } else if ( ( symbol_predicate( key ) )  &&
                        ( symbol_predicate( car_cur_arg ) ) &&
                        (strcmp
                         ( key->uni_val.char_val, car_cur_arg->uni_val.char_val) == 0) ) {
                return cur_arg;
            } else {
                assoc(key, cdr (args_list) );
            }
        } else {
            error_handler("ERR ASSOC: ARGS_LIST SHOULD BE AN ASSOCIATION LIST");
        }
    } else {
        error_handler("ERR ASSOC: ARGS_LIST SHOULD BE AN ASSOCIATION LIST");
    }
}

val* make_list_rec (int n, va_list ptr) {
    if ( n == 0 ) {
        nil_constructor();

    } else {
        val* car = va_arg(ptr, val*);
        return cons(car, make_list_rec( (n - 1), ptr));
    }
}

/* создает список из переданных аргументов */
val* make_list ( int n, ...) {
    if ( n == 0 ) {
        nil_constructor();

    }else {
        val* list = malloc(sizeof(val));
        va_list ptr;
        va_start(ptr, n);
        list = make_list_rec (n, ptr);
        va_end(ptr);
        return list;
    }
}

/* Аналогичный по работае make_list, но с использованием цикла */
/* val* make_list ( int n, ...) { */
/*     if ( n == 0 ) { */
/*         nil_constructor(); */

/*     }else { */
/*         val* list = malloc(sizeof(val)); */
/*         val* car; */
/*         va_list ptr; */
/*         va_start(ptr, n); */
/*         car = va_arg(ptr, val*); */

/*         list = cons(car, nil_constructor()); */
/*         n--; */

/*         for(int i = 0; i < n; i++) { */
/*             car = va_arg(ptr, val*); */
/*             val* car_cell = cons(car, nil_constructor()); */
/*             list = append(list, car_cell); */
/*         } */
/*         return list; */
/*     } */
/* } */

int false_predicate(val* cell) {
    if( TYPE_NIL == cell->type_num ) {
        return 1;
    }
    return 0;
}

int true_predicate(val* cell) {
    if( !false_predicate( cell ) ) {
        return 1;
    }
    return 0;
}

int error_predicate(val* cell) {
    if ( TYPE_ERROR  == cell->type_num ) {
        return 1;
    }
    return 0;
}

int number_predicate(val* cell) {
    if ( ( TYPE_SYMBOL != cell->type_num ) &&
         ( TYPE_STRING != cell->type_num ) &&
         ( TYPE_CELL != cell->type_num ) &&
         ( TYPE_NIL  != cell->type_num )) {
        return 1;
    }
    return 0;
}

int symbol_predicate(val* cell) {
    if ( TYPE_SYMBOL == cell->type_num ) {
        return 1;
    }
    return 0;
}

int string_predicate(val* cell) {
    if ( TYPE_STRING == cell->type_num ) {
        return 1;
    }
    return 0;
}

int atom_predicate (val* cell) {
    if ( ( TYPE_STRING == cell->type_num ) ||
         ( TYPE_SYMBOL == cell->type_num ) ||
         ( TYPE_INT == cell->type_num ) ) {
        return 1;
    }
    return 0;
}

int null_predicate (val* cell) {
    if  ( TYPE_NIL == cell->type_num ) {
        return 1;
    }
    return 0;
}

int pair_predicate (val* cell) {
    if ( ( cell->type_num == TYPE_CELL ) &&
         ( !dotpair_predicate( cell ) )) {
        return 1;

    }else {
        return 0;
    }
}

int dotpair_predicate (val* cell) {
    if (cell->type_num == TYPE_CELL) {
        val* cdr_cell = cdr( cell );

        if( ( cdr_cell->type_num == TYPE_INT ) ||
            ( cdr_cell->type_num == TYPE_STRING ) ||
            ( cdr_cell->type_num == TYPE_SYMBOL ) ) {
            return 1;
        } else {
            return 0;
        }
    } else{
        return 0;
    }
}

/* печатает и элемент в скобки, если это не атом и не пустой список*/
void wrap_brackets_if_not_atom_or_empty_cell (val* car) {
    if ( ( atom_predicate( car ) ) ||
         ( null_predicate( car ) ) ) {
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
    char tmp_char[100];
    val* car_pnt;
    val* cdr_pnt;
    switch ( param->type_num ) {
    case TYPE_INT:
        tmp_int = *param->uni_val.int_val;
        printf( "%d", tmp_int );
        return;
    case TYPE_SYMBOL:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), 100 );
        printf( "%s", tmp_char );
        return;
    case TYPE_STRING:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), 100 );
        printf( "'%s'", tmp_char );
        return;
    case TYPE_NIL:
        printf( "()" );
        return;

    case TYPE_ERROR:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), 100 );
        printf( "%s", tmp_char );
        return;
    case TYPE_CELL:
        car_pnt = car( param );
        cdr_pnt = cdr( param );

        if        (  null_predicate(cdr_pnt) ) {
            /* это одноэлементный список (cdr = NIL) */
            wrap_brackets_if_not_atom_or_empty_cell ( car_pnt );
        } else if ( dotpair_predicate( param ) ) {
            /* это точечная пара (cdr = ATOM)  */
            pprint( car_pnt );
            printf(" . ");
            pprint( cdr_pnt );
        } else if ( pair_predicate( param ) ) {
            /* это список (cdr = CELL) */
            if ( atom_predicate( car_pnt ) ) {
                pprint( car_pnt );
            } else {
                wrap_brackets_if_not_atom_or_empty_cell( car_pnt );
            }
            printf(" ");
            pprint( cdr_pnt );
        } else {
            printf( "\nERR: UNIMPLEMENTED CELL %d : %d", car_pnt->type_num,
                    cdr_pnt->type_num);
            /* todo: добавить обработчик ошибок сюда */
        }
        return;
    default:
        printf( "\nERR: UNIMPLEMENTED");
    }
}

void test_assoc() {
    char* ptr_a = malloc(sizeof(char[1]));
    strncpy( ptr_a, "a", 1 );
    val* a_val = string_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = string_val_constructor( ptr_b );

    char* ptr_c = malloc(sizeof(char[1]));
    strncpy( ptr_c, "c", 1 );
    val* c_val = string_val_constructor( ptr_c );

    char* ptr_d = malloc(sizeof(char[1]));
    strncpy( ptr_d, "d", 1 );
    val* d_val = string_val_constructor( ptr_d );

    int* ptr_1 = malloc(sizeof(int));
    *ptr_1 = 1;
    val* val_1 = int_val_constructor( ptr_1 );

    int* ptr_2 = malloc(sizeof(int));
    *ptr_2 = 2;
    val* val_2 = int_val_constructor( ptr_2 );

    int* ptr_3 = malloc(sizeof(int));
    *ptr_3 = 3;
    val* val_3 = int_val_constructor( ptr_3 );

    int* ptr_4 = malloc(sizeof(int));
    *ptr_4 = 4;
    val* val_4 = int_val_constructor( ptr_4 );

    val* null_val = nil_constructor();

    val* result;
    val* arg_list1 = make_list(4, cons( a_val, val_1), cons( b_val, val_2),
                                  cons( c_val, val_3), cons( d_val, val_4));

    val* arg_list2 = make_list(4, cons( val_1, a_val), cons( val_2, b_val),
                               cons( val_3, c_val), cons( val_4, d_val) );

    val* arg_list3 = make_list(4, cons( val_1, a_val), cons( val_2, b_val),
                               c_val, cons( val_4, d_val) );

    result = assoc( d_val, arg_list1 );
    ipprint(result);
    printf("\n");

    result = assoc( b_val, arg_list1 );
    ipprint(result);
    printf("\n");

    result = assoc( val_4, arg_list1 );
    ipprint(result);
    printf("\n");

    result = assoc( val_4, arg_list1 );
    ipprint(result);
    printf("\n");

    result = assoc( val_4, arg_list2 );
    ipprint(result);
    printf("\n");

    result = assoc( val_3, arg_list2 );
    ipprint(result);
    printf("\n");

    /* проверка обработчика ошибки */
    /* result = assoc( val_3, make_list(3, a_val, b_val, c_val)); */
    /* ipprint(result); */
    /* printf("\n"); */

    /* проверка обработчика ошибки */
    /* result = assoc( val_3, arg_list3); */
    /* ipprint(result); */
    /* printf("\n"); */

    result = assoc( val_1, arg_list3);
    ipprint(result);
    printf("\n");
}

void test_make_list() {

    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = symbol_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int* ptr_f = malloc(sizeof(int));
    *ptr_f = 6;
    val* f_val = int_val_constructor( ptr_f );

    /* теперь соберем список (4 b 5) */
    val* foobar = make_list(3, a_val, b_val, c_val);
    ipprint( foobar );
    printf("\n");

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = make_list(2, a_val,
                          make_list(1,
                                    make_list(2, a_val, c_val)));
    ipprint( bazo );
    printf("\n");

    /* соберем () */
    val* baz = make_list(0);
    ipprint( baz );
    printf("\n");

    /* соберем (()) */
    val* bar = make_list(1, make_list(0));
    ipprint( bar );
    printf("\n");

    /* соберем (4 5 b 6) */
    val* baro = make_list(4, a_val, b_val, c_val, f_val);
    ipprint( baro );
    printf("\n");
}

/* некоторые тесты проверяют срабатывания ошибок, поэтому закомменчены,
   поскольку за ошибкой следует выход и до следующих тестов прога не доходит */
void test_add_sub_mu_division(){
    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = string_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int* ptr_f = malloc(sizeof(int));
    *ptr_f = 6;
    val* f_val = int_val_constructor( ptr_f );

    val* result;

    printf("sub tests: \n");
    result = sub(4, a_val, f_val, f_val, c_val);
    ipprint(  result );
    printf("\n");

    result = sub(1, f_val);
    ipprint(  result );
    printf("\n");

    /* проверка срабатывания обработчика ошибки */
    /* result = sub(0); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    /* проверка срабатывания обработчика ошибки */
    /* result = sub(1, b_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    /* проверка срабатывания обработчика ошибки */
    /* result = sub(4, a_val, f_val, d_val, c_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    printf("add tests: \n");
    result = add(5, a_val, f_val, f_val, c_val, f_val);
    ipprint(  result );
    printf("\n");

    result = add(1, f_val);
    ipprint(  result );
    printf("\n");

    result = add(0);
    ipprint(  result );
    printf("\n");

    /* проверка срабатывания обработчика ошибки */
    /* result = add(4, a_val, f_val, d_val, c_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    printf("mul tests: \n");
    result = mul(5, a_val, f_val, f_val, c_val, f_val);
    ipprint(  result );
    printf("\n");

    result = mul(1, f_val);
    ipprint(  result );
    printf("\n");

    result = mul(0);
    ipprint(  result );
    printf("\n");

    /* проверка срабатывания обработчика ошибки */
    /* result = mul(4, a_val, f_val, d_val, c_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    printf("division tests: \n");

    /* проверка срабатывания обработчика ошибки */
    /* result = division(5, a_val, f_val, f_val, c_val, f_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    /* проверка срабатывания обработчика ошибки */
    /* result = division(1, f_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    result = division(4, mul(5, a_val, f_val, f_val, c_val, f_val), a_val, a_val, f_val);
    ipprint(  result );
    printf("\n");

    /* проверка срабатывания обработчика ошибки */
    /* result = division(0); */
    /* ipprint(  result ); */
    /* printf("\n"); */

    /* проверка срабатывания обработчика ошибки */
    /* result = division(1, b_val); */
    /* ipprint(  result ); */
    /* printf("\n"); */
}

void test_set_car_and_set_cdr () {
    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = symbol_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int* ptr_f = malloc(sizeof(int));
    *ptr_f = 6;
    val* f_val = int_val_constructor( ptr_f );


    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                                 cons( c_val, d_val )),
                                          d_val),
                                   d_val));

    /* теперь соберем список (4 b 5) */
    val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* соберем вложенный список (4 (4 5) 5) */
    val* baz = cons( a_val, cons ( cons( a_val,
                                         cons( c_val, d_val )),
                                   cons( c_val, d_val )));
    /* собираем список (6) */
    val* bar = cons( f_val, nil_constructor());

    /* (6 (4 5) 5)*/
    set_car(baz, f_val);
    ipprint( baz );
    printf("\n");

    /* ((6) b 5) */
    set_car(foobar, bar);
    ipprint( foobar );
    printf("\n");

    /* (4 6) */
    set_cdr(bazo, bar);
    ipprint( bazo );
    printf("\n");

    /* восстанавливаем заничение - иначе получаются циклические списки*/
    bazo = cons( a_val, cons (cons ( cons ( a_val,
                                            cons( c_val, d_val )),
                                     d_val),
                              d_val));
    set_cdr(bar, bazo);
    ipprint( bar );
    printf("\n");

}

void test_map() {
    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = string_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int* ptr_f = malloc(sizeof(int));
    *ptr_f = 6;
    val* f_val = int_val_constructor( ptr_f );

    val* result;

    val* arg_list;

    val* test_fn1(val* arg) {
        add( 2, arg, a_val );
    }

    val* test_fn2(val* arg) {
        sub( 2, arg, a_val );
    }

    val* test_fn3(val* arg) {
        car( arg );
    }


    arg_list = make_list( 5, a_val, c_val, c_val, f_val, a_val );
    printf("arg_list: ");
    ipprint( arg_list );
    printf( "\n" );

    result = map( test_fn1, arg_list );
    ipprint( result );
    printf( "\n" );

    result = map( test_fn2, arg_list );
    ipprint( result );
    printf( "\n" );

    arg_list = make_list( 6, make_list(2,  a_val, c_val),
                          make_list(2,  f_val, c_val),
                          make_list(2,  c_val, f_val),
                          make_list(2,  a_val, a_val),
                          make_list(2,  d_val, d_val),
                          make_list(2,  b_val, c_val));
    printf("arg_list: ");
    ipprint( arg_list );
    printf( "\n" );

    result = map( test_fn3, arg_list );
    ipprint( result );
    printf( "\n" );
}

void test_car_and_cdr () {

    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = string_val_constructor( ptr_b );

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
    val* b_val = string_val_constructor( ptr_b );

    int* ptr_c = malloc(sizeof(int));
    *ptr_c = 5;
    val* c_val = int_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int* ptr_f = malloc(sizeof(int));
    *ptr_f = 6;
    val* f_val = int_val_constructor( ptr_f );

    val* reversed_lst;

    /* теперь соберем список (4 b 5) */
    val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val )));

    /* соберем вложенный список (4 (4 5) 5) */
    val* baz = cons( a_val, cons ( cons( a_val,
                                         cons( c_val, d_val )),
                                   cons( c_val, d_val )));

    val* bazo = cons( a_val, cons (cons ( cons ( a_val,
                                                 cons( c_val, d_val )),
                                          d_val),
                                   d_val));

    reversed_lst = reverse (baz);

    printf( "reversed_lst baz\n" );
    ipprint( reversed_lst );
    printf( "\n" );

    reversed_lst = reverse (foobar);

    printf( "reversed_lst foobar\n" );
    ipprint( reversed_lst );
    printf( "\n" );

    reversed_lst = reverse (bazo);

    printf( "reversed_lst bazo\n" );
    ipprint( reversed_lst );
    printf( "\n" );

    /* проверяем, что reverse создает копию списка,
       а не перекомпоновывает указатели */
    set_cdr(bazo, f_val);

    /* печатает тот же reversed_lst,
       поскольку он является копией и изменения в исходном списке
       никак на него не влияют */
    printf( "reversed_lst bazo \n" );
    ipprint( reversed_lst );
    printf( "\n" );

}

void test_pair () {
    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = string_val_constructor( ptr_b );

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
    val* b_val = symbol_val_constructor( ptr_b );

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
    val* b_val = string_val_constructor( ptr_b );

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
    val* b_val = symbol_val_constructor( ptr_b );

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

/* int main (void) { */
/*     /\* тесты *\/ */

/*     /\* test_reverse(); *\/ */
/*     /\* test_set_car_and_set_cdr(); *\/ */
/*     /\* test_make_list(); *\/ */
/*     /\* test_map(); *\/ */
/*     /\* test_add_sub_mu_division(); *\/ */
/*     /\* test_assoc(); *\/ */
/*     /\* test_pair(); *\/ */
/*     /\* test_ipprint(); *\/ */
/*     /\* test_append (); *\/ */
/*     /\* test_car_and_cdr (); *\/ */
/*     /\* test_length(); *\/ */

/*     return 0; */
/* } */
