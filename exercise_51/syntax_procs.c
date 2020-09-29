#include "syntax.h"

/* сборка gcc syntax_procs.c syntax.c test.c environment.c -o proga */
int var_predicate(val* exp) {
    if ( symbol_predicate( exp ) ) {
        return 1;
    } else {
        return 0;
    }
}

int self_evaluating_predicate(val* exp) {
    if ( number_predicate( exp ) ||
         string_predicate( exp ) ) {
        return 1;
    } else {
        return 0;
    }
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

val* text_of_quotation( val* exp ) {
    return car( cdr ( exp ) );
}

int assigment_predicate(val* exp) {
    char set[6] = "set!";
    if ( ( pair_predicate( exp ) ) &&
         ( symbol_predicate( car( exp ) ) ) ) {
        val* symbol = car( exp );

        if(strcmp( set, symbol->uni_val.char_val ) == 0 ) {
            return 1;
        } else{
            return 0;
        }
    } else {
        return 0;
    }
}

val* assignment_variable( val* exp ) {
    return car( cdr ( exp ) );
}

val* assignment_value(val* exp) {
    return car( cdr (cdr ( exp ) ) );
}

int lambda_predicate(val* exp) {
    char lambda[6] = "lambda";
    if ( ( pair_predicate( exp ) ) &&
         ( symbol_predicate( car( exp ) ) ) ) {
        val* symbol = car( exp );

        if(strcmp( lambda, symbol->uni_val.char_val ) == 0 ) {
            return 1;
        } else{
            return 0;
        }
    } else {
        return 0;
    }
}

val* lambda_params(val* exp) {
    return car( cdr( exp ) );
}

val* lambda_body(val* exp) {
    return cdr( cdr( exp ) );
}

val* make_lambda(val* params, val*body) {
    char* lambda_str = (char*)malloc( sizeof( char[6] ) );
    strncpy( lambda_str, "lambda", 6 );
    val* lambda = symbol_val_constructor( lambda_str );

    return cons( lambda, (cons( params, body ) ) );
}

int let_predicate(val* exp) {
    char let[6] = "let";
    if ( ( pair_predicate( exp ) ) &&
         ( symbol_predicate( car( exp ) ) ) ) {
        val* symbol = car( exp );

        if(strcmp( let, symbol->uni_val.char_val ) == 0 ) {
            return 1;
        } else{
            return 0;
        }
    } else {
        return 0;
    }
}

val* let_vars_bindings(val* exp) {
    return car( cdr( exp ) );
}

val* let_body(val* exp) {
    return cdr( cdr( exp ) );
}

int if_predicate(val* exp) {
    char if_str[6] = "if";
    if ( ( pair_predicate( exp ) ) &&
         ( symbol_predicate( car( exp ) ) ) ) {
        val* symbol = car( exp );

        if(strcmp( if_str, symbol->uni_val.char_val ) == 0 ) {
            return 1;
        } else{
            return 0;
        }
    } else {
        return 0;
    }
}

val* if_condition(val* exp) {
    return car( cdr( exp ) );
}

val* if_consequent(val* exp) {
    return car( cdr( cdr( exp ) ) );
}

val* if_alternative(val* exp) {
    if (null_predicate( cdr( cdr( cdr( exp ) ) ) ) ) {

        char *err_str = malloc( sizeof( char[5] ) );
        strncpy( err_str, "false", 5 );
        return error_val_constructor( err_str );
    } else {
        return car(cdr ( cdr( cdr( exp ) ) ) );
    }
}

val* make_if(val* predicate, val* consequent, val* alternative) {
    char* if_str = (char*)malloc( sizeof( char[2] ) );
    strncpy( if_str, "if", 2 );
    val* if_val = symbol_val_constructor( if_str );

    return make_list(4, if_val, predicate, consequent, alternative);
}

int define_predicate(val* exp) {
    char define[6] = "define";
    if ( ( pair_predicate( exp ) ) &&
         ( symbol_predicate( car( exp ) ) ) ) {
        val* symbol = car( exp );

        if(strcmp( define, symbol->uni_val.char_val ) == 0 ) {
            return 1;
        } else{
            return 0;
        }
    } else {
        return 0;
    }
}

val* definition_var(val* exp) {
    if (symbol_predicate( car( cdr( exp ) ) ) ) {
        return car( cdr( exp ) );

    } else {
        return car( car( cdr( exp ) ) );
    }
}

val* definition_value(val* exp) {
    if ( symbol_predicate( car( cdr( cdr ( exp ) ) ) ) ) {
        return car( cdr( cdr( exp ) ) );

    } else {
        make_lambda( cdr( car( cdr( exp ) ) ),
                     cdr( cdr ( exp ) ) );
    }
}

int begin_predicate(val* exp) {
    char begin[6] = "begin";
    if ( ( pair_predicate( exp ) ) &&
         ( symbol_predicate( car( exp ) ) ) ) {
        val* symbol = car( exp );

        if(strcmp( begin, symbol->uni_val.char_val ) == 0 ) {
            return 1;
        } else{
            return 0;
        }
    } else {
        return 0;
    }
}

val* begin_actions(val *exp) {
    return cdr( exp );
}

int last_exp_predicate(val* seq) {
    if ( null_predicate(cdr( seq) ) ) {
        return 1;
    }
    return 0;
}

val* first_exp(val* seq) {
    return car( seq );
}

val* rest_exps(val* seq) {
    return cdr( seq );
}

val* make_begin(val* seq) {
    char* begin_str = (char*)malloc( sizeof( char[5] ) );
    strncpy( begin_str, "begin", 5 );
    val* begin = symbol_val_constructor( begin_str );
    return cons( begin, seq);
}

val* trasform_seq_to_exp(val* seq) {
    if ( null_predicate(seq) ) {
        return seq;

    }else if ( last_exp_predicate( seq ) ) {
        return first_exp( seq );

    } else {
        return make_begin( seq );
    }
}

int application_predicate(val* exp) {
    return pair_predicate( exp );
}

val* operator(val* exp) {
    return car( exp );
}
val* operands(val* exp) {
    return cdr( exp );
}

int no_operands_predicate(val* ops) {
    return null_predicate( ops );
}
val* firs_operand(val* ops) {
    return car( ops );
}

val* rest_operands(val* ops) {
    return cdr( ops );
}

int driver_loop (int max_input_size, int max_str_size) {

    while(1) {

        printf("\n");
        printf("Ввод: ");
        printf("\n");

        char** array = read_input( max_input_size, max_str_size );
        val*  list = parse_input( array, max_input_size, max_str_size);
        list = transform_list( list );

        printf("\n");
        printf("Результат вычислений: ");
        printf("\n");

        ipprint(list);
        printf("\n");

        /* пока что очищаем память */
        free(array);
        free(list);
    }
}

int main () {

    int max_input_size = 10000;
    int max_str_size = 1000;

    driver_loop(max_input_size, max_str_size);
}
