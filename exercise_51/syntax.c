#include "env.h"

/* сборка gcc syntax.c test.c environment.c -o proga */
int opening_parenthesis_predicate(char symbol);

int closing_parenthesis_predicate(char symbol);

char** read_input(int max_input_size, int max_str_size);

val* transform_rec(val* param_list, int parenthesis_cnt, val* retval_list);

enum list_of_state_types {
    TYPE_UNDONE_EXP,
    TYPE_UNDONE_STRING,
    TYPE_ANALYSE_EXP,
    TYPE_ANALYSE_STRING,
    TYPE_ANALYSE_CUR_INPUT_STRING,
    TYPE_ANALYSE_INPUT_DONE,
};

int brackets_cnt = 0;

int analyse_cur_string( int state, char* string) {
    int i = 0;

    /* printf("analyse_cur_string: строка %s\n", string); */
    while(1) {
        switch( state ) {

            /* просто читаем символы */
        case TYPE_ANALYSE_CUR_INPUT_STRING:
            /* printf("analyse_cur_string: анализирую выражеие\n"); */
            if ( string[i] == 0 ) {
                return state;

            } else if ( string[i] == '"' ) {
                state = TYPE_ANALYSE_STRING;
                i += 1;

            } else if ( string[i] == '(' ) {
                brackets_cnt += 1;
                i += 1;
                state = TYPE_ANALYSE_EXP;

            } else if ( string[i] == ')' ) {
                brackets_cnt -= 1;
                i += 1;
                state = TYPE_ANALYSE_EXP;

            } else {
                i += 1;
            }
            break;

            /* проверям кавычки */
        case TYPE_ANALYSE_STRING:
            /* printf("analyse_cur_string: анализирую строку\n"); */
            /* printf("i %d\n", i); */
            /* printf("analyse_cur_string: char %d\n", string[i]); */
            if ( string[i] == 0 ) {
                state = TYPE_UNDONE_STRING;
                return state;

            } else if ( string[i] == '"' ) {
                state = TYPE_ANALYSE_CUR_INPUT_STRING;
                i += 1;

            } else {
                i += 1;
            }

            break;

            /* проверяем скобки */
        case TYPE_ANALYSE_EXP:
            if ( ( string[i] == 0 ) && ( brackets_cnt != 0) ) {
                return TYPE_UNDONE_EXP;

            } else if ( string[i] == '(' ) {
                brackets_cnt += 1;
                i += 1;

            } else if ( string[i] == ')' ) {
                brackets_cnt -= 1;

                if ( brackets_cnt == 0) {
                    state = TYPE_ANALYSE_CUR_INPUT_STRING;
                    i += 1;
                }
            } else {
                i += 1;
            }
            break;
        }
    }
    return state;
}

int analyse_input( int state, char* array_strings[] ) {
    int i = 0;

    char* cur_string;
    while(1) {
        /* printf("i %d\n", i); */
        /* printf("state %d\n", state); */

        switch( state ) {

        case TYPE_ANALYSE_CUR_INPUT_STRING:
            /* printf("analyse_input: анаизирую текущую строку\n"); */
            cur_string = array_strings[i];

            /* printf("analyse_input: строка %s\n", cur_string); */
            if ( cur_string == NULL) {
                return TYPE_ANALYSE_INPUT_DONE;

            } else {
                state = analyse_cur_string( TYPE_ANALYSE_CUR_INPUT_STRING, cur_string );
                i += 1;
            }
            break;

        case TYPE_UNDONE_STRING:
            cur_string = array_strings[i];
            if ( cur_string == NULL) {
                return TYPE_UNDONE_STRING;
            } else {
                state = analyse_cur_string( TYPE_ANALYSE_STRING, cur_string );
                i += 1;
            }
            break;

        case TYPE_UNDONE_EXP:
            cur_string = array_strings[i];
            if ( cur_string == NULL) {
                return TYPE_UNDONE_EXP;
            } else {
                state = analyse_cur_string( TYPE_ANALYSE_EXP, cur_string );
                i += 1;
            }
            break;
        }
    }
    return state;
}

