#include "syntax_procs.h"

/* сборка gcc eval.c syntax_procs.c syntax.c test.c environment.c -o proga */
val* primitives_bindings;
val* primitives_names;
val* global_environment;

val* ttrue;
val* ffalse;
val* ok;

val* eval ( val* exp, val * env );

val* setup_env() {
    add_frame(primitives_bindings, make_empty_environment());
}

val* bind_proc_name_and_proc_fn ( char *proc_name, char *proc_fn_name ) {
    char* name = malloc(sizeof(char[100]));
    char* fn_name = malloc(sizeof(char[100]));

    strncpy( name, proc_name, 100 );
    strncpy( fn_name, proc_fn_name, 100 );

    val* proc_name_struct = symbol_val_constructor( proc_name );
    val* proc_fn_name_struct = symbol_val_constructor( fn_name );

    return cons( proc_name_struct, proc_fn_name_struct );
    /* return make_list( 2, proc_name_struct, proc_fn_name_struct ); */
}

val* primitives_procedures_names() {
    make_list(24,
              bind_proc_name_and_proc_fn( "car", "primitive" ),
              bind_proc_name_and_proc_fn( "cdr", "primitive" ),
              bind_proc_name_and_proc_fn( "cons", "primitive" ),
              bind_proc_name_and_proc_fn( "make_list", "primitive" ),
              bind_proc_name_and_proc_fn( "set_car", "primitive" ),
              bind_proc_name_and_proc_fn( "set_cdr", "primitive" ),
              bind_proc_name_and_proc_fn( "add", "primitive" ),
              bind_proc_name_and_proc_fn( "sub", "primitive" ),
              bind_proc_name_and_proc_fn( "mul", "primitive" ),
              bind_proc_name_and_proc_fn( "division", "primitive" ),
              bind_proc_name_and_proc_fn( "reverse", "primitive" ),
              bind_proc_name_and_proc_fn( "append", "primitive" ),
              bind_proc_name_and_proc_fn( "map", "primitive" ),
              bind_proc_name_and_proc_fn( "assoc", "primitive" ),
              bind_proc_name_and_proc_fn( "length","primitive" ),
              bind_proc_name_and_proc_fn( "last_pair", "primitive" ),
              bind_proc_name_and_proc_fn( "true_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "false_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "null_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "pair_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "symbol_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "atom_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "string_predicate", "primitive" ),
              bind_proc_name_and_proc_fn( "dotpair_predicate", "primitive" )
              );
}

val* primitives_procedures_bindings() {
    make_list(24,
              bind_proc_name_and_proc_fn( "car", "car" ),
              bind_proc_name_and_proc_fn( "cdr", "cdr" ),
              bind_proc_name_and_proc_fn( "cons", "cons" ),
              bind_proc_name_and_proc_fn( "list", "make_list" ),
              bind_proc_name_and_proc_fn( "set-car!", "set_car" ),
              bind_proc_name_and_proc_fn( "set-cdr!", "set_cdr" ),
              bind_proc_name_and_proc_fn( "+", "add" ),
              bind_proc_name_and_proc_fn( "-", "sub" ),
              bind_proc_name_and_proc_fn( "*", "mul" ),
              bind_proc_name_and_proc_fn( "/", "division" ),
              bind_proc_name_and_proc_fn( "reverse", "reverse" ),
              bind_proc_name_and_proc_fn( "append", "append" ),
              bind_proc_name_and_proc_fn( "map", "map" ),
              bind_proc_name_and_proc_fn( "assoc", "assoc" ),
              bind_proc_name_and_proc_fn( "length", "length" ),
              bind_proc_name_and_proc_fn( "last-pair", "last_pair" ),
              bind_proc_name_and_proc_fn( "true?", "true_predicate" ),
              bind_proc_name_and_proc_fn( "false?", "false_predicate" ),
              bind_proc_name_and_proc_fn( "null?", "null_predicate" ),
              bind_proc_name_and_proc_fn( "pair?", "pair_predicate" ),
              bind_proc_name_and_proc_fn( "symbol?", "symbol_predicate" ),
              bind_proc_name_and_proc_fn( "atom?", "atom_predicate" ),
              bind_proc_name_and_proc_fn( "string?", "string_predicate" ),
              bind_proc_name_and_proc_fn( "dotpair?", "dotpair_predicate" )
              );
}

/* val* primitives = primitives_procedures(); */

val* lookup_primitive_implementation_name( val* key, val* primitives ) {
    return ( assoc( key,  primitives ) );
}

val* list_of_values(val* exps, val* env ) {
    /* printf(" list_of_values exps: "); */
    /* ipprint( exps ); */
    /* printf("\n"); */
    /* fflush(stdout); */

    if ( no_operands_predicate( exps ) ) {
        return nil_constructor();
    } else {
        val* first_exp = eval( first_operand( exps ),  env );
        /* printf(" list_of_values first exp evaluated: "); */
        /* ipprint( first_exp ); */
        /* printf("\n"); */
        /* fflush(stdout); */

        return cons( first_exp,
                     list_of_values(rest_operands( exps ), env ) );
    }
}

