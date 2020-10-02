#include "env.h"

/* сборка gcc syntax.c test.c environment.c -o proga */
int opening_parenthesis_predicate(char symbol);

int closing_parenthesis_predicate(char symbol);

char** read_input(int max_input_size, int max_str_size);

val* transform_rec(val* param_list, int parenthesis_cnt, val* retval_list);

val* quote_exp_rec ( val* param_list, int parenthesis_cnt );
val* quote_exp_fn (val* param_list);

/* типы возможных состояний */
enum list_of_analyse_state_types {
    TYPE_UNDONE_EXP,
    TYPE_UNDONE_STRING,
    TYPE_ANALYSE_EXP,
    TYPE_ANALYSE_STRING,
    TYPE_ANALYSE_CUR_INPUT_STRING,
    TYPE_ERROR_EXP,
    TYPE_ANALYSE_CUR_INPUT_STRING_DONE,
    TYPE_ANALYSE_INPUT_DONE,
};

int brackets_cnt = 0;
int double_quotes_cnt = 0;

/* анализирует текущую строку ввода  */
int analyse_cur_string( int state, char* string) {
    int i = 0;

    while(1) {
        switch( state ) {

            /* просто читаем символы */
        case TYPE_ANALYSE_CUR_INPUT_STRING:

            switch ( string[i] ) {
            case 0:
                /* printf("конец ввода\n"); */
                if ( brackets_cnt != 0 ) {
                    state = TYPE_ANALYSE_EXP;

                }else if( double_quotes_cnt != 0 ) {
                    state = TYPE_ANALYSE_EXP;

                } else {
                    return TYPE_ANALYSE_CUR_INPUT_STRING_DONE;
                }
                break;

                /* если двойная кавычка - переходим в состояние чтения строки */
            case '"':
                /* printf("analyse_cur_input_str кавычки\n"); */
                /* fflush(stdout); */

                state = TYPE_ANALYSE_STRING;
                i += 1;
                double_quotes_cnt += 1;

                /* printf("double_quotes_cnt %d\n", double_quotes_cnt); */
                /* fflush(stdout); */
                break;

                /* если какая-то из скобок - переходим в состояние чтения выражения */
            case '(':
                /* printf("analyse_cur_input_str  open: brackets %d\n",
                   brackets_cnt); */
                /* fflush(stdout); */

                brackets_cnt += 1;
                i += 1;
                state = TYPE_ANALYSE_EXP;
                break;

            case ')':
                /* printf("analyse_cur_input_str  close: brackets %d\n",
                   brackets_cnt); */
                /* fflush(stdout); */

                brackets_cnt -= 1;
                i += 1;
                state = TYPE_ANALYSE_EXP;
                break;

                /* иначе пропускаем символ */
            default:
                /* printf("прочий символ\n"); */
                i += 1;
            }

            break;

            /* проверям кавычки */
        case TYPE_ANALYSE_STRING:
            switch ( string[i] ) {

                /* текущая строка ввода кончилась,
                   а закрывающих кавычек так и не было  */
            case 0:
                if ( double_quotes_cnt > 0 ) {
                    return TYPE_UNDONE_STRING;

                } else {
                    state = TYPE_ANALYSE_CUR_INPUT_STRING;
                }
                break;

                /* встретили закрывающую кавычку - вернуться в состояние
                   разбора символов */
            case '"':
                /* printf("TYPE_ANALYSE_STRING: double_quotes_cnt %d\n",
                   double_quotes_cnt); */
                fflush(stdout);
                double_quotes_cnt -= 1;
                state = TYPE_ANALYSE_CUR_INPUT_STRING;
                i += 1;
                break;

            default:
                i += 1;
            }
            break;

            /* проверяем скобки */
        case TYPE_ANALYSE_EXP:

            switch ( string[i] ) {
                /* строка кончилась */
            case 0:

                /* есть незакрытая скобка */
                if ( brackets_cnt > 0 ) {
                    return TYPE_UNDONE_EXP;

                    /* закрывающих скобок больше, чем открывающих  */
                } else if ( brackets_cnt < 0 ) {
                    return TYPE_ERROR_EXP;

                    /* открывающих и закрывающих скобок поровну */
                } else {
                    return TYPE_ANALYSE_CUR_INPUT_STRING_DONE;
                }
                break;

                /* встретили открывающую скобку - увеличили счетчик */
            case '(' :
                /* printf("analyse exp open: brackets %d\n", brackets_cnt); */
                brackets_cnt += 1;
                i += 1;
                break;

                /* встретили закрывающую скобку - уменьшили счетчик */
            case ')' :
                /* printf("analyse exp close: brackets %d\n", brackets_cnt); */
                brackets_cnt -= 1;

                /* открывающих и закрывающих скобок поровну */
                if ( brackets_cnt == 0) {
                    state = TYPE_ANALYSE_CUR_INPUT_STRING;
                    i += 1;

                } else {
                    i += 1;
                }
                break;

            case '"':
                double_quotes_cnt += 1;
                state = TYPE_ANALYSE_STRING;
                i += 1;
                break;

            default:
                i += 1;
            }
        }
    }
    return state;
}

