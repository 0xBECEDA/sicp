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

int max_symbol_name_length = 100;
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

int equal_numbers_predicate( val* n1, val* n2 );

val* last_pair(val* cell);

val* reverse(val* cell);

void pprint(val* param);

void error_handler(char* str);

void ipprint (val* param);

/* объясления ошибок */
val* car_error;
val* cdr_error;
val* last_pair_error;
val* append_error;
val* reverse_rec_error;
val* reverse_error;
val* add_error;
val* mul_error;
val* sub_rec_error;
val* sub_error1;
val* sub_error2;
val* division_rec_error1;
val* division_rec_error2;
val* division_error1;
val* division_error2;
val* map_error;
val* assoc_error;
val* ipprint_error;

/* конструктор значения-ссылки на копию числа */
val* int_val_constructor ( int* val ) {
    struct val * retval = malloc(sizeof(val));
    int* pnt = malloc(sizeof(int));
    retval->uni_val.int_val = val; /* copy val, may be not need (todo) */
    retval->type_num = TYPE_INT;
    return retval;
}

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

/* инициализирует сообщения об ошибках */
val* init_primitives_errors () {
    char *string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR CAR: arg isn't a pair", max_symbol_name_length );
    car_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR CDR: arg isn't a pair", max_symbol_name_length );
    cdr_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR LAST_PAIR: empty cell", max_symbol_name_length );
    last_pair_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR APPEND: args schold be a lists", max_symbol_name_length );
    append_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR REVERSE: wrong type of cell", max_symbol_name_length );
    reverse_rec_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR REVERSE: arg ist't a pair", max_symbol_name_length );
    reverse_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ADD ERROR: add gets only numbers", max_symbol_name_length );
    add_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "MUL ERROR: mul gets only numbers", max_symbol_name_length );
    mul_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "SUB ERROR: sub gets only numbers", max_symbol_name_length );
    sub_rec_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "SUB ERROR: sub needs at least 1 arg", max_symbol_name_length );
    sub_error1 = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "DIVISION ERROR: division by zero", max_symbol_name_length );
    division_rec_error1 = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "DIVISION ERROR: division does't support fractional numbers",
             max_symbol_name_length );
    division_rec_error2 = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "DIVISION ERROR: division gets only numbers",
             max_symbol_name_length );
    division_error1 = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "DIVISION ERROR: division needs at least 2 args",
             max_symbol_name_length );
    division_error2 = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR MAP: arg isn't a pair", max_symbol_name_length );
    map_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR ASSOC: list of args should be an association list",
             max_symbol_name_length );
    assoc_error = error_val_constructor( string );
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
        /* error_handler ("ERR CAR: ARG ISN'T A PAIR"); */
        return car_error;
    }
}
/* возвращает cdr ячейки */
val* cdr (val* cell) {
    if ( (!atom_predicate ( cell ) ) &&
         (!null_predicate ( cell ) ) ) {
        return cell->uni_val.cell_val->cdr;
    } else {
        /* error_handler ("ERR CDR: ARG ISN'T A PAIR"); */
        return cdr_error;
    }
}

val* set_car ( val* list, val* new_value ) {
    /* val* car_list = car( list ); */

    list->uni_val.cell_val->car = new_value;
    return list;
}

val* set_cdr ( val* list, val* new_value ) {
    /* val* car_list = car( list ); */
    list->uni_val.cell_val->cdr = new_value;
    return list;
}

/* todo: додеделать */
void error_handler(char* str) {
    printf("\n %s \n", str);
    fflush(stdout);
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
        int result = length_rec( cell );
        /* printf("length arg: "); */
        /* ipprint( cell ); */
        /* printf("\n"); */
        /* printf("length result: %d\n", result); */
        return result;
    }
    error_handler( "ERR LENGTH: ARG ISN'T A PAIR");
}

/* возвращает последнюю пару списка */
val* last_pair (val* cell) {
    if ( cell->type_num == TYPE_NIL ) {
        /* printf( "\nERR LAST_PAIR: EMPTY CELL %d ", cell->type_num); */
        /* fflush(stdout); */
        return last_pair_error;
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
        /* error_handler( "APPEND ERR: ARGS SCHOULD BE LISTS"); */
        return append_error;
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
        char* char_value = malloc(sizeof(char[max_symbol_name_length]));
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
            /* error_handler("ERR REVERSE: WRONG TYPE OF LIST CELL"); */
            return reverse_rec_error;
        }
    }
}

