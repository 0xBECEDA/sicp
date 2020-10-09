#include "syntax_procs.h"

/* сборка gcc eval.c syntax_procs.c syntax.c test.c environment.c -o proga */
val* primitives_objects;
val* primitives_names;
val* global_environment;

val* ttrue;
val* ffalse;
val* ok;

val* car_args_error;
val* cdr_args_error;
val* cons_args_error;
val* set_car_args_error;
val* set_cdr_args_error;
val* equal_numbers_predicate_error;
val* append_args_error;
val* assoc_args_error;
val* length_args_error;
val* last_pair_args_error;
val* true_predicate_args_error;
val* false_predicate_args_error;
val* null_predicate_args_error;
val* pair_predicate_args_error;
val* symbol_predicate_args_error;
val* atom_predicate_args_error;
val* string_predicate_args_error;
val* dotpair_predicate_args_error;
val* eval_primitive_procs_error;
val* set_variable_value_error;
val* eval_error;
val* apply_error;

val* eval ( val* exp, val * env );

val* eval_sequence( val* exps, val * env );

val* init_eval_errors() {
    char * string;
    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "CAR ERROR: car needs exactly 1 arg",
             max_symbol_name_length );
    car_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "CDR ERROR: cdr needs exactly 1 arg",
             max_symbol_name_length );
    cdr_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "CONS ERROR: cons needs exactly 2 args",
             max_symbol_name_length);
    cons_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "SET-CAR! ERROR: set-car! needs exactly 2 args",
             max_symbol_name_length );
    set_car_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "SET-CDR! ERROR: set-cdr! needs exactly 2 args",
             max_symbol_name_length );
    set_cdr_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "EQUALITY ERROR: '=' needs exactly 2 args",
             max_symbol_name_length );
    equal_numbers_predicate_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "APPEND ERROR: append needs exactly 2 args",
             max_symbol_name_length);
    append_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ASSOC ERROR: assoc needs exactly 2 args",
             max_symbol_name_length );
    assoc_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "LENGTH ERROR: length needs exactly 1 arg",
             max_symbol_name_length );
    length_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "LAST-PAIR ERROR: last-pair needs exactly 1 arg",
             max_symbol_name_length );
    last_pair_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "TRUE? ERROR: true_predicate needs exactly 1 arg",
             max_symbol_name_length );
    true_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "FALSE? ERROR: false needs exactly 1 arg",
             max_symbol_name_length );
    false_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "NULL? ERROR: null needs exactly 1 arg",
             max_symbol_name_length );
    null_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "PAIR? ERROR: pair? needs exactly 1 arg",
             max_symbol_name_length );
    pair_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "SYMBOL? ERROR: symbol? needs exactly 1 arg",
             max_symbol_name_length );
    symbol_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "ATOM? ERROR: atom? needs exactly 1 arg",
             max_symbol_name_length );
    atom_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "STRING? ERROR: string? needs exactly 1 arg",
             max_symbol_name_length );
    string_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "DOTPAIR? ERROR: dotpair? needs exactly 1 arg",
             max_symbol_name_length );
    dotpair_predicate_args_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "EVAL_PRIMITIVE_APPLICATION ERROR: unknown proc",
             max_symbol_name_length );
    eval_primitive_procs_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string,
             "APPLY: unknown type of procedure",
             max_symbol_name_length );
    apply_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR SET_VARIABLE_VALUE: unussigned variable",
             max_symbol_name_length );
    set_variable_value_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR EVAL: Unknown type of exp",
             max_symbol_name_length );
    eval_error = error_val_constructor( string );
    return ok;
}
val* setup_env() {
    global_environment = extend_environment( primitives_names,
                                             primitives_objects,
                                             make_empty_environment() );
    return ok;
}

