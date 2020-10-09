#ifndef SYNTAX_H
#define SYNTAX_H

#include "primitives.h"

char** read_input(int max_input_size, int max_str_size);

val* parse_input(char** array_strings, int max_size_input, int max_size_str);

val* copy_num_or_symbol_to_new_cell (val* elt);

val* transform_list(val* list);

val* init_syntax_errors();
#endif