/* возвращает инверсированную копию списка */
val* reverse(val* cell) {
    if ( pair_predicate( cell )) {
        return reverse_rec( cell, nil_constructor() );
    }
    return reverse_error;
    /* error_handler( "ERR REVERSE: ARG ISN'T A PAIR"); */
}

val* add ( val* args ) {
    int* sum = malloc(sizeof(int));
    *sum = 0;

    val* add_rec( val* args ) {
        if ( null_predicate( args ) ) {
            return int_val_constructor( sum );
        }
        val* cur_arg = car( args );
        if ( number_predicate( cur_arg ) ) {
            int the_sum = *sum;
            int term = *cur_arg->uni_val.int_val;
            the_sum += term;
            *sum = the_sum;
            add_rec( cdr( args ) );
        } else {
            return add_error;
        }
    }
    return add_rec( args );
}

val* mul ( val* args ) {
    int* product = malloc(sizeof(int));
    *product = 1;

    val* mul_rec( val* args ) {
        if ( null_predicate( args ) ) {
            return int_val_constructor( product );
        }
        val* cur_arg = car( args );
        if ( number_predicate( cur_arg ) ) {
            int the_product = *product;
            int term = *cur_arg->uni_val.int_val;
            the_product *= term;
            *product = the_product;
            mul_rec( cdr( args ) );

        } else {
            return mul_error;
        }
    }
    return mul_rec( args );
}

val* sub_rec( val* args, int* diff ) {
    if ( null_predicate( args ) ) {
        return int_val_constructor( diff );

    } else {
        val* cur_arg = car( args );
        if ( number_predicate( cur_arg ) ) {
            int the_diff = *diff;
            int term = *cur_arg->uni_val.int_val;
            the_diff -= term;
            *diff = the_diff;
            sub_rec( cdr( args ), diff );

        } else {
            return sub_rec_error;
        }
    }
}

val* sub( val* args ) {
    /* если аргументов нет, выдать ошибку */
    if ( null_predicate( args ) ) {
        return sub_error1;
    } else {
        int* diff = malloc(sizeof(int));
        *diff = 0;

        val* cur_arg = car( args );
        if ( length( args ) == 1 ) {
            /* аргумент один */
            if ( number_predicate( cur_arg ) ) {
                int term = *cur_arg->uni_val.int_val;
                diff -= term;
                return int_val_constructor( diff );

            } else {
                return sub_rec_error;
            }
        }
        val* first_elt = car( args );
        *diff = *first_elt->uni_val.int_val;
        return sub_rec( cdr( args ), diff );
    }
}

val* division_rec( val* args, int* quotient ) {
    char * string;
    if ( null_predicate( args ) ) {
        return int_val_constructor( quotient );

    } else {
        val* cur_arg = car( args );
        if ( number_predicate( cur_arg ) ) {
            int the_quotient = *quotient;
            int term = *cur_arg->uni_val.int_val;

            if ( term == 0 ) {
                string = malloc( sizeof( char[max_symbol_name_length] ) );
                return division_rec_error1;

            } else if ( term > the_quotient ){
                string = malloc( sizeof( char[max_symbol_name_length] ) );
                return division_rec_error2;

            } else {
                the_quotient /= term;
                *quotient = the_quotient;
                division_rec( cdr( args ), quotient );
            }
        } else {
            return division_error1;
        }
    }
}

val* division( val* args ) {
    char* string;
    /* если аргументов нет или аргумент 1, выдать ошибку */
    if ( null_predicate( args ) || (length( args ) == 1) ) {
        string = malloc( sizeof( char[max_symbol_name_length] ) );
        return division_error2;
    } else {
        int* quotient = malloc(sizeof(int));
        val* first_elt = car( args );

        if ( number_predicate( first_elt ) ) {
                *quotient = *first_elt->uni_val.int_val;
                return division_rec( cdr( args ), quotient );

        } else {
            return division_error1;
        }
    }
}

