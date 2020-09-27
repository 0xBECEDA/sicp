#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Этот заголовочный файл содердит в себе прототипы примитивов и предикатов:
   - cons, car, cdr, set_car, set_cdr, reverse, append, length, last-pair, make_list, map,
   add, sub, mul, division, assoc
   - pair_predicate, atom_predicate, dotpair_predicate, null_predicate, number_predicate,
   symbol_predicate, true_predicate, false_predicate*/

/* возможные типы значений */
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


val* last_pair (val* cell);

val* reverse(val* cell);

void error_handler(char* str);

val* int_val_constructor ( int* val );

/* val* char_val_constructor ( char* val ); */

val* symbol_val_constructor ( char* val );

val* string_val_constructor ( char* val );

val* nil_constructor();

val* cell_val_constructor ( cell* val );

val* error_val_constructor ( char* val );

val* cons  (val* car, val* cdr );

val* car (val* cell);

val* cdr (val* cell);

val* set_car (val* cell, val* new_value );

val* set_cdr (val* cell, val* new_value );

void error_handler(char* str);

int length_rec (val* cell);

int length (val* cell);

val* last_pair (val* cell);

val* append (val* cell1, val* cell2);

val* reverse_rec(val* list, val* new_cell );

val* reverse(val* cell);

val* add(int n, ...);

val* mul(int n, ...);

val* sub(int n, ...);

val* division(int n, ...);

val* map(val* (*op)(val*),  val* arg_list);

val* assoc(val* key, val* args_list);

val* make_list_rec (int n, va_list ptr);

val* make_list ( int n, ...);

int false_predicate(val* cell);

int true_predicate(val* cell);

int number_predicate(val* cell);

int symbol_predicate(val* cell);

int atom_predicate (val* cell);

int null_predicate (val* cell);

int pair_predicate (val* cell);

int dotpair_predicate (val* cell);

int error_predicate (val* cell);

int string_predicate(val* cell);

void wrap_brackets_if_not_atom_or_empty_cell (val* car);

void ipprint (val* param);

void pprint(val* param);

#endif
