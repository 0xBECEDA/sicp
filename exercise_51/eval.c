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

    return cons( proc_name_struct, proc_fn_name_struct );
    /* return make_list( 2, proc_name_struct, proc_fn_name_struct ); */
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

val* primitives = primitives_procedures();

val* lookup_primitive_implementation_name( val* key ) {
    return ( assoc( key,  primitives ) );
}

val* eval_primitive_application( ) {

}

val* eval ( val* env, val* exp ) {

}

int main () {
    val* primitives = primitives_procedures();
    /* ipprint( primitives_procedures() ); */
    char* list = malloc(sizeof(char[4]));
    strncpy( list, "list", 4 );
    val* key = symbol_val_constructor( list );

    ipprint( assoc( key,  primitives ) );
    printf("\n");
    return 1;
}