/* анализирует весь ввод построчно */
int analyse_input( int state, char* array_strings[] ) {
    int i = 0;

    char* cur_string;
    while(1) {
        /* printf("i %d\n", i); */
        /* printf("state %d\n", state); */

        switch( state ) {

        case TYPE_ANALYSE_CUR_INPUT_STRING:
            cur_string = array_strings[i];

            if ( cur_string == NULL) {
                return TYPE_ANALYSE_CUR_INPUT_STRING_DONE;
            } else {
                state = analyse_cur_string( TYPE_ANALYSE_CUR_INPUT_STRING, cur_string );
                i += 1;
            }
            break;

        case TYPE_UNDONE_STRING:
            /* printf("строка не завершена\n"); */
            cur_string = array_strings[i];
            if ( cur_string == NULL) {
                return TYPE_UNDONE_STRING;
            } else {
                state = analyse_cur_string( TYPE_ANALYSE_STRING, cur_string );
                i += 1;
            }
            break;

        case TYPE_UNDONE_EXP:
            /* printf("выражение не завершено\n"); */
            /* printf("brackets %d\n", brackets_cnt); */
            /* printf("\n"); */
            cur_string = array_strings[i];
            if ( cur_string == NULL) {
                return TYPE_UNDONE_EXP;
            } else {
                state = analyse_cur_string( TYPE_ANALYSE_EXP, cur_string );
                i += 1;
            }
            break;

        case TYPE_ERROR_EXP:
            return state;
            break;

        case TYPE_ANALYSE_CUR_INPUT_STRING_DONE:
            /* printf("обработка строки завершена\n"); */
            cur_string = array_strings[i];
            if ( cur_string == NULL ) {
                return TYPE_ANALYSE_INPUT_DONE;
            } else {
                state = analyse_cur_string( TYPE_ANALYSE_EXP, cur_string );
                i += 1;
            }
        }
    }
    return state;
}