val* make_primitives_procedure_object ( char *proc_name ) {
    char* name = malloc(sizeof(char[100]));
    char* primitive_str = malloc(sizeof(char[100]));

    strncpy( name, proc_name, 100 );
    strncpy( primitive_str, "primitive", 100 );

    val* proc_name_struct = symbol_val_constructor( proc_name );
    val* primitive = symbol_val_constructor( primitive_str );

    return make_list( 2, primitive,  proc_name_struct );
}

val* make_primitives_procedure_name ( char *proc_name ) {
    char* name = malloc(sizeof(char[100]));

    strncpy( name, proc_name, 100 );

    val* proc_name_struct = symbol_val_constructor( proc_name );

    return proc_name_struct;
}

val* primitives_procedures_names() {
    make_list( 26,
               make_primitives_procedure_name ("car"),
               make_primitives_procedure_name ("cdr"),
               make_primitives_procedure_name ("cons"),
               make_primitives_procedure_name ("list"),
               make_primitives_procedure_name ("set-car!"),
               make_primitives_procedure_name ("set-cdr!"),
               make_primitives_procedure_name ("+"),
               make_primitives_procedure_name ("-"),
               make_primitives_procedure_name ("*"),
               make_primitives_procedure_name ("/"),
               make_primitives_procedure_name ("="),
               make_primitives_procedure_name ("reverse"),
               make_primitives_procedure_name ("append"),
               make_primitives_procedure_name ("map"),
               make_primitives_procedure_name ("assoc"),
               make_primitives_procedure_name ("length"),
               make_primitives_procedure_name ("last-pair"),
               make_primitives_procedure_name ("true?"),
               make_primitives_procedure_name ("false?"),
               make_primitives_procedure_name ("null?"),
               make_primitives_procedure_name ("pair?"),
               make_primitives_procedure_name ("symbol?"),
               make_primitives_procedure_name ("atom?"),
               make_primitives_procedure_name ("string?"),
               make_primitives_procedure_name ("dotpair?"),
               make_primitives_procedure_name ("print") );
}

val* primitives_procedures_objects() {
    make_list(26,
              make_primitives_procedure_object( "car" ),
              make_primitives_procedure_object( "cdr" ),
              make_primitives_procedure_object( "cons" ),
              make_primitives_procedure_object( "make_list" ),
              make_primitives_procedure_object( "set_car" ),
              make_primitives_procedure_object( "set_cdr" ),
              make_primitives_procedure_object( "add" ),
              make_primitives_procedure_object( "sub" ),
              make_primitives_procedure_object( "mul" ),
              make_primitives_procedure_object( "division" ),
              make_primitives_procedure_object( "equal_numbers_predicate" ),
              make_primitives_procedure_object( "reverse" ),
              make_primitives_procedure_object( "append" ),
              make_primitives_procedure_object( "map" ),
              make_primitives_procedure_object( "assoc" ),
              make_primitives_procedure_object( "length" ),
              make_primitives_procedure_object( "last_pair" ),
              make_primitives_procedure_object( "true_predicate" ),
              make_primitives_procedure_object( "false_predicate" ),
              make_primitives_procedure_object( "null_predicate" ),
              make_primitives_procedure_object( "pair_predicate" ),
              make_primitives_procedure_object( "symbol_predicate" ),
              make_primitives_procedure_object( "atom_predicate" ),
              make_primitives_procedure_object( "string_predicate" ),
              make_primitives_procedure_object( "dotpair_predicate"),
              make_primitives_procedure_object( "ipprint")
              );
}

val* lookup_primitive_implementation_name( val* key, val* primitives ) {
    return ( assoc( key,  primitives ) );
}

val* list_of_values(val* exps, val* env ) {
    printf(" list_of_values exps: ");
    ipprint( exps );
    printf("\n");
    fflush(stdout);

    if ( no_operands_predicate( exps ) ) {
        return nil_constructor();
    } else {
        val* first_exp = eval( first_operand( exps ),  env );
        if ( error_predicate( first_exp ) ) {
            return first_exp;
        }
        return cons( first_exp,
                     list_of_values(rest_operands( exps ), env ) );
    }
}

