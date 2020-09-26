#include "env.h"

/* сборка gcc syntax.c test.c environment.c -o proga */
int opening_parenthesis_predicate(char symbol);

int closing_parenthesis_predicate(char symbol);

char** read_input(int max_input_size, int max_str_size);

val* transform_rec(val* param_list, int parenthesis_cnt, val* retval_list);

int check_brackets(char* array_strings[]) {
    int opening_parenthesis_cnt = 0;
    int closing_parenthesis_cnt = 0;

    int check_brackets_in_a_string(char* string) {
        for(int i = 0; ; i++) {
            if ( string[i] == 0 ) {
                return 1;
            }else if( closing_parenthesis_predicate(string[i]) ) {
                closing_parenthesis_cnt++;

            }else if ( opening_parenthesis_predicate(string[i]) ) {
                opening_parenthesis_cnt++;
            }
        }
    }

    for(int i = 0; ; i++) {
        if ( array_strings[i] == NULL) {
            if( opening_parenthesis_cnt == closing_parenthesis_cnt ) {
                return 1;
            }else {
                return 0;
            }
        } else {
            check_brackets_in_a_string( array_strings[i] );
        }
    }
}

char** read_input(int max_input_size, int max_str_size) {
    char*  string = malloc(sizeof(char) * max_str_size);
    char** array_strings = malloc(sizeof(char*) * max_input_size);;

    for( int i = 0; i < max_input_size; i++) {
        array_strings[i] = NULL;
    }

    int i = 0;
    int j = 0;

    while( (fgets(string, max_str_size, stdin) != NULL) || i == max_input_size ) {
        array_strings[i] = string;

        if (check_brackets ( array_strings ) ) {
            return array_strings;

        } else {
            string = malloc(sizeof(char) * 1000);
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

val* transform_symbol_string_to_val_struct(char cur_symbol[]) {
    val* new_struct;

    if ( num_str_predicate( cur_symbol ) ) {
        int* num = malloc(sizeof(int));
        *num = transform_str_to_num( (char*)cur_symbol );
        new_struct = int_val_constructor( num );
        return new_struct;

    } else {
        char* copy_str = copy_str_in_memory( cur_symbol );
        new_struct = char_val_constructor( copy_str );
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
    /* счетчик  */
    int cur_symbol_cnt = 0;
    char cur_symbol [cur_symbol_max_size];
    val* new_struct;
    val* retval_list = nil_constructor();

    /* париснг текущей строки */
    val* parse_string( char* string) {

        val* cur_str_list = nil_constructor();

        for(int i = 0; ; i++) {

            switch (string[i]) {
                /* строка кончилась */
            case 0:
                if (cur_symbol_cnt != 0) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));
                }
                return cur_str_list;
                /* встретили открывающую скобку */
            case '(':
                /* printf("открывающая скобка\n"); */
                /* если что-то есть в буфере знаков строки - преобразовать */
                if( cur_symbol_cnt != 0) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */
                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                }

                cur_symbol[cur_symbol_cnt] = string[i];

                new_struct = malloc(sizeof(val));
                new_struct = transform_symbol_string_to_val_struct( cur_symbol );

                cur_str_list = append(cur_str_list,
                                      cons( new_struct, nil_constructor()));

                /* ipprint( cur_str_list ); */
                /* printf("\n"); */

                cur_symbol_cnt = 0;
                clear_array(cur_symbol, cur_symbol_max_size);

                break;
                /* встретили закрывающую скобку */
            case ')':
                /* printf("закрывающая скобка\n"); */
                if( cur_symbol_cnt != 0) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol );

                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                }
                cur_symbol[cur_symbol_cnt] = string[i];

                new_struct = malloc(sizeof(val));
                new_struct = transform_symbol_string_to_val_struct( cur_symbol );

                cur_str_list = append(cur_str_list,
                                      cons( new_struct, nil_constructor()));

                /* ipprint( cur_str_list ); */
                /* printf("\n"); */

                cur_symbol_cnt = 0;
                clear_array(cur_symbol, cur_symbol_max_size);


                break;
                /* встретили пробел */
            case ' ':
                /* printf("пробел\n"); */
                if( cur_symbol_cnt != 0) {
                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol );
                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);
                }
                break;
                /* встретили перевод строки */
            case'\n':
                /* printf("перевод строки\n"); */
                if( cur_symbol_cnt != 0) {

                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol );
                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);
                }
                break;
                /* встретили одинарную кавычку ' */
            case 39:
                /* printf("кавычка\n"); */
                if( cur_symbol_cnt != 0 ) {
                    new_struct = malloc(sizeof(val));
                    new_struct = transform_symbol_string_to_val_struct( cur_symbol );
                    cur_str_list = append(cur_str_list,
                                          cons( new_struct, nil_constructor()));

                    /* ipprint( cur_str_list ); */
                    /* printf("\n"); */

                    cur_symbol_cnt = 0;
                    clear_array(cur_symbol, cur_symbol_max_size);

                }
                cur_symbol[cur_symbol_cnt] = string[i];

                new_struct = malloc(sizeof(val));
                new_struct = transform_symbol_string_to_val_struct( cur_symbol );

                cur_str_list = append(cur_str_list,
                                      cons( new_struct, nil_constructor()));

                /* ipprint( cur_str_list ); */
                /* printf("\n"); */

                cur_symbol_cnt = 0;
                clear_array(cur_symbol, cur_symbol_max_size);
                break;

            default:
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