char** read_input(int max_input_size, int max_str_size) {
    char*  string = malloc(sizeof(char) * max_str_size);
    char** array_strings = malloc(sizeof(char*) * max_input_size);;

    printf("read_input\n");
    for( int i = 0; i < max_input_size; i++) {
        array_strings[i] = NULL;
    }

    int i = 0;
    int state = TYPE_ANALYSE_CUR_INPUT_STRING;

    while( (fgets(string, max_str_size, stdin) != NULL) ||
           ( i == max_input_size ) ) {
        array_strings[i] = string;

        /* printf("new_string %s\n", string); */
        /* printf("new_string %d\n", string[0]); */
        state = analyse_input( state, array_strings );

        switch ( state ) {

        case TYPE_ANALYSE_INPUT_DONE:
            return array_strings;
            break;

        default:
            /* printf("жду новую строку\n"); */
            string = malloc(sizeof(char) * max_str_size);
            i++;
        }
    }
    return array_strings;
}

int transform_str_to_num(char* string) {
    int num = atoi(string);
    return num;
}

int space_predicate(char symbol) {
    if (symbol == ' ') {
        return 1;
    }
    return 0;
}

int opening_parenthesis_predicate(char symbol) {
    if (symbol == '(') {
        return 1;
    }
    return 0;
}

int closing_parenthesis_predicate(char symbol) {
    if (symbol == ')') {
        return 1;
    }
    return 0;
}

/* если строка символа  начинается с числа - мы считаем, что символ,
   который она представляет - это число */
int num_str_predicate(char* symbol_str) {
    if(symbol_str[0] == '0' ||
       symbol_str[0] == '1' ||
       symbol_str[0] == '2' ||
       symbol_str[0] == '3' ||
       symbol_str[0] == '4' ||
       symbol_str[0] == '5' ||
       symbol_str[0] == '6' ||
       symbol_str[0] == '7' ||
       symbol_str[0] == '8' ||
       symbol_str[0] == '9') {
        return 1;
    }
    return 0;
}

char* copy_str_in_memory(char cur_symbol[]) {
    char* str_pnt = malloc(sizeof(char[100]));
    strncpy( str_pnt, cur_symbol, 100 );
    return str_pnt;
}

val* transform_symbol_string_to_val_struct(char cur_symbol[], int string_flag) {
    val* new_struct;
    char* copy_str;
    if ( ( num_str_predicate( cur_symbol ) ) &&
         ( string_flag == 0 ) ) {
        /* printf("трансформация числа\n"); */
        int* num = malloc(sizeof(int));
        *num = transform_str_to_num( (char*)cur_symbol );
        new_struct = int_val_constructor( num );
        return new_struct;

    } else if ( string_flag == 1 ) {
        copy_str = copy_str_in_memory( cur_symbol );
        new_struct = string_val_constructor( copy_str );
        return new_struct;

    } else {
        copy_str = copy_str_in_memory( cur_symbol );
        new_struct = symbol_val_constructor( copy_str );
        return new_struct;
    }
}

char* clear_array(char array[], int max_size) {
    for (int i = 0; i < max_size; i++) {
        array[i] = 0;
    }
    return array;
}