int eq_names_predicate ( char* name1, char* name2 ) {
    /* printf("name1 %s\n", name1); */
    /* printf("name2 %s\n", name2); */
    if(strcmp( name1, name2 ) == 0 ) {
        /* printf("true\n"); */
        return 1;
    }
    /* printf("false\n"); */
    return 0;
}

val* apply_primitive_application( val* proc, val* args ) {

    char* proc_name = proc->uni_val.char_val;
    int* result = (int*)malloc(sizeof(int));;

    /* printf(" apply_primitive_application: "); */
    /* ipprint( args ); */
    /* printf("\n"); */
    /* fflush(stdout); */

    if ( eq_names_predicate( proc_name, "car" ) ) {
        if ( length( args ) == 1 ) {
            return car( car( args ) );

        } else {
            return car_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "cdr" ) ) {
        if ( length( args ) == 1 ) {
            return cdr( car( args ) );

        } else {
            return cdr_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "make_list" ) ) {
        return args;

    } else if ( eq_names_predicate( proc_name, "cons" ) ) {
        if ( length( args ) == 2 ) {
            return cons( car( args ), car( cdr ( args ) ) );

        } else {
            return cons_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "set_car" ) ) {
        if ( length ( args ) == 2 ) {
            val* list = car( args );
            val* new_value = car( cdr ( args ) );
            return set_car( list, new_value );

        } else {
            return set_car_args_error;
        }
    } else if ( eq_names_predicate( proc_name, "set_cdr" ) ) {
        if ( length ( args ) == 2 ) {
            val* list = car( args );
            val* new_value = car( cdr ( args ) );
            return set_cdr( list, new_value );

        } else {
            return set_cdr_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "ipprint" ) ) {
        ipprint( car( args ) );
        printf("\n");
        return ok;

    } else if ( eq_names_predicate( proc_name, "equal_numbers_predicate" ) ) {
        if ( length( args ) == 2 ) {
            val* n1 = car( args );
            val* n2 = car(cdr ( args ) );

            printf("n1: ");
            ipprint(n1);
            printf("\n");

            printf("n2: ");
            ipprint(n2);
            printf("\n");

            *result = equal_numbers_predicate( n1, n2 );
            printf("result =: %d\n ", *result );
            if ( *result == 1 ) {
                return ttrue;

            } else {
                return ffalse;
            }
        } else {
            return equal_numbers_predicate_error;
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
            return append_args_error;
        }
    /* } else if ( eq_names_predicate( proc_name, "map" ) ) { */

    /*     if ( length ( args ) == 2 ) { */
    /*         val* fn = car( args ); */
    /*         val* list = car( cdr ( args ) ); */
    /*         return map( fn, list ); */
    /*     } */

    } else if ( eq_names_predicate( proc_name, "assoc" ) ) {
        if ( length ( args ) == 2 ) {
            val* key = car( args );
            val* list = car( cdr ( args ) );
            return assoc( key, list );

        } else {
            return assoc_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "length" ) ) {
        if ( length ( args ) == 1 ) {
            *result = length( car( args ) );
            return int_val_constructor( result );

        } else {
            /* printf("length false\n"); */
            /* ipprint( length_args_error ); */
            /* printf("\n"); */

            return length_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "last_pair" ) ) {
        if ( length ( args ) == 1 ) {
            return last_pair( car( args ) );

        } else {
            return last_pair_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "true_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            *result = true_predicate( car( args ) );

            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return true_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "false_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            *result = false_predicate( car( args ) );

            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return false_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "null_predicate" ) ) {
        /* printf("null_predicate arg: "); */
        /* ipprint( args ); */
        /* printf("\n"); */
        /* fflush(stdout); */

        if ( length ( args ) == 1 ) {
            *result = null_predicate( car( args ) );
            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return null_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "pair_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            *result = pair_predicate( car( args ) );

            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return pair_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "symbol_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            *result = symbol_predicate( car( args ) );

            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return symbol_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "atom_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            *result = atom_predicate( car( args ) );
            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return atom_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "string_predicate" ) ) {
        if ( length ( args ) == 1 ) {
            *result = string_predicate( car( args ) );
            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return string_predicate_args_error;
        }

    } else if ( eq_names_predicate( proc_name, "dotpair_predicate" ) ) {

        if ( length ( args ) == 1 ) {
            *result = dotpair_predicate( car( args ) );

            if ( *result == 1 ) {
                return ttrue;
            }
            return ffalse;

        } else {
            return dotpair_predicate_args_error;
        }

    } else {
        return eval_primitive_procs_error;
    }
}