val* copy_num_or_symbol_to_new_cell (val* elt) {
    val* new_struct_car = malloc(sizeof(val));
    val *new_struct_cdr = malloc(sizeof(val));;
    val* new_cell;
    char* string = malloc(sizeof(char[100]));
    int* num = malloc(sizeof(int));

    if ( symbol_predicate ( elt ) ) {
        strncpy( string, elt->uni_val.char_val, 100 );
        new_struct_car = char_val_constructor( string  );
        new_struct_cdr = nil_constructor();
        new_cell = cons( new_struct_car, new_struct_cdr );
        return new_cell;

    } else if ( number_predicate ( elt ) ){
        *num = *elt->uni_val.int_val;
        new_struct_car = int_val_constructor( num );
        new_struct_cdr = nil_constructor();
        new_cell = cons( new_struct_car, new_struct_cdr );
        return new_cell;

    } else {
        error_handler("COPY_NUM_OR_SYMBOL_TO_NEW_CELL ERR: VALUE ISN'T NUM OR SYMBOL");
    }
}

void test_copy_num_or_symbol_to_new_cell() {
    int* ptr_a = malloc(sizeof(int));
    *ptr_a = 4;
    val* a_val = int_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = char_val_constructor( ptr_b );

    char* ptr_c = malloc(sizeof(char[4]));
    strncpy( ptr_c, "book", 4 );
    val* c_val = char_val_constructor( ptr_c );

    val* d_val = nil_constructor();

    int* ptr_f = malloc(sizeof(int));
    *ptr_f = 6;
    val* f_val = int_val_constructor( ptr_f );

    val* test;

    test = copy_num_or_symbol_to_new_cell(f_val);
    free( f_val );
    ipprint(test);
    printf("\n");

    test = copy_num_or_symbol_to_new_cell(b_val);
    free( b_val );
    ipprint(test);
    printf("\n");

    test = copy_num_or_symbol_to_new_cell(c_val);
    free( c_val );
    ipprint(test);
    printf("\n");

}

val* push (val* elt, val* list) {
    return cons( elt, list);
}

int quote_predicate( val* exp ) {
    val* elt = car( exp );
    char* quote = "quote";
    if ( symbol_predicate( elt ) ) {
        char* str = elt->uni_val.char_val;
        if(strcmp( quote, str ) == 0 ) {
            return 1;
        }
    }
    return 0;
}

