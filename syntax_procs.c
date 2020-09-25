#include "syntax.h"

int var_predicate(val* exp) {
    if ( symbol_predicate( exp ) ) {
        return 1;
    } else {
        return 0;
    }
}

int string_predicate(val* exp) {
    if ( symbol_predicate( exp ) ) {
        char* value = exp->uni_val.char_val;
        if(value[0] == 39 ) {
            return 1;

        } else {
            return 0;
        }
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

/* val* definition_value(val* exp) { */
/*     if ( symbol_predicate( car( cdr( cdr ( exp ) ) ) ) ) { */
/*         return car( cdr( cdr( exp ) ) ); */

/*     } else { */
/*         make_lambda( cdr( car( cdr( exp ) ) ), */
/*                      cdr( cdr ( exp ) ) ); */
/*     } */
/* } */

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

val* lambda_params(val* exp) {
    return car( cdr( exp ) );
}

val* lambda_body(val* exp) {
    return cdr( cdr( exp ) );
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

int main () {

    int max_input_size = 10000;
    int max_str_size = 1000;
    char** array = read_input( max_input_size, max_str_size );

    val*  list = parse_input( array, max_input_size, max_str_size);
    printf("\n");
    ipprint(list);
    printf("\n");

    list = transform_list( list );

    char* quote = "'";
    if (quote[0] == 39) {
        printf("quote\n");
    }

    printf("\n");
    ipprint(list);
    printf("\n");

    int result = lambda_predicate( list );
    printf("result %d\n", result);
}