int compound_procedure_predicate( val* proc ) {
    if( pair_predicate ( proc ) ) {
        val* first_elt = car( proc );
        if (eq_names_predicate( first_elt->uni_val.char_val, "procedure" ) ) {
            return 1;
        }
    }
    return 0;
}

int primitive_procedure_predicate( val* proc ) {
    if( pair_predicate ( proc ) ) {
        val* first_elt = car( proc );

        if (eq_names_predicate( first_elt->uni_val.char_val, "primitive" ) ) {
            return 1;
        }
    }
    return 0;
}

val* primitive_implementation( val* proc ) {
    return car( cdr( proc ) );
}

val* error_in_args ( val* args ) {
    if (null_predicate( args ) ) {
        return nil_constructor();
    } else if ( pair_predicate( args ) || dotpair_predicate( args ) ) {

        if ( error_predicate ( car( args ) ) ) {
            return cons( car( args ), error_in_args ( cdr( args ) ) );

        } else {
            error_in_args ( cdr( args ) );
        }

    } else if ( atom_predicate ) {
        if (error_predicate ( args ) ) {
            return args;
        }
    }
}

val* apply( val* proc, val* args ) {
    /* printf ("apply args: "); */
    /* ipprint( args ); */
    /* printf("\n"); */

    val* args_errors = error_in_args( args );

    if ( !null_predicate( args_errors ) ) {
        return args_errors;

    } else if ( primitive_procedure_predicate( proc ) ) {
        return apply_primitive_application( primitive_implementation( proc ), args );

    } else if ( compound_procedure_predicate( proc ) ) {
        printf ("compound procedure args: ");
        ipprint( args );
        printf("\n");

        printf ("compound procedure parameters: ");
        ipprint( procedure_parameters( proc ) );
        printf("\n");

        val* new_env = extend_environment( procedure_parameters( proc ),
                                           args, procedure_environment( proc ) );
        return eval_sequence( procedure_body( proc ),
                              new_env );
    } else {
        return apply_error;
    }
}

val* set_variable_value_rec( val* vars, val* values, val* var, val* value ) {
    if ( null_predicate (  vars ) ) {
        return nil_constructor();
    } else {
        val* key = car( vars );

        if ( ( symbol_predicate( key ) )  &&
             ( symbol_predicate( var ) ) &&
             (strcmp
              ( key->uni_val.char_val, var->uni_val.char_val ) == 0 ) ) {
            set_car( values, value );
            return ok;
        } else {
            set_variable_value_rec( cdr( vars ), cdr( values), var, value );
        }
    }
}

val* set_variable_value( val* var, val* value, val* env ) {

    if ( null_predicate( env ) ) {
        /* char *string = malloc( sizeof( char[max_symbol_name_length] ) ); */
        /* strncpy( string, "ERR SET_VARIABLE_VALUE: unussigned variable", */
        /*          max_symbol_name_length ); */
        /* return error_val_constructor( string ); */
        return set_variable_value_error;
    } else {
        val* frame = first_frame( env );
        val* retval = set_variable_value_rec( frame_variables( frame ),
                                              frame_values( frame ), var, value );
        if ( null_predicate ( retval ) ) {
            set_variable_value( var, value, rest_frames( env ) );
        } else {
            return ok;
        }
    }
}