/* почищает ввод от пробелов и переводов строки; */
/* заводит структру под каждое слово, число или иной символ; */
/* возвращает список, составленный из этих структур */
val* parse_input(char** array_strings, int max_size_input, int max_size_str) {

    /* максимальный размер одного слова или числа */
    int cur_symbol_max_size = 100;
    int string_flag = 0;
    /* счетчик  */
    int cur_symbol_cnt = 0;
    char cur_symbol[cur_symbol_max_size];
    val* new_struct;
    val* retval_list = nil_constructor();


    /* париснг текущей строки */
    val* parse_string( char* string) {

        val* cur_str_list = nil_constructor();

        for(int i = 0; ; i++) {

            /* printf("string %d\n", string[i]); */
            switch (string[i]) {
                /* строка кончилась */
            case 0:
                if (cur_symbol_cnt != 0) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));
                }
                return cur_str_list;
                /* встретили двойные кавычки */
            case '"':
                /* printf("двойные кавычки\n"); */

                if ( string_flag == 0 ) {
                    string_flag += 1;

                } else {
                    string_flag--;

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 1 );

                    /* ipprint( new_struct ); */
                    /* printf("\n"); */

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));
                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);
                }
                break;

            case '(':
                /* printf("открывающая скобка\n"); */
                /* если что-то есть в буфере знаков строки - преобразовать */
                if ( ( cur_symbol_cnt != 0 ) && ( string_flag == 0 ) ) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */
                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                    cur_symbol[cur_symbol_cnt] = string[i];

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                } else if ( string_flag == 0 ) {

                    cur_symbol[cur_symbol_cnt] = string[i];

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                } else {
                    cur_symbol[cur_symbol_cnt] = string[i];
                    cur_symbol_cnt++;
                }
                break;
                /* встретили закрывающую скобку */
            case ')':
                /* printf("закрывающая скобка\n"); */
                if ( ( cur_symbol_cnt != 0 ) && ( string_flag == 0 ) ) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                    cur_symbol[cur_symbol_cnt] = string[i];

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                } else if ( string_flag == 0 ) {

                    cur_symbol[cur_symbol_cnt] = string[i];

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                } else {
                    cur_symbol[cur_symbol_cnt] = string[i];
                    cur_symbol_cnt++;
                }
                break;
                /* встретили пробел */
            case ' ':
                /* printf("пробел\n"); */
                if ( ( cur_symbol_cnt != 0 ) && ( string_flag == 0 ) ) {
                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );
                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                } else if ( string_flag == 1 ) {
                    cur_symbol[cur_symbol_cnt] = string[i];
                    cur_symbol_cnt++;
                }

                break;
                /* встретили перевод строки */
            case'\n':
                /* printf("перевод строки\n"); */
                if ( ( cur_symbol_cnt != 0 ) && ( string_flag == 0 ) ) {

                    /* printf("здесь\n"); */
                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );
                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                } else if ( string_flag == 1 ) {
                    cur_symbol[cur_symbol_cnt] = string[i];
                    cur_symbol_cnt++;
                }

                break;
                /* встретили одинарную кавычку ' */
            case 39:
                /* printf("кавычка\n"); */
                if ( ( cur_symbol_cnt != 0 ) && ( string_flag == 0 ) ) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );
                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                } else if ( string_flag == 0 ) {

                cur_symbol[cur_symbol_cnt] = string[i];

                new_struct = malloc(sizeof(val));
                new_struct = transform_symbol_string_to_val_struct( cur_symbol, 0 );

                cur_str_list = append(cur_str_list,
                                      cons( new_struct, nil_constructor()));

                /* ipprint( cur_str_list ); */
                /* printf("\n"); */

                cur_symbol_cnt = 0;
                clear_array(cur_symbol, cur_symbol_max_size);

                } else if ( string_flag == 1 ) {
                    cur_symbol[cur_symbol_cnt] = string[i];
                    cur_symbol_cnt++;
                }
                break;

            default:
                /* printf("%d\n", string[i]); */
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_symbol_cnt++;
            }
        }
    }

    /* парисм строку за строкой, пока они не кончатся */
    for (int i = 0; ; i++) {
        if (array_strings[i] != NULL) {
            val* list = parse_string (array_strings[i]);
            retval_list = append(retval_list, list);
        } else {
            return retval_list;
        }
    }
}

/* val* copy_num_or_symbol_to_new_cell (val* elt) { */
/*     val* new_struct_car = malloc(sizeof(val)); */
/*     val *new_struct_cdr = malloc(sizeof(val));; */
/*     val* new_cell; */
/*     char* string = malloc(sizeof(char[100])); */
/*     int* num = malloc(sizeof(int)); */

/*     if ( symbol_predicate ( elt ) ) { */
/*         strncpy( string, elt->uni_val.char_val, 100 ); */
/*         new_struct_car = symbol_val_constructor( string  ); */
/*         new_struct_cdr = nil_constructor(); */
/*         new_cell = cons( new_struct_car, new_struct_cdr ); */
/*         return new_cell; */

/*     } else if ( number_predicate ( elt ) ){ */
/*         *num = *elt->uni_val.int_val; */
/*         new_struct_car = int_val_constructor( num ); */
/*         new_struct_cdr = nil_constructor(); */
/*         new_cell = cons( new_struct_car, new_struct_cdr ); */
/*         return new_cell; */

/*     } else { */
/*         error_handler("COPY_NUM_OR_SYMBOL_TO_NEW_CELL ERR: VALUE ISN'T NUM OR SYMBOL"); */
/*     } */
/* } */

/* void test_copy_num_or_symbol_to_new_cell() { */
/*     int* ptr_a = malloc(sizeof(int)); */
/*     *ptr_a = 4; */
/*     val* a_val = int_val_constructor( ptr_a ); */

/*     char* ptr_b = malloc(sizeof(char[1])); */
/*     strncpy( ptr_b, "b", 1 ); */
/*     val* b_val = char_val_constructor( ptr_b ); */