char** read_input(int max_input_size, int max_str_size) {
    char*  string = malloc(sizeof(char) * max_str_size);
    char** array_strings = malloc(sizeof(char*) * max_input_size);;

    /* printf("read_input\n"); */
    /* fflush(stdout); */
    for( int i = 0; i < max_input_size; i++) {
        array_strings[i] = NULL;
    }

    int i = 0;
    int state = TYPE_ANALYSE_CUR_INPUT_STRING;

    while( (fgets(string, max_str_size, stdin) != NULL) ||
           ( i == max_input_size ) ) {
        array_strings[i] = string;

        brackets_cnt = 0;
        double_quotes_cnt = 0;

        /* printf("новая строка\n"); */
        state = analyse_input( TYPE_ANALYSE_CUR_INPUT_STRING, array_strings );

        switch ( state ) {

        case TYPE_ANALYSE_INPUT_DONE:
            return array_strings;
            break;

        case TYPE_ERROR_EXP:
            printf("ERROR: syntax error\n");
            return NULL;

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
    /* printf("copy_str_in_memory: '%s' \n", cur_symbol); */
    /* fflush(stdout); */
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
        /* printf("трансформирую строку\n"); */
        copy_str = copy_str_in_memory( cur_symbol );
        /* printf("строка %s\n", copy_str); */
        new_struct = string_val_constructor( copy_str );

        return new_struct;


    } else {
        /* printf("трансформация прочего символа\n"); */
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

val* flush_array_symbol ( char cur_symbol_array[], int string_flag,
                          int cur_symbol_max_size, int *array_cnt ) {
    val* new_struct = malloc(sizeof(val));

    new_struct = transform_symbol_string_to_val_struct( cur_symbol_array,
                                                        string_flag );

    clear_array( cur_symbol_array, cur_symbol_max_size );
    *array_cnt = 0;
    return cons( new_struct, nil_constructor() );
}

val* add_token_to_list( char cur_symbol_array[], int string_flag,
                        int  cur_symbol_max_size, val* list, int* array_cnt ) {
    val* new_pair = flush_array_symbol( cur_symbol_array,
                                        string_flag, cur_symbol_max_size,
                                        array_cnt);
    list = append( list, new_pair );
    return list;
}

enum list_of_parse_state_types {
    PARSE_CUR_INPUT_STRING,
    PARSE_EXP,
    PARSE_STRING,
    PARSE_QUOTE,
    PARSE_CUR_INPUT_STRING_DONE,
};

val* parse_string(int state, char* string, char cur_symbol[], int* array_cnt) {
    val* cur_str_list = nil_constructor();
    val* new_pair;

    int cur_symbol_cnt = *(int*)malloc(sizeof(int));
    cur_symbol_cnt = *array_cnt;
    int cur_symbol_max_size = 100;
    int i = 0;

    while(1) {

        switch (state) {

        case PARSE_CUR_INPUT_STRING_DONE:
            if ( cur_symbol_cnt != 0 && double_quotes_cnt == 0 ) {
                cur_str_list = add_token_to_list(cur_symbol, 0, cur_symbol_max_size,
                                                 cur_str_list,
                                                 &cur_symbol_cnt);
            }
            *array_cnt = cur_symbol_cnt;
            return cur_str_list;
            break;

        case PARSE_CUR_INPUT_STRING:
            /* printf("PARSE_CUR_INPUT_STRING\n"); */
            switch ( string[i] ) {

            case 0:
                state = PARSE_CUR_INPUT_STRING_DONE;
                break;

            case '(':
                state = PARSE_EXP;
                break;

            case ')':
                state = PARSE_EXP;
                break;

            case'"':
                /* printf("строка!\n"); */
                state = PARSE_STRING;
                break;

            case' ':
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);
                }
                i++;
                break;

            case'\n':
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);
                }
                i++;
                break;

            case 39:
                state = PARSE_QUOTE;
                break;

            default:
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_symbol_cnt++;
                i++;
            }
            break;

        case PARSE_STRING:

            /* printf("PARSE_STRING\n"); */
            switch ( string[i] ) {
            case 0:
                /* printf("смена состояния\n"); */
                state = PARSE_CUR_INPUT_STRING_DONE;
                break;

            case '"':
                /* printf("double_quotes_cnt %d\n", double_quotes_cnt); */
                if (double_quotes_cnt > 0) {
                    double_quotes_cnt--;

                    if ( double_quotes_cnt == 0 ) {
                        /* printf("cur_symbol %s\n", cur_symbol); */
                        cur_str_list = add_token_to_list(cur_symbol, 1,
                                                         cur_symbol_max_size,
                                                         cur_str_list,
                                                         &cur_symbol_cnt);

                        state = PARSE_CUR_INPUT_STRING;
                        i++;
                    }
                } else {
                    double_quotes_cnt++;
                    i++;
                }
                break;

            default:
                /* printf( "прочий символ строки %d \n", string[i] ); */
                fflush(stdout);
                cur_symbol[cur_symbol_cnt] = string[i];
                /* printf("cur_symbol %s\n", cur_symbol); */
                cur_symbol_cnt++;
                i++;
            }
            break;

        case PARSE_EXP:
            switch ( string[i] ) {
                /* printf("PARSE_EXP\n"); */
            case 0:
                state = PARSE_CUR_INPUT_STRING_DONE;
                break;

            case '(':
                /* printf("открывающая скобка"); */
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);

                }
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_str_list = add_token_to_list(cur_symbol, 0,
                                                 cur_symbol_max_size,
                                                 cur_str_list,
                                                 &cur_symbol_cnt);

                i++;
                break;

            case ')':
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);

                }
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_str_list = add_token_to_list(cur_symbol, 0,
                                                 cur_symbol_max_size,
                                                 cur_str_list,
                                                 &cur_symbol_cnt);

                i++;
                break;

            case '"':
                /* printf("строка!\n"); */
                state = PARSE_STRING;
                break;

            case ' ':
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);

                }
                i++;
                break;

            case 39:
                state = PARSE_QUOTE;
                break;

            case'\n':
                /* printf("перевод строки\n"); */

                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);
                }
                i++;
                break;

            default:
                /* printf("прочий символ\n"); */
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_symbol_cnt++;
                i++;
            }
            break;

        case PARSE_QUOTE:
            /* printf("PARSE_QUOTE\n"); */
            switch ( string[i] ) {

            case 0:
                state = PARSE_CUR_INPUT_STRING_DONE;
                break;

            case 39:
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);
                }
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_str_list = add_token_to_list(cur_symbol, 0,
                                                 cur_symbol_max_size,
                                                 cur_str_list,
                                                 &cur_symbol_cnt);
                i++;

            case '(':
                state = PARSE_EXP;
                break;

            case ')':

                state = PARSE_EXP;
                break;

            case'"':
                state = PARSE_STRING;
                break;

            case' ':
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);
                }
                i++;
                break;

            case'\n':
                if ( cur_symbol_cnt != 0 ) {
                    cur_str_list = add_token_to_list(cur_symbol, 0,
                                                     cur_symbol_max_size,
                                                     cur_str_list,
                                                     &cur_symbol_cnt);
                }
                i++;
                break;

            default:
                cur_symbol[cur_symbol_cnt] = string[i];
                cur_symbol_cnt++;
                i++;
            }
            break;
        }
    }
}