int eq_names_predicate ( char* name1, char* name2 ) {
    if(strcmp( name1, name2 ) == 0 ) {
        return 1;
    }
    return 0;
}

val* apply_primitive_application( val* proc, val* args ) {

    char* proc_name = proc->uni_val.char_val;
    int result = *(int*)malloc(sizeof(int));;

    /* printf(" apply_primitive_application: "); */
    /* ipprint( args ); */
    /* printf("\n"); */
    /* fflush(stdout); */

    if ( eq_names_predicate( proc_name, "car" ) ) {
        if ( length( args ) == 1 ) {
            return car( car( args ) );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "CAR ERROR: car needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "cdr" ) ) {
        if ( length( args ) == 1 ) {
            return cdr( car( args ) );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "CDR ERROR: cdr needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "make_list" ) ) {
        return args;

    } else if ( eq_names_predicate( proc_name, "cons" ) ) {
        if ( length( args ) == 2 ) {
            return cons( car( args ), car( cdr ( args ) ) );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "CONS ERROR: cons needs exactly 2 args",
                     max_symbol_name_length);
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "set_car" ) ) {
        if ( length ( args ) == 2 ) {
            val* list = car( args );
            val* new_value = car( cdr ( args ) );
            return set_car( list, new_value );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "SET-CAR! ERROR: set-car! needs exactly 2 args",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }
    } else if ( eq_names_predicate( proc_name, "set_cdr" ) ) {
        if ( length ( args ) == 2 ) {
            val* list = car( args );
            val* new_value = car( cdr ( args ) );
            return set_cdr( list, new_value );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "SET-CDR! ERROR: set-cdr! needs exactly 2 args",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "add" ) ) {
        return add( args );

    } else if ( eq_names_predicate( proc_name, "sub" ) ) {
        return sub( args );

    } else if ( eq_names_predicate( proc_name, "mul" ) ) {
        return mul( args );

    } else if ( eq_names_predicate( proc_name, "division" ) ) {
        return division( args );

    } else if ( eq_names_predicate( proc_name, "reverse" ) ) {
        return reverse( car( args ) );

    } else if ( eq_names_predicate( proc_name, "append" ) ) {
        if ( length ( args ) == 2 ) {
            val* list1 = car( args );
            val* list2 = car( cdr ( args ) );
            return append( list1, list2 );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "APPEND ERROR: append needs exactly 2 args",
                     max_symbol_name_length);
            return error_val_constructor( string );
        }
        /* } else if ( eq_names_predicate( proc_name, "map" ) ) { */

    } else if ( eq_names_predicate( proc_name, "assoc" ) ) {
        if ( length ( args ) == 2 ) {
            val* key = car( args );
            val* list = car( cdr ( args ) );
            return assoc( key, list );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "ASSOC ERROR: assoc needs exactly 2 args",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "length" ) ) {
        if ( length ( args ) == 1 ) {
            result = length( car( args ) );
            return int_val_constructor( &result );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "LENGTH ERROR: length needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "last_pair" ) ) {
        if ( length ( args ) == 1 ) {
            return last_pair( car( args ) );

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "LAST-PAIR ERROR: last-pair needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "true_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = true_predicate( car( args ) );

            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string, "TRUE? ERROR: true_predicate needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "false_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = false_predicate( car( args ) );

            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "FALSE? ERROR: false needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "null_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = null_predicate( car( args ) );
            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "NULL? ERROR: null needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "pair_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = pair_predicate( car( args ) );

            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "PAIR? ERROR: pair? needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "symbol_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = symbol_predicate( car( args ) );

            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "SYMBOL? ERROR: symbol? needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "atom_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = atom_predicate( car( args ) );
            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "ATOM? ERROR: atom? needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "string_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            result = string_predicate( car( args ) );
            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "STRING? ERROR: string? needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else if ( eq_names_predicate( proc_name, "dotpair_predicate" ) ) {

        if ( length ( args ) == 1 ) {
            result = dotpair_predicate( car( args ) );

            if ( result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            char *string = malloc( sizeof( char[max_symbol_name_length] ) );
            strncpy( string,
                     "DOTPAIR? ERROR: dotpair? needs exactly 1 arg",
                     max_symbol_name_length );
            return error_val_constructor( string );
        }

    } else {
        printf("eval_primitive_application: Unknown proc\n");
    }
}

/* (define (apply procedure arguments) */
/*  (cond ((primitive-procedure? procedure) */
/*         (apply-primitive-procedure procedure arguments)) */
/*   ((compound-procedure? procedure) */
/*    (eval-sequence */
/*     (procedure-body procedure) */
/*     (extend-environment */
/*      (procedure-parameters procedure) */
/*      arguments */
/*      (procedure-environment procedure)))) */
/*   (else */
/*       (error */
/*        "Неизвестный тип процедуры -- APPLY" procedure)))) */

int primitive_procedure_predicate( val* proc ) {
    if ( !null_predicate( assoc(proc, primitives_names ) ) ) {
        return 1;
    }
    return 0;
}

val* apply( val* proc, val* args ) {
    if ( primitive_procedure_predicate( proc ) ) {
        return apply_primitive_application( proc, args );
        /* } else if ( compound_procedure_predicate( proc ) ) { */

    } else {
        char *string = malloc( sizeof( char[100] ) );
        strncpy( string,
                 "APPLY: unknown type of procedure",
                 100 );
        return error_val_constructor( string );

    }
}

val* eval_assigment( val* exp, val * env ) {

}

/* (define (eval-definition exp env) */
/*  (define-variable! (definition-variable exp) */
/*   (eval (definition-value exp) env) */
/*   env) */
/*  ’ok) */

/* (define (define-variable! var val env) */
/*  (let ((frame (first-frame env))) */
/*   (define (scan vars vals) */
/*    (cond ((null? vars) */
/*           (add-binding-to-frame! var val frame)) */
/*     ((eq? var (car vars)) */
/*      (set-car! vals val)) */
/*     (else (scan (cdr vars) (cdr vals))))) */
/*   (scan (frame-variables frame) */
/*    (frame-values frame)))) */

val* define_variable( val* var, val* value, val* env ) {
    val* frame = first_frame( env );
    frame = add_binding( frame, var, value );

    set_car( env, frame );
    return frame;
}

val* eval_definition( val* exp, val * env ) {
    printf("eval_definition exp\n");
    ipprint( exp );
    printf("\n");

    printf("definition_var\n");
    ipprint( definition_var( exp ) );
    printf("\n");

    printf("definition_value\n");
    ipprint( definition_value( exp ) );
    printf("\n");

    define_variable( definition_var( exp ),
                     eval( definition_value( exp ), env ),
                     env );

    printf(" env after define_variable: \n");
    ipprint( global_environment );
    printf("\n");

    return ok;
}

val* eval_if( val* exp, val * env ) {

}

val* eval_sequence( val* exp, val * env ) {

}

val* make_procedure( val* params, val* body ) {

}

val* eval ( val* exp, val * env ) {
    printf("eval exp\n");
    ipprint( exp );
    printf("\n");
    fflush(stdout);
    if ( self_evaluating_predicate( exp ) ) {
        return exp;

    } else if ( var_predicate( exp ) ) {
        /* printf("var\n"); */
        /* ipprint( exp ); */
        /* printf("\n"); */

        return lookup_variable( exp, env );

    } else if ( quote_predicate( exp ) ) {
        /* printf("quote\n"); */
        /* ipprint( exp ); */
        /* printf("\n"); */
        return text_of_quotation( exp );

    } else if ( assigment_predicate( exp ) ) {
        return eval_assigment ( exp, env );

    } else if ( define_predicate( exp ) ) {
        return eval_definition ( exp, env );

    } else if ( if_predicate( exp ) ) {
        return eval_if ( exp, env );

    } else if ( lambda_predicate( exp ) ) {
        return make_procedure( lambda_params( exp ),
                               lambda_body( exp ) );
    } else if ( begin_predicate( exp ) ) {
        return eval_sequence ( begin_actions( exp ), env );

    } else if ( application_predicate( exp ) ) {
        apply( eval( operator( exp ), env ), list_of_values( operands( exp), env) );

    } else {
        printf("Unknown type of exp\n");
    }
}

int eval_driver_loop( int max_input_size, int max_str_size ) {
    while(1) {

        printf("\n");
        printf("Ввод: ");
        printf("\n");

        char** array = read_input( max_input_size, max_str_size );
        if ( array != NULL ) {
            val*  list = parse_input( array, max_input_size, max_str_size);
            list = transform_list( list );
            /* printf("Результат трансформации выражения: "); */
            /* printf("\n"); */
            /* ipprint( list ); */
            /* printf("\n"); */

            val* result = eval ( list, global_environment );

            printf("\n");
            printf("Результат вычисления выражения: ");
            printf("\n");

            ipprint( result );
            printf("\n");
            printf("\n");

            /* test_predicates ( list ); */
            /* пока что очищаем память */

            free(array);
            free(list);

        }
    }
}

int main () {
    primitives_bindings = primitives_procedures_bindings();
    primitives_names = primitives_procedures_names();

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

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "ok",
             max_symbol_name_length );
    ok = symbol_val_constructor( string );

    global_environment = setup_env();

    /* ipprint( global_environment ); */
    /* printf("\n"); */

    eval_driver_loop( 10000, 1000 );
    return 1;
}