/*     char* ptr_c = malloc(sizeof(char[4])); */
/*     strncpy( ptr_c, "book", 4 ); */
/*     val* c_val = char_val_constructor( ptr_c ); */

/*     val* d_val = nil_constructor(); */

/*     int* ptr_f = malloc(sizeof(int)); */
/*     *ptr_f = 6; */
/*     val* f_val = int_val_constructor( ptr_f ); */

/*     val* test; */

/*     test = copy_num_or_symbol_to_new_cell(f_val); */
/*     free( f_val ); */
/*     ipprint(test); */
/*     printf("\n"); */

/*     test = copy_num_or_symbol_to_new_cell(b_val); */
/*     free( b_val ); */
/*     ipprint(test); */
/*     printf("\n"); */

/*     test = copy_num_or_symbol_to_new_cell(c_val); */
/*     free( c_val ); */
/*     ipprint(test); */
/*     printf("\n"); */
/* } */

val* push (val* elt, val* list) {
    return cons( elt, list);
}

int return_flag = 0;

val* transform_rec( val* param_list, int parenthesis_cnt, val* retval_list ) {
    /* выполнять, пока список не кончился */
    while( !(null_predicate( param_list ) ) ) {

        /* это число */
        if ( number_predicate( car( param_list ) ) ) {
            /* просто записать его в результирующий список */
            retval_list = append( retval_list, cons( car( param_list ),
                                                     nil_constructor()));
            param_list = cdr( param_list );

            /* это символ или строка */
        }  else if ( ( symbol_predicate( car( param_list ) ) ) ||
                     ( string_predicate( car( param_list ) ) ) ) {
            val* symbol = car( param_list );
            char* string = symbol->uni_val.char_val;

            switch(string[0]) {

            case '(':
                parenthesis_cnt++;
                param_list = cdr( param_list );
                /* рекурсивно обойти элементы, сформировав из них подсписок */
                val* sub_retval_list = transform_rec( param_list,
                                                      parenthesis_cnt,
                                                      nil_constructor() );

                if ( error_predicate( sub_retval_list ) ) {
                    return sub_retval_list;

                /* список не был обойден до конца */
                } else if ( return_flag == 0 ) {

                    /* вытащить остаток исходного списка,
                       который предстоит преобразовать */
                    param_list = cdr( sub_retval_list );
                    sub_retval_list = car( sub_retval_list );
                    retval_list = append( retval_list, cons( sub_retval_list,
                                                             nil_constructor()) );
                 /* иначе список был обойден целиком */
                } else {
                    /* присоединяем результат обхода и возвращаем
                       весь результирующий список */
                    retval_list = append( retval_list, sub_retval_list );
                    return retval_list;
                }
                break;

            case ')':
                parenthesis_cnt--;

                if (parenthesis_cnt < 0) {
                    char *string = malloc( sizeof( char[12] ) );
                    strncpy( string, "syntax error", 12 );
                    retval_list = error_val_constructor( string );
                    return retval_list;

                } else if ( parenthesis_cnt == 0 ) {
                    /* мы дошли до конца списка */
                    if ( null_predicate( cdr( param_list ) ) ) {
                        return_flag = return_flag + 1;
                        return retval_list;

                        /* иначе присоединить остаток
                           необойденного списка к результату */
                    } else {
                        return cons( retval_list, cdr( param_list ) );
                    }
                }
                break;
                /* встретили одинарную кавычку */
            case 39:
                param_list = cdr( param_list );

                char *quote = malloc( sizeof( char[5] ) );
                strncpy( quote, "quote", 5 );
                val* quote_val = symbol_val_constructor( quote );
                val* quote_cell = cons(quote_val, nil_constructor());
                val* next_elt = car( param_list );
                val* quote_sub_list;
                val* quote_exp;
                val* quoted_quote_exp;
                val* full_exp;

                /* следующий элемент - число */
                if ( number_predicate( next_elt ) ) {

                    quote_sub_list = append( quote_cell, cons( next_elt,
                                                               nil_constructor() ) );
                    /* если дошли до конца списка, возвращаем результат */
                    if (null_predicate ( cdr( param_list ) ) ) {
                        retval_list = quote_sub_list;
                        return retval_list;
                    }
                    /* иначе присоединяем текущий результат к общему
                       и продолжаем обход */
                    retval_list = append( retval_list, cons( quote_sub_list,
                                                             nil_constructor()) );
                    param_list = cdr( param_list );

                    /* следующий элемент - символ или строка */
                } else if ( ( symbol_predicate( next_elt ) ) ||
                            ( string_predicate( next_elt ) ) ) {
                    char* next_str = next_elt->uni_val.char_val;

                    /* следующий элемент - это не скобки */
                    if ( next_str[0] != '(' &&  next_str[0] != ')' ) {
                        quote_sub_list = append( quote_cell,
                                                 cons( next_elt,
                                                       nil_constructor() ) );

                        if (null_predicate ( cdr( param_list ) ) ) {
                            retval_list = quote_sub_list;
                            return retval_list;
                        }
                        retval_list = append( retval_list, cons( quote_sub_list,
                                                                 nil_constructor() ) );
                        param_list = cdr( param_list );

                        /* следующий элемент - какая-то скобка */
                    } else {

                        /* рекурсивно обойти остаток списка */
                        quote_sub_list = transform_rec( param_list, 0,
                                                        nil_constructor() );

                        printf(" quote_sub_list: \n");
                        ipprint(quote_sub_list);
                        printf("\n");

                        if ( error_predicate( quote_sub_list ) ) {
                            return quote_sub_list;

                            /* получили невложенный список - он весь является
                               закавыченным выражением */
                        } else if( atom_predicate( car( quote_sub_list ) ) ) {
                            quote_exp = quote_sub_list;

                            quoted_quote_exp = append(quote_cell,
                                                      cons
                                                      ( quote_exp,
                                                        nil_constructor() ) );

                            retval_list = append( retval_list, quoted_quote_exp);
                            return retval_list;
                            /* получили вложенный список, где car - это закавыченное
                               выражение */
                        } else {
                            quote_exp = car( quote_sub_list );
                            quoted_quote_exp = append(quote_cell,
                                                      cons
                                                      ( quote_exp,
                                                        nil_constructor() ) );
                            full_exp = append( cons( quoted_quote_exp,
                                                     nil_constructor() ),
                                               cdr( quote_sub_list ) );

                            retval_list = append( retval_list, full_exp);
                            return retval_list;
                        }
                    }
                }
                break;

            default:
                retval_list = append( retval_list, cons( symbol, nil_constructor()));
                param_list = cdr( param_list );

            }
        }
    }
    return retval_list;
}