val* parse_input(char** array_strings, int max_size_input, int max_size_str) {

    val* retval_list = nil_constructor();
    val* list;
    int cur_symbol_max_size = 100;
    int buf_symbol_cnt = *(int*)malloc(sizeof(int));
    buf_symbol_cnt = 0;
    char buf_symbols[cur_symbol_max_size];
    clear_array( buf_symbols, cur_symbol_max_size );


    double_quotes_cnt = 0;

    /* парисм строку за строкой, пока они не кончатся */
    for (int i = 0; ; i++) {
        if (array_strings[i] != NULL) {

            if ( double_quotes_cnt > 0 ) {
                list = parse_string (PARSE_STRING, array_strings[i], buf_symbols,
                                     &buf_symbol_cnt);
                /* printf("buf_symbols %s\n", buf_symbols); */
                retval_list = append(retval_list, list);
            } else {
                clear_array( buf_symbols, cur_symbol_max_size );
                buf_symbol_cnt = 0;

                list = parse_string (PARSE_CUR_INPUT_STRING,
                                     array_strings[i], buf_symbols,
                                     &buf_symbol_cnt);
                retval_list = append(retval_list, list);
            }

            /* ipprint( list ); */
            /* printf("\n"); */
        } else {
            return retval_list;
        }
    }
}

val* find_quote_exp( val* param_list, val* retval_list, int parenthesis_cnt) {
    while( parenthesis_cnt != 0 ) {

        /* printf("find_quote_exp retval_list \n"); */
        /* ipprint( retval_list ); */
        /* printf("\n"); */
        if ( number_predicate( car( param_list ) ) ) {
            retval_list = append( retval_list, cons( car( param_list ),
                                                     nil_constructor()));
            param_list = cdr( param_list );

        }  else if ( ( symbol_predicate( car( param_list ) ) ) ||
                     ( string_predicate( car( param_list ) ) ) ) {
            val* symbol = car( param_list );
            char* string = symbol->uni_val.char_val;

            switch( string[0] ) {

            case '(':
                parenthesis_cnt++;
                retval_list = append( retval_list, cons( symbol,
                                                         nil_constructor()));
                param_list = cdr( param_list );
                break;

            case ')':
                parenthesis_cnt--;
                retval_list = append( retval_list, cons( symbol,
                                                         nil_constructor()));
                param_list = cdr( param_list );
                break;
            default:
                retval_list = append( retval_list, cons( car( param_list ),
                                                         nil_constructor()));
                param_list = cdr( param_list );
            }
        }
    }
    return cons( retval_list, param_list );
}

