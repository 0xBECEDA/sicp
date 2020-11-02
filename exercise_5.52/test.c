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

val* atom_predicate_compile (val* cell);

val* dotpair_predicate_compile (val* cell);

val* pair_predicate_compile (val* cell);

val* null_predicate_compile (val* cell);

val* number_predicate_compile (val* cell);

val* true_predicate_compile (val* cell);

val* false_predicate_compile (val* cell);

val* symbol_predicate_compile (val* cell);

val* equal_numbers_predicate_compile ( val* n1, val* n2 );

val* ttrue;
val* ffalse;

/* объясления ошибок */
val* car_error;
val* cdr_error;
val* last_pair_error;
val* append_error;
val* reverse_rec_error;
val* length_error;
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
val* bigger_predicate_error;
val* smaller_predicate_error;

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
    strncpy( string, "ERR LENGTH: arg ist't a pair", max_symbol_name_length );
    length_error = error_val_constructor( string );

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

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR > : bigger_predicate needs only numbers",
             max_symbol_name_length );

    bigger_predicate_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR < : smaller_predicate needs only numbers",
             max_symbol_name_length );

    smaller_predicate_error = error_val_constructor( string );
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
        return result;
    }
    return -1;
}

val* length_compile (val* cell) {
    int* retval = malloc(sizeof(int));

    if ( pair_predicate( cell )) {
        *retval = length_rec( cell );
        return int_val_constructor( retval );
    }
    *retval = -1;
    return int_val_constructor( retval );
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

int eq_names_predicate ( char* name1, char* name2 ) {
    if(strcmp( name1, name2 ) == 0 ) {
        return 1;
    }
    return 0;
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
}

int smaller_predicate( val* args ) {
    int result = 0;
    int prev_number;
    val* first_elt;

    int smaller_predicate_rec( val* args ) {
        if ( null_predicate( args ) ) {
            return result;

        } else {
            first_elt = car( args );
            if ( number_predicate( first_elt ) ) {
                int cur_number = *first_elt->uni_val.int_val;
                /* printf("smaller_predicate %d\n"); */
                if ( prev_number < cur_number ) {
                    result = 1;
                    prev_number = cur_number;
                    smaller_predicate_rec( cdr( args ) );

                } else {
                    result = 0;
                    prev_number = cur_number;
                    smaller_predicate_rec( cdr( args ) );
                }
            } else {
                return -1;
            }
        }
    }

    if ( number_predicate( car( args ) ) ) {
        first_elt = car( args );
        prev_number = *first_elt->uni_val.int_val;
        args = cdr(args);
        result = 1;
        return smaller_predicate_rec( args );
    }

    return -1;
}

val* smaller_predicate_compile( val* args ) {
    val* result;
    int prev_number;
    val* first_elt;

    val* smaller_predicate_rec( val* args ) {
        if ( null_predicate( args ) ) {
            return result;
        } else {
            first_elt = car( args );
            if ( number_predicate( first_elt ) ) {
                int cur_number = *first_elt->uni_val.int_val;
                /* printf("smaller_predicate %d\n"); */
                if ( prev_number < cur_number ) {
                    result = ttrue;
                    prev_number = cur_number;
                    smaller_predicate_rec( cdr( args ) );

                } else {
                    result = ffalse;
                    prev_number = cur_number;
                    smaller_predicate_rec( cdr( args ) );
                }
            } else {
                result = smaller_predicate_error;
            }
        }
    }

    if ( number_predicate( car( args ) ) ) {
        first_elt = car( args );
        prev_number = *first_elt->uni_val.int_val;
        args = cdr(args);
        result = ttrue;
        return smaller_predicate_rec( args );
    }
    return smaller_predicate_error;
}

int bigger_predicate( val* args ) {
    int result = 0;
    int prev_number;
    val* first_elt;

    int bigger_predicate_rec( val* args ) {
        /* printf("bigger_predicate_rec\n"); */
        if ( null_predicate( args ) ) {
            /* printf("null_predicate\n"); */
            fflush(stdout);
            return result;

        } else {
            first_elt = car( args );
            if ( number_predicate( first_elt ) ) {
                int cur_number = *first_elt->uni_val.int_val;

                if ( prev_number > cur_number ) {
                    result = 1;
                    prev_number = cur_number;
                    bigger_predicate_rec( cdr( args ) );

                } else {
                    result = 0;
                    prev_number = cur_number;
                    bigger_predicate_rec( cdr( args ) );
                }
            } else {
                return -1;
            }
        }
    }

    if ( number_predicate( car( args ) ) ) {
        /* printf("здесь\n"); */
        first_elt = car( args );
        prev_number = *first_elt->uni_val.int_val;
        args = cdr(args);
        result = 1;
        return bigger_predicate_rec( args );
    }

    return -1;
}

val* bigger_predicate_compile( val* args ) {
    val* result;
    int prev_number;
    val* first_elt;

    val* bigger_predicate_rec( val* args ) {
        if ( null_predicate( args ) ) {
            return result;
        } else {
            first_elt = car( args );
            if ( number_predicate( first_elt ) ) {
                int cur_number = *first_elt->uni_val.int_val;
                if ( prev_number > cur_number ) {
                    result = ttrue;
                    prev_number = cur_number;
                    bigger_predicate_rec( cdr( args ) );

                } else {
                    result = ffalse;
                    prev_number = cur_number;
                    bigger_predicate_rec( cdr( args ) );
                }
            } else {
                result = bigger_predicate_error;
            }
        }
    }
    if ( number_predicate( car( args ) ) ) {
        first_elt = car( args );
        prev_number = *first_elt->uni_val.int_val;
        args = cdr(args);
        result = ttrue;
        return bigger_predicate_rec( args );
    }
    return bigger_predicate_error;
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
                *diff = *diff - term;
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
    if ( null_predicate( args ) || ( length( args ) == 1 ) ) {
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

/* val* false_predicate_compile(val* cell) { */
/*     int* retval = malloc(sizeof(int)); */
/*     if( TYPE_NIL == cell->type_num ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */

/* } */

val* false_predicate_compile(val* cell) {
    if( TYPE_NIL == cell->type_num ) {
        return ttrue;
    }
    return ffalse;
}

int true_predicate(val* cell) {
    if( ( !false_predicate( cell ) ) &&
        ( TYPE_ERROR != cell->type_num ) ) {
        return 1;
    }
    return 0;
}

/* val* true_predicate_compile(val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if( ( !false_predicate( cell ) ) && */
/*         ( TYPE_ERROR != cell->type_num ) ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* true_predicate_compile(val* cell) {
    if( ( !false_predicate( cell ) ) &&
        ( TYPE_ERROR != cell->type_num ) ) {
        return ttrue;
    }
    return ffalse;
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

/* val* number_predicate_compile(val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if ( ( TYPE_SYMBOL != cell->type_num ) && */
/*          ( TYPE_STRING != cell->type_num ) && */
/*          ( TYPE_CELL != cell->type_num ) && */
/*          ( TYPE_NIL != cell->type_num ) && */
/*          ( TYPE_ERROR != cell->type_num ) ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* number_predicate_compile(val* cell) {
    if ( ( TYPE_SYMBOL != cell->type_num ) &&
         ( TYPE_STRING != cell->type_num ) &&
         ( TYPE_CELL != cell->type_num ) &&
         ( TYPE_NIL != cell->type_num ) &&
         ( TYPE_ERROR != cell->type_num ) ) {
        return ttrue;
    }
    return ffalse;
}

int symbol_predicate(val* cell) {
    if ( TYPE_SYMBOL == cell->type_num ) {
        return 1;
    }
    return 0;
}

/* val* symbol_predicate_compile(val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if ( TYPE_SYMBOL == cell->type_num ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*      } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* symbol_predicate_compile(val* cell) {
    if ( TYPE_SYMBOL == cell->type_num ) {
        return ttrue;
    }
    return ffalse;
}

int string_predicate(val* cell) {
    if ( TYPE_STRING == cell->type_num ) {
        return 1;
    }
    return 0;
}

/* val* string_predicate_compile(val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if ( TYPE_STRING == cell->type_num ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* string_predicate_compile(val* cell) {

    if ( TYPE_STRING == cell->type_num ) {
        return ttrue;
    }
    return ffalse;
}

int atom_predicate (val* cell) {
    if ( ( TYPE_STRING == cell->type_num ) ||
         ( TYPE_SYMBOL == cell->type_num ) ||
         ( TYPE_INT == cell->type_num ) ) {
        return 1;
    }
    return 0;
}

/* val* atom_predicate_compile (val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if ( ( TYPE_STRING == cell->type_num ) || */
/*          ( TYPE_SYMBOL == cell->type_num ) || */
/*          ( TYPE_INT == cell->type_num ) ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* atom_predicate_compile (val* cell) {

    if ( ( TYPE_STRING == cell->type_num ) ||
         ( TYPE_SYMBOL == cell->type_num ) ||
         ( TYPE_INT == cell->type_num ) ) {
        return ttrue;
    }
    return ffalse;
}

int null_predicate (val* cell) {
    if  ( TYPE_NIL == cell->type_num ) {
        return 1;
    }
    return 0;
}

/* val* null_predicate_compile (val* cell) { */
/*     int* retval = malloc(sizeof(int)); */
/*     if  ( TYPE_NIL == cell->type_num ) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */

/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* null_predicate_compile (val* cell) {
    if  ( TYPE_NIL == cell->type_num ) {
        return ttrue;
    }
    return ffalse;
}


int pair_predicate (val* cell) {
    if ( ( cell->type_num == TYPE_CELL ) &&
         ( !dotpair_predicate( cell ) )) {
        return 1;

    }else {
        return 0;
    }
}

/* val* pair_predicate_compile (val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if ( ( cell->type_num == TYPE_CELL ) && */
/*          ( !dotpair_predicate( cell ) )) { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */

/*     }else { */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/* } */

val* pair_predicate_compile (val* cell) {
    if ( ( cell->type_num == TYPE_CELL ) &&
         ( !dotpair_predicate( cell ) )) {
        return ttrue;
    }else {
        return ffalse;
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

/* val* equal_numbers_predicate_compile( val* n1, val* n2 ) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if ( ( number_predicate( n1 ) ) && */
/*          ( number_predicate( n2 ) ) ) { */
/*         int num1 = *n1->uni_val.int_val; */
/*         int num2 = *n2->uni_val.int_val; */

/*         if ( num1 == num2 ) { */
/*             *retval = 1; */
/*             return  int_val_constructor( retval ); */
/*         } */
/*         *retval = 0; */
/*         return  int_val_constructor( retval ); */
/*     } */
/*     *retval = 0; */
/*     return  int_val_constructor( retval ); */
/* } */

val* equal_numbers_predicate_compile( val* n1, val* n2 ) {
    if ( ( number_predicate( n1 ) ) &&
         ( number_predicate( n2 ) ) ) {
        int num1 = *n1->uni_val.int_val;
        int num2 = *n2->uni_val.int_val;

        if ( num1 == num2 ) {
            return ttrue;
        }
        return ffalse;
    }
    return ffalse;
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

/* val* dotpair_predicate_compile (val* cell) { */
/*     int* retval = malloc(sizeof(int)); */

/*     if (cell->type_num == TYPE_CELL) { */
/*         val* cdr_cell = cdr( cell ); */

/*         if( ( cdr_cell->type_num == TYPE_INT ) || */
/*             ( cdr_cell->type_num == TYPE_STRING ) || */
/*             ( cdr_cell->type_num == TYPE_SYMBOL ) ) { */
/*             *retval = 1; */
/*             return  int_val_constructor( retval ); */
/*         } else { */
/*             *retval = 0; */
/*             return  int_val_constructor( retval ); */
/*         } */
/*     } else{ */
/*         *retval = 1; */
/*         return  int_val_constructor( retval ); */
/*     } */
/* } */

val* dotpair_predicate_compile (val* cell) {
    if (cell->type_num == TYPE_CELL) {
        val* cdr_cell = cdr( cell );

        if( ( cdr_cell->type_num == TYPE_INT ) ||
            ( cdr_cell->type_num == TYPE_STRING ) ||
            ( cdr_cell->type_num == TYPE_SYMBOL ) ) {
            return ttrue;
        } else {
            return ffalse;
        }
    } else{
        return ffalse;
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

val* set_true_and_false () {

    char *string;
    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "true",
             max_symbol_name_length );
    ttrue = symbol_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "false",
             max_symbol_name_length );
    ffalse = symbol_val_constructor( string );
}