val* eval_assignment( val* exp, val * env ) {
    /* printf("eval_assignment exp: "); */
    /* ipprint( exp ); */
    /* printf("\n"); */

    /* printf("assignment_value: "); */
    /* ipprint( assignment_value( exp ) ); */
    /* printf("\n"); */

    val* retval = set_variable_value( assignment_variable( exp ),
                               eval ( assignment_value( exp ), env ), env );
    return retval;
}

val* define_variable( val* var, val* value, val* env ) {
   val*  frame = first_frame( env );

    val* define_variable_rec( val* vars, val* values) {
        if ( null_predicate( vars ) ) {
            add_binding( frame, var, value );
            return ok;
        } else {

            val* key = car( vars );

            if ( ( symbol_predicate( key ) )  &&
                 ( symbol_predicate( var ) ) &&
                 (strcmp
                  ( key->uni_val.char_val, var->uni_val.char_val ) == 0 ) ) {
                set_car(values, value );
                return ok;
            } else {
                define_variable_rec( cdr( vars) , cdr( values) );
            }
        }
    }

    define_variable_rec( frame_variables( frame ), frame_values( frame ) );
    return ok;
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

    /* printf(" env after define_variable: \n"); */
    /* ipprint( global_environment ); */
    /* printf("\n"); */

    return ok;
}

val* eval_if( val* exp, val * env ) {
    val* if_predicate_result = eval ( if_condition( exp ), env );
    /* printf("if_predicate_result: "); */
    /* ipprint( if_predicate_result ); */
    /* printf("\n"); */
    if ( symbol_predicate( if_predicate_result ) ) {

        /* printf("eval_if symbol\n"); */

        if ( eq_names_predicate("true",
                                if_predicate_result->uni_val.char_val ) ) {

            printf("if predicate is true \n");
            printf("\n");
            return eval( if_consequent( exp ), env );

        } else {
            printf("if predicate is false \n");
            printf("\n");
            return eval( if_alternative( exp ), env );
        }
    } else if (true_predicate ( if_predicate_result ) ) {
        return eval( if_consequent( exp ), env );

    } else {
        return eval( if_alternative( exp ), env );
    }
}

val* eval_sequence( val* exps, val * env ) {
    if ( last_exp_predicate( exps ) ) {
        return eval( first_exp( exps ), env );
    }
    eval( first_exp( exps ), env );
    return eval_sequence( rest_exps( exps ), env );
}

val* eval ( val* exp, val * env ) {
    printf("eval exp\n");
    ipprint( exp );
    printf("\n");
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

    } else if ( assignment_predicate( exp ) ) {
        return eval_assignment( exp, env );

    } else if ( define_predicate( exp ) ) {
        return eval_definition( exp, env );

    } else if ( if_predicate( exp ) ) {
        return eval_if( exp, env );

    } else if ( lambda_predicate( exp ) ) {
        return make_procedure( lambda_params( exp ),
                               lambda_body( exp ), env );
    } else if ( begin_predicate( exp ) ) {
        return eval_sequence( begin_actions( exp ), env );

    } else if ( application_predicate( exp ) ) {
        apply( eval( operator( exp ), env ), list_of_values( operands( exp), env) );

    } else {
        /* printf("Unknown type of exp\n"); */
        return eval_error;
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

            val* result = eval( list, global_environment );

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

    primitives_objects = primitives_procedures_objects();
    primitives_names = primitives_procedures_names();
    setup_env();

    init_primitives_errors();
    init_env_errors();
    init_syntax_errors();
    init_syntax_procs_errors();
    init_eval_errors();

    eval_driver_loop( 10000, 1000 );
    return 1;

}