val* quote_exp_fn (val* param_list) {
    char *quote = malloc( sizeof( char[5] ) );
    strncpy( quote, "quote", 5 );
    val* quote_val = symbol_val_constructor( quote );
    val* quote_cell = cons(quote_val, nil_constructor());
    val* next_elt = car( param_list );
    val* quote_sub_list;
    val* quote_exp;

    /* printf("quote_exp_fn param_list\n"); */
    /* ipprint( param_list ); */
    /* printf("\n"); */

    /* printf("quote_exp_fn next_elt\n"); */
    /* ipprint( next_elt ); */
    /* printf("\n"); */

    if ( ( number_predicate( next_elt ) ) ||
         ( string_predicate( next_elt ) ) ) {
        quote_sub_list = append( quote_cell, cons( next_elt,
                                                   nil_constructor() ) );
        return cons( quote_sub_list, cdr( param_list ) );

    } else if ( symbol_predicate( next_elt ) )  {

        char* next_elt_str = next_elt->uni_val.char_val;
        /* следующий элемент - это не скобки */

        if ( next_elt_str[0] == '(' && next_elt_str[1] == 0 ) {
            /* printf("список\n"); */
            quote_sub_list = find_quote_exp( cdr( param_list ),
                                             cons(car( param_list ),
                                                  nil_constructor() ),
                                             1);

            /* printf("found quote_exp + param_list\n"); */
            /* ipprint( quote_sub_list ); */
            /* printf("\n"); */

            quote_exp = car( quote_sub_list );
            param_list = cdr( quote_sub_list );

            /* printf("found quote_exp\n"); */
            /* ipprint( quote_exp ); */
            /* printf("\n"); */

            quote_exp = transform_rec( quote_exp, 0, nil_constructor() );

            /* param_list = cdr( quote_exp ); */

            quote_exp = car( quote_exp );

            /* printf("transformed quote_exp\n"); */
            /* ipprint( quote_exp ); */
            /* printf("\n"); */

            quote_exp = append( quote_cell,
                                cons( quote_exp,
                                      nil_constructor() ) );

            /* printf("ready quote_exp\n"); */
            /* ipprint( quote_exp ); */
            /* printf("\n"); */


            /* printf("param_list after tramsform\n"); */
            /* ipprint( param_list ); */
            /* printf("\n"); */

            /* printf("return val\n"); */
            /* ipprint( cons( quote_exp, param_list ) ); */
            /* printf("\n"); */

            return cons( quote_exp, param_list );

        } else if ( next_elt_str[0] == ')' && next_elt_str[1] == 0 ) {

            char *string = malloc( sizeof( char[12] ) );
            strncpy( string, "syntax error", 12 );
            return error_val_constructor( string );

        } else if ( next_elt_str[0] == 39 && next_elt_str[1] == 0 ) {
            quote_sub_list = quote_exp_fn( cdr( param_list ) );

            /* printf("\n"); */
            /* printf("quote_sub_list\n"); */
            /* ipprint( quote_sub_list ); */
            /* printf("\n"); */

            quote_exp = car( quote_sub_list );
            param_list = cdr( quote_sub_list );

            quote_exp = append( quote_cell,
                                cons( quote_exp,
                                      nil_constructor() ) );

            return cons( quote_exp, param_list );

        } else {
            quote_sub_list = append( quote_cell,
                                     cons( next_elt,
                                           nil_constructor() ) );
            /* printf("quote_sub_list \n"); */
            /* ipprint( quote_sub_list ); */
            /* printf("\n"); */

            return cons( quote_sub_list , cdr( param_list ) );

        }
    }
}