val* transform_rec( val* param_list, int parenthesis_cnt, val* retval_list) {
    /* выполнять, пока список не кончился */
    while( !(null_predicate( param_list ) ) ) {

        if ( number_predicate( car( param_list ) )) {
            retval_list = append( retval_list, cons( car( param_list ),
                                                     nil_constructor()));
            param_list = cdr( param_list );

        }  else if ( symbol_predicate( car( param_list ) ) ) {
            val* symbol = car( param_list );
            char* string = symbol->uni_val.char_val;

            switch(string[0]) {

            case '(':

                parenthesis_cnt++;
                param_list = cdr(  param_list );
                val* sub_retval_list = transform_rec( param_list,
                                                      parenthesis_cnt,
                                                      nil_constructor() );
                if ( error_predicate( sub_retval_list ) ) {
                    return cons( sub_retval_list, nil_constructor());

                } else {

                    param_list = cdr( sub_retval_list );
                    sub_retval_list = car( sub_retval_list );

                    retval_list = append( retval_list, cons( sub_retval_list,
                                                             nil_constructor()) );
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
                    if ( null_predicate( cdr( param_list ) ) ) {
                        return retval_list;
                    } else {
                        return cons( retval_list, cdr( param_list ) );
                    }
                }
                break;

            case 39:
                param_list = cdr( param_list );

                char *quote = malloc( sizeof( char[5] ) );
                strncpy( quote, "quote", 5 );
                val* quote_val = char_val_constructor( quote );
                val* quote_cell = cons(quote_val, nil_constructor());
                val* next_elt = car( param_list );
                val* quote_sub_list;
                val* quote_exp;
                val* quoted_quote_exp;
                val* full_exp;

                if ( number_predicate( next_elt ) ) {

                    quote_sub_list = append( quote_cell, cons( next_elt,
                                                               nil_constructor() ) );
                    if (null_predicate ( cdr( param_list ) ) ) {
                        retval_list = quote_sub_list;
                        return retval_list;
                    }

                    retval_list = append( retval_list, cons( quote_sub_list,
                                                             nil_constructor()) );
                    param_list = cdr( param_list );

                } else if ( symbol_predicate( next_elt ) ) {

                    char* next_str = next_elt->uni_val.char_val;
                    if ( next_str[0] != '(' &&
                         next_str[0] != ')' ) {
                        quote_sub_list = append( quote_cell,
                                                 cons( next_elt,
                                                       nil_constructor() ) );
                        /* printf("quote_sub_list: "); */
                        /* ipprint( quote_sub_list ); */
                        /* printf("\n"); */

                        if (null_predicate ( cdr( param_list ) ) ) {
                            retval_list = quote_sub_list;
                            return retval_list;
                        }
                        retval_list = append( retval_list, cons( quote_sub_list,
                                                                 nil_constructor() ) );
                        param_list = cdr( param_list );

                    } else {
                        quote_sub_list = transform_rec( param_list, 0,
                                                        nil_constructor() );
                        /* printf("quote_sub_list: "); */
                        /* ipprint( quote_sub_list ); */
                        /* printf("\n"); */

                        if ( error_predicate( quote_sub_list ) ) {
                            /* printf("error\n"); */
                            return quote_sub_list;

                        } else if( atom_predicate( car( quote_sub_list ) ) ) {
                            quote_exp = quote_sub_list;
                            /* printf("atom!\n"); */
                            /* printf("quote_sub_list: "); */
                            /* ipprint( quote_sub_list ); */
                            /* printf("\n"); */

                            quoted_quote_exp = append(quote_cell,
                                                      cons
                                                      ( quote_exp,
                                                        nil_constructor() ) );
                            /* printf("quoted_quote_exp: "); */
                            /* ipprint( quoted_quote_exp ); */
                            /* printf("\n"); */

                            retval_list = append( retval_list, quoted_quote_exp);
                            return retval_list;

                        } else {
                            quote_exp = car( quote_sub_list );

                            /* printf("quote_exp: "); */
                            /* ipprint( quote_exp ); */
                            /* printf("\n"); */

                            quoted_quote_exp = append(quote_cell,
                                                      cons
                                                      ( quote_exp,
                                                        nil_constructor() ) );
                            /* printf("quoted_quote_exp: "); */
                            /* ipprint( quoted_quote_exp ); */
                            /* printf("\n"); */
                            full_exp = append( cons( quoted_quote_exp,
                                                     nil_constructor() ),
                                               cdr( quote_sub_list ) );

                            /* printf("full_exp: "); */
                            /* ipprint( full_exp ); */
                            /* printf("\n"); */

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
    val* list = transform_rec( param_list, 0, nil_constructor());
    return list;
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
    val* b_val = char_val_constructor( ptr_b );

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


    list = transform_list( list );
    printf("\n");
    ipprint( list );
    printf("\n");

}


int main(void) {
    /* test_depth_list(); */
    /* test_copy_num_or_symbol_to_new_cell(); */
    test_parse_input();
    return 0;
}
