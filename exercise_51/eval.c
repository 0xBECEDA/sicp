#include "syntax_procs.h"

/* сборка gcc eval.c syntax_procs.c syntax.c test.c environment.c -o proga */

val* setup_env() {

}

val* bind_proc_name_and_proc_fn ( char *proc_name, char *proc_fn_name ) {
    char* name = malloc(sizeof(char[100]));
    char* fn_name = malloc(sizeof(char[100]));
    strncpy( name, proc_name, 100 );
    strncpy( fn_name, proc_fn_name, 100 );

    val* proc_name_struct = symbol_val_constructor( proc_name );
    val* proc_fn_name_struct = symbol_val_constructor( fn_name );

    /* return cons( proc_name_struct, proc_fn_name_struct ); */
    return make_list( 2, proc_name_struct, proc_fn_name_struct );
}

val* primitives_procedures() {

    make_list(24,
              bind_proc_name_and_proc_fn( "car", "car" ),
              bind_proc_name_and_proc_fn( "cdr", "cdr" ),
              bind_proc_name_and_proc_fn( "cons", "cons" ),
              bind_proc_name_and_proc_fn( "list", "make_list" ),
              bind_proc_name_and_proc_fn( "set_car!", "set_car" ),
              bind_proc_name_and_proc_fn( "set_cdr!", "set_cdr" ),
              bind_proc_name_and_proc_fn( "+", "add" ),
              bind_proc_name_and_proc_fn( "-", "sub" ),
              bind_proc_name_and_proc_fn( "*", "mul" ),
              bind_proc_name_and_proc_fn( "/", "division" ),
              bind_proc_name_and_proc_fn( "reverse", "reverse" ),
              bind_proc_name_and_proc_fn( "append", "append" ),
              bind_proc_name_and_proc_fn( "map", "map" ),
              bind_proc_name_and_proc_fn( "assoc", "assoc" ),
              bind_proc_name_and_proc_fn( "length", "length" ),
              bind_proc_name_and_proc_fn( "last_pair", "last_pair" ),
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

/* (define (list-of-values exps env) */
/*  (if (no-operands? exps) */
/*      '() */
/*           (cons (eval (first-operand exps) env) */
/*            (list-of-values (rest-operands exps) env)))) */

int eq_names_predicate ( char* name1, char* name2 ) {
    if(strcmp( name1, name2 ) == 0 ) {
        return 1;
    }
    return 0;
}

val* apply_primitive_application( val* proc, val* args ) {

    char* proc_name = proc->uni_val.char_val;

    if ( eq_names_predicate( proc_name, "car" ) ) {
        return car( args );

    } else if ( eq_names_predicate( proc_name, "cdr" ) ) {
        return cdr( args );

    } else if ( eq_names_predicate( proc_name, "make_list" ) ) {
        return args;

    } else if ( eq_names_predicate( proc_name, "cons" ) ) {
        return cons( car( args ), car( cdr ( args ) ) );

    } else if ( eq_names_predicate( proc_name, "set_car" ) ) {
        if ( length ( args ) == 2 ) {
            val* cell = car( args );
            val* list = car( cdr ( args ) );
            return set_car( list, cell );

        } else {
            char *string = malloc( sizeof( char[100] ) );
            strncpy( string, "SET-CAR! ERROR: set-car! needs exactly 2 args", 100 );
            return error_val_constructor( string );
        }
    } else if ( eq_names_predicate( proc_name, "set_cdr" ) ) {
        if ( length ( args ) == 2 ) {
            val* cell = car( args );
            val* list = car( cdr ( args ) );
            return set_cdr( list, cell );

        } else {
            char *string = malloc( sizeof( char[100] ) );
            strncpy( string, "SET-CDR! ERROR: set-cdr! needs exactly 2 args", 100 );
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
        return reverse( args );

    } else if ( eq_names_predicate( proc_name, "append" ) ) {
        val* list1 = car( args );
        val* list2 = cadr( args );
        return append( list1, list2 );

    } else {
        printf("eval_primitive_application: Unknown proc\n");
    }
}

/* (define (eval exp env) */
/*  (cond ((self-evaluating? exp) exp) */
/*   ((variable? exp) (lookup-variable-value exp env)) */
/*   ((quoted? exp) (text-of-quotation exp)) */
/*   ((assignment? exp) (eval-assignment exp env)) */
/*   ((definition? exp) (eval-definition exp env)) */
/*   ((if? exp) (eval-if exp env)) */
/*   ((lambda? exp) */
/*    (make-procedure (lambda-parameters exp) */
/*     (lambda-body exp) */
/*     env)) */
/*   ((begin? exp) */
/*    (eval-sequence (begin-actions exp) env)) */
/*   ((cond? exp) (eval (cond->if exp) env)) */
/*   ((application? exp) */
/*    (apply (eval (operator exp) env) */
/*     (list-of-values (operands exp) env))) */

val* eval_assigment( val* exp, val * env ) {

}

val* eval_definition( val* exp, val * env ) {

}

val* eval_if( val* exp, val * env ) {

}

val* eval_sequence( val* exp, val * env ) {

}

val* make_procedure( val* params, val* body ) {

}

val* eval ( val* exp, val * env ) {
    if ( self_evaluating_predicate( exp ) ) {
        return exp;

    } else if ( var_predicate( exp ) ) {
        return lookup_variable( exp, env );

    } else if ( quote_predicate( exp ) ) {
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
        apply( eval( operator( exp ) ), list-of-values( operands( exp), env) );

    } else {
        printf("Unknown type of exp\n");
    }
}

int main () {
    /* val* primitives = primitives_procedures(); */
    /* ipprint( primitives_procedures() ); */
    char* list = malloc(sizeof(char[4]));
    strncpy( list, "list", 4 );
    val* key = symbol_val_constructor( list );

    ipprint( assoc( key, primitives_procedures() ) );
    printf("\n");
    return 1;
}