val* transform_list( val* param_list ) {
    return_flag = 0;
    if ( ( length( param_list ) == 1 ) &&
         ( atom_predicate(car (param_list) ) ) ) {
        return car( param_list );

    } else {
    val* list = transform_rec( param_list, 0, nil_constructor());
    return list;

    }
}

/* считает максимальную глубину правильного списка */
int depth_list (val* list) {
    if ( null_predicate( list ) ) {
        return 0;
    } else if (atom_predicate( car ( list) ) ) {
        return 0 + depth_list( cdr( list) );
    } else {
        return 1 + depth_list( car( list) ) + depth_list( cdr( list) );
    }
}

void test_depth_list() {
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

    int depth;
    /* теперь соберем список (4 b 5) */
    val* foobar = make_list(3, a_val, b_val, c_val);

    /* соберем вложенный список (4 ((4 5))) */
    val* bazo = make_list(2, a_val,
                          make_list(1,
                                    make_list(2, a_val, c_val)));
    /* соберем (()) */
    val* bar = make_list(1, make_list(0));

    depth = depth_list( foobar );
    printf("%d\n", depth);
    depth = depth_list( bazo );
    printf("%d\n", depth);
    depth = depth_list( bar );
    printf("%d\n", depth);

}

void test_parse_input() {
    /* (1 2 3 (4 6)) */
    /* (let ((a 7) */
    /*       (b 8)) */
    /*  (+ a b)) */
    /* (1 2 3 (4 6) 5) */
    /* (define (factorial n) */
    /*  (if (= n 0) */
    /*      1 */
    /*      (* n (factorial (- n 1))))) */

    int max_input_size = 10000;
    int max_str_size = 1000;
    char** array = read_input( max_input_size, max_str_size );


    val*  list = parse_input( array, max_input_size, max_str_size);

    printf("\n");
    ipprint(list);
    printf("\n");


    /* list = transform_list( list ); */
    /* printf("\n"); */
    /* ipprint( list ); */
    /* printf("\n"); */

    /* printf("\n"); */
    /* ipprint( cdr( list ) ); */
    /* printf("\n"); */

}


int main(void) {
    /* test_depth_list(); */
    /* test_copy_num_or_symbol_to_new_cell(); */
    test_parse_input();
    return 0;
}