val* transform_rec( val* param_list, int parenthesis_cnt, val* retval_list ) {
    val* sub_retval_list;

    /* printf("transform_rec\n"); */
    /* выполнять, пока список не кончился */
    while( !(null_predicate( param_list ) ) ) {


        /* printf("\n"); */
        /* printf("transform_list param_list in cycle\n"); */
        /* ipprint( param_list ); */
        /* printf("\n"); */

        /* printf("transform_list retval_list in cycle\n"); */
        /* ipprint( retval_list ); */
        /* printf("\n"); */

        /* это число */
        if  ( number_predicate( car( param_list ) ) ||
            ( string_predicate( car( param_list ) ) ) ) {
           /* просто записать его в результирующий список */
            retval_list = append( retval_list, cons( car( param_list ),
                                                     nil_constructor()));
            param_list = cdr( param_list );

        } else if ( symbol_predicate( car( param_list ) ) ) {
            val* symbol = car( param_list );
            char* string = symbol->uni_val.char_val;

            switch( string[0] ) {

            case '(':
                parenthesis_cnt++;
                param_list = cdr( param_list );
                /* рекурсивно обойти элементы, сформировав из них подсписок */
                sub_retval_list = transform_rec( param_list,
                                                 parenthesis_cnt,
                                                 nil_constructor() );

                if ( error_predicate( sub_retval_list ) ) {
                    return cons( sub_retval_list, nil_constructor() );
                }
                /* вытащить остаток исходного списка,
                   который предстоит преобразовать */
                param_list = cdr( sub_retval_list );
                sub_retval_list = car( sub_retval_list );
                retval_list = append( retval_list, cons( sub_retval_list,
                                                         nil_constructor()) );
                break;

            case ')':
                parenthesis_cnt--;

                if ( parenthesis_cnt < 0 ) {
                    char *string = malloc( sizeof( char[12] ) );
                    strncpy( string, "syntax error", 12 );
                    retval_list = error_val_constructor( string );
                    return retval_list;
                }

                return cons( retval_list, cdr( param_list ) );

            case 39:
                sub_retval_list = quote_exp_fn( cdr( param_list ) );

                if( error_predicate ( sub_retval_list ) ) {
                    return sub_retval_list;
                }
                param_list = cdr( sub_retval_list );

                /* printf("param_list \n"); */
                /* ipprint( param_list ); */
                /* printf("\n"); */

                sub_retval_list = car( sub_retval_list );

                /* printf("sub_retval_list \n"); */
                /* ipprint( sub_retval_list ); */
                /* printf("\n"); */


                retval_list = append( retval_list, cons( sub_retval_list,
                                                         nil_constructor()) );

                /* printf("retval_list \n"); */
                /* ipprint( retval_list ); */
                /* printf("\n"); */
                break;

            default:
                retval_list = append( retval_list, cons( symbol, nil_constructor()));
                param_list = cdr( param_list );
            }
        }
    }

    return retval_list;
}

val* push (val* elt, val* list) {
    return cons( elt, list);
}

val* transform_list( val* param_list ) {
    /* return_flag = 0; */
    if ( ( length( param_list ) == 1 ) &&
         ( atom_predicate(car (param_list) ) ) ) {
        /* printf("одинарный списк\n"); */
        return car( param_list );

    } else {
        val* list = transform_rec( param_list, 0, nil_constructor());
        return car( list );

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

    int max_input_size = 10000;
    int max_str_size = 1000;
    val* list;

    char** array = read_input( max_input_size, max_str_size );

    if ( array!= NULL ) {
        list = parse_input( array, max_input_size, max_str_size);

        printf("Вывод parse_input: ");
        ipprint( list );

        list = transform_list( list );

        printf("\n");
        printf("Вывод: ");
        ipprint( list );
        printf("\n");
        printf("\n");
    }
}


int main(void) {
    /* test_depth_list(); */
    /* test_copy_num_or_symbol_to_new_cell(); */
    test_parse_input();
    return 0;
}