/* принимает функцию и список аргумнтов
/* применяет функцию к каждому аргументу и возвращает список результатов */
/* в отличие от map из схемы принимает только 1 список аргументов и функцию, принимающую
только 1 аргумент */
val* map(val* (*op)(val*),  val* arg_list) {
    if ( ( atom_predicate( arg_list ) ) ||
        ( dotpair_predicate( arg_list ) ) ) {
        return map_error;
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
                 ( *key->uni_val.int_val == *car_cur_arg->uni_val.int_val ) ) {
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
            return assoc_error;
        }
    } else {
        return assoc_error;
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

int false_predicate(val* cell) {
    if( TYPE_NIL == cell->type_num ) {
        return 1;
    }
    return 0;
}

int true_predicate(val* cell) {
    if( ( !false_predicate( cell ) ) &&
        ( TYPE_ERROR != cell->type_num ) ) {
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
         ( TYPE_NIL != cell->type_num ) &&
         ( TYPE_ERROR != cell->type_num ) ) {
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

int equal_numbers_predicate( val* n1, val* n2 ) {
    if ( ( number_predicate( n1 ) ) &&
         ( number_predicate( n2 ) ) ) {
        int num1 = *n1->uni_val.int_val;
        int num2 = *n2->uni_val.int_val;

        if ( num1 == num2 ) {
            return 1;
        }
        return 0;
    }
    return 0;
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
void wrap_brackets_if_not_atom_or_empty_cell (val* param) {
    if ( ( atom_predicate( param ) ) ||
         ( null_predicate( param ) ) ) {
        pprint( param );
    } else {

        printf("(");
        pprint( param );
        printf(")");
        fflush(stdout);
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
    char tmp_char[max_symbol_name_length];
    val* car_pnt;
    val* cdr_pnt;
    switch ( param->type_num ) {

    case TYPE_INT:
        tmp_int = *param->uni_val.int_val;
        printf( "%d", tmp_int );
        return;

    case TYPE_SYMBOL:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), max_symbol_name_length );
        printf( "%s", tmp_char );
        return;

    case TYPE_STRING:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), max_symbol_name_length );
        printf( "'%s'", tmp_char );
        return;

    case TYPE_NIL:
        printf( "()" );
        return;

    case TYPE_ERROR:
        strncpy( tmp_char, ((char*)(param->uni_val.char_val)), max_symbol_name_length );
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
            if ( dotpair_predicate( car_pnt ) || pair_predicate( car_pnt ) ) {
                wrap_brackets_if_not_atom_or_empty_cell( car_pnt );
                printf(" ");
                pprint( cdr_pnt );

            } else {
            pprint( car_pnt );
            printf(" . ");
            pprint( cdr_pnt );
            }

        } else if ( pair_predicate( param ) ) {
            /* это список (cdr = CELL) */
            /* printf("пара\n"); */
            if ( atom_predicate( car_pnt ) ) {
                /* printf("атом\n"); */
                pprint( car_pnt );

            } else {
                /* printf("car ячейки - не атом\n"); */
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
        fflush(stdout);
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
/* void test_add_sub_mu_division(){ */
/*     int* ptr_a = malloc(sizeof(int)); */
/*     *ptr_a = 4; */
/*     val* a_val = int_val_constructor( ptr_a ); */

/*     char* ptr_b = malloc(sizeof(char[1])); */
/*     strncpy( ptr_b, "b", 1 ); */
/*     val* b_val = string_val_constructor( ptr_b ); */

/*     int* ptr_c = malloc(sizeof(int)); */
/*     *ptr_c = 5; */
/*     val* c_val = int_val_constructor( ptr_c ); */

/*     val* d_val = nil_constructor(); */

/*     int* ptr_f = malloc(sizeof(int)); */
/*     *ptr_f = 6; */
/*     val* f_val = int_val_constructor( ptr_f ); */

/*     val* result; */

/*     printf("sub tests: \n"); */
/*     result = sub(4, a_val, f_val, f_val, c_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     result = sub(1, f_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = sub(0); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = sub(1, b_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = sub(4, a_val, f_val, d_val, c_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     printf("add tests: \n"); */
/*     result = add(5, a_val, f_val, f_val, c_val, f_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     result = add(1, f_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     result = add(0); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = add(4, a_val, f_val, d_val, c_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     printf("mul tests: \n"); */
/*     result = mul(5, a_val, f_val, f_val, c_val, f_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     result = mul(1, f_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     result = mul(0); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = mul(4, a_val, f_val, d_val, c_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     printf("division tests: \n"); */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = division(5, a_val, f_val, f_val, c_val, f_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = division(1, f_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     result = division(4, mul(5, a_val, f_val, f_val, c_val, f_val), a_val, a_val, f_val); */
/*     ipprint(  result ); */
/*     printf("\n"); */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = division(0); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */

/*     /\* проверка срабатывания обработчика ошибки *\/ */
/*     /\* result = division(1, b_val); *\/ */
/*     /\* ipprint(  result ); *\/ */
/*     /\* printf("\n"); *\/ */
/* } */

void test_fn( val* list, val* value ) {
    set_car(list, value);
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

    val* test = make_list( 3, a_val, b_val, c_val );
    printf("test: ");
    ipprint( test );
    printf("\n");

    val* test2 = test;
    printf("test2: ");
    ipprint( test2 );
    printf("\n");

    val* test3 = make_list(3, test, a_val, b_val );
    printf("test3: ");
    ipprint( test3 );
    printf("\n");

    val* test4 = car( test3 );
    test_fn( test4, f_val );
    /* set_car( test4, f_val ); */

    printf("test: ");
    ipprint( test );
    printf("\n");

    printf("test2: ");
    ipprint( test2 );
    printf("\n");

    printf("test3: ");
    ipprint( test3 );
    printf("\n");

}



/* void test_set_car_and_set_cdr () { */
/*     int* ptr_a = malloc(sizeof(int)); */
/*     *ptr_a = 4; */
/*     val* a_val = int_val_constructor( ptr_a ); */

/*     char* ptr_b = malloc(sizeof(char[1])); */
/*     strncpy( ptr_b, "b", 1 ); */
/*     val* b_val = symbol_val_constructor( ptr_b ); */

/*     int* ptr_c = malloc(sizeof(int)); */
/*     *ptr_c = 5; */
/*     val* c_val = int_val_constructor( ptr_c ); */

/*     val* d_val = nil_constructor(); */

/*     int* ptr_f = malloc(sizeof(int)); */
/*     *ptr_f = 6; */
/*     val* f_val = int_val_constructor( ptr_f ); */


/*     /\* соберем вложенный список (4 ((4 5))) *\/ */
/*     val* bazo = cons( a_val, cons (cons ( cons ( a_val, */
/*                                                  cons( c_val, d_val )), */
/*                                           d_val), */
/*                                    d_val)); */

/*     /\* теперь соберем список (4 b 5) *\/ */
/*     val* foobar = cons( a_val, cons ( b_val, cons( c_val, d_val ))); */

/*     /\* соберем вложенный список (4 (4 5) 5) *\/ */
/*     val* baz = cons( a_val, cons ( cons( a_val, */
/*                                          cons( c_val, d_val )), */
/*                                    cons( c_val, d_val ))); */
/*     /\* собираем список (6) *\/ */
/*     val* bar = cons( f_val, nil_constructor()); */

/*     /\* (6 (4 5) 5)*\/ */
/*     set_car(baz, f_val); */
/*     ipprint( baz ); */
/*     printf("\n"); */

/*     /\* ((6) b 5) *\/ */
/*     set_car(foobar, bar); */
/*     ipprint( foobar ); */
/*     printf("\n"); */

/*     /\* (4 6) *\/ */
/*     set_cdr(bazo, bar); */
/*     ipprint( bazo ); */
/*     printf("\n"); */

/*     /\* восстанавливаем заничение - иначе получаются циклические списки*\/ */
/*     bazo = cons( a_val, cons (cons ( cons ( a_val, */
/*                                             cons( c_val, d_val )), */
/*                                      d_val), */
/*                               d_val)); */
/*     set_cdr(bar, bazo); */
/*     ipprint( bar ); */
/*     printf("\n"); */

/*     bazo = cons( a_val, cons (cons ( cons ( a_val, */
/*                                             cons( c_val, d_val )), */
/*                                      d_val), */
/*                               d_val)); */
/*     set_car( bazo, b_val ); */
/*     ipprint( bazo ); */
/*     printf("\n"); */

/*     set_car( bazo, c_val ); */
/*     ipprint( bazo ); */
/*     printf("\n"); */

/*     set_car( bazo, nil_constructor() ); */
/*     ipprint( bazo ); */
/*     printf("\n"); */

/* } */

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

    val* test_fn1( val* arg ) {
        add( make_list( 2, arg, f_val ) );
    }

    val* test_fn2(val* arg) {
        sub( make_list( 2, arg, a_val ) );
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
