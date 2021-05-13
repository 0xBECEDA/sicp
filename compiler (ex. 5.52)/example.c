/* gcc example.c test.c -o test */
#include <stdio.h>
#include <stdlib.h>
#include "primitives.h"
val* n;
val* a3943445553 ( ) {
    int* ptr_n = malloc ( sizeof ( int ) );
    * ptr_n = 10;
    n = int_val_constructor ( ptr_n );
}

val* f;
val* a3809224601 ( ) {
    char* ptr_f = malloc ( sizeof ( char [ 100 ] ) );
    strncpy ( ptr_f,  "some string",  100 );
    f = string_val_constructor ( ptr_f );
}

val* b;
val* a3876335077 ( ) {
    int* ptr_int_b;
    char* ptr_char_b;
    switch  ( n->type_num )  {
    case TYPE_INT:
        ptr_int_b = malloc ( sizeof ( int ) ) ;
        * ptr_int_b =* n->uni_val.int_val ;
        b = int_val_constructor ( ptr_int_b );
        break;
    case TYPE_STRING:
        ptr_char_b = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( ptr_char_b,  n->uni_val.char_val,  100 );
        b = string_val_constructor ( ptr_char_b );
        break;
    case TYPE_SYMBOL:
        ptr_char_b = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( ptr_char_b,  n->uni_val.char_val,  100 );
        b = symbol_val_constructor ( ptr_char_b );
        break;
    case TYPE_NIL:
        b = nil_constructor ( );
        break;
    case TYPE_CELL:
        b = cell_val_constructor ( n->uni_val.cell_val );
        break;
    }
}

val* a;
val* a3826002220 ( ) {
    int* ptr_int_a;
    char* ptr_char_a;
    switch  ( f->type_num )  {
    case TYPE_INT:
        ptr_int_a = malloc ( sizeof ( int ) ) ;
        * ptr_int_a =* f->uni_val.int_val ;
        a = int_val_constructor ( ptr_int_a );
        break;
    case TYPE_STRING:
        ptr_char_a = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( ptr_char_a,  f->uni_val.char_val,  100 );
        a = string_val_constructor ( ptr_char_a );
        break;
    case TYPE_SYMBOL:
        ptr_char_a = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( ptr_char_a,  f->uni_val.char_val,  100 );
        a = symbol_val_constructor ( ptr_char_a );
        break;
    case TYPE_NIL:
        a = nil_constructor ( );
        break;
    case TYPE_CELL:
        a = cell_val_constructor ( f->uni_val.cell_val );
        break;
    }
}

val* c;
val* a3859557458 ( ) {
    val* a1495290328 ( ) {
        int* ptr_a873244444 = malloc ( sizeof ( int ) );
        * ptr_a873244444 = 1;
        val* a873244444 = int_val_constructor ( ptr_a873244444 );
        int* ptr_a923577301 = malloc ( sizeof ( int ) );
        * ptr_a923577301 = 2;
        val* a923577301 = int_val_constructor ( ptr_a923577301 );
        int* ptr_a906799682 = malloc ( sizeof ( int ) );
        * ptr_a906799682 = 3;
        val* a906799682 = int_val_constructor ( ptr_a906799682 );
        return make_list ( 3,  a873244444,  a923577301,  a906799682 );
    }
    c = a1495290328 ( );
}

val* b3943445553 ( ) {
    char* ptr_n = malloc ( sizeof ( char [ 100 ] ) );
    strncpy ( ptr_n,  "some string",  100 );
    n->uni_val.char_val = ptr_n;
    n->type_num = TYPE_STRING;
}

val* g;
val* a3792446982 ( ) {
    int* ptr_int_g;
    char* ptr_char_g;
    switch  ( c->type_num )  {
    case TYPE_INT:
        ptr_int_g = malloc ( sizeof ( int ) ) ;
        * ptr_int_g =* c->uni_val.int_val ;
        g = int_val_constructor ( ptr_int_g );
        break;
    case TYPE_STRING:
        ptr_char_g = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( ptr_char_g,  c->uni_val.char_val,  100 );
        g = string_val_constructor ( ptr_char_g );
        break;
    case TYPE_SYMBOL:
        ptr_char_g = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( ptr_char_g,  c->uni_val.char_val,  100 );
        g = symbol_val_constructor ( ptr_char_g );
        break;
    case TYPE_NIL:
        g = nil_constructor ( );
        break;
    case TYPE_CELL:
        g = cell_val_constructor ( c->uni_val.cell_val );
        break;
    }
}

val* b3876335077 ( ) {
    int* ptr_int_b;
    char* ptr_char_b;
    switch  ( g->type_num )  {
    case TYPE_INT:
        * b->uni_val.int_val =* g->uni_val.int_val;
        b->type_num = TYPE_INT;
        break;
    case TYPE_STRING:
        strncpy ( b->uni_val.char_val,  g->uni_val.char_val,  100 );
        b->type_num = TYPE_STRING;
        break;
    case TYPE_SYMBOL:
        strncpy ( b->uni_val.char_val,  g->uni_val.char_val,  100 );
        b->type_num = TYPE_SYMBOL;
        break;
    case TYPE_NIL:
        b->type_num = TYPE_NIL;
        break;
    case TYPE_CELL:
        * b->uni_val.cell_val =* g->uni_val.cell_val;
        b->type_num = TYPE_CELL;
        break;
    }
}

val* c3809224601 ( ) {
    * f->uni_val.int_val = 0;
    f->type_num = TYPE_INT;
}

val* b3859557458 ( ) {
    int* ptr_int_c;
    char* ptr_char_c;
    switch  ( f->type_num )  {
    case TYPE_INT:
        * c->uni_val.int_val =* f->uni_val.int_val;
        c->type_num = TYPE_INT;
        break;
    case TYPE_STRING:
        c->uni_val.char_val = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( c->uni_val.char_val,  f->uni_val.char_val,  100 );
        c->type_num = TYPE_STRING;
        break;
    case TYPE_SYMBOL:
        c->uni_val.char_val = malloc ( sizeof ( char [ 100 ] ) ) ;
        strncpy ( c->uni_val.char_val,  f->uni_val.char_val,  100 );
        c->type_num = TYPE_SYMBOL;
        break;
    case TYPE_NIL:
        c->type_num = TYPE_NIL;
        break;
    case TYPE_CELL:
        * c->uni_val.cell_val =* f->uni_val.cell_val;
        c->type_num = TYPE_CELL;
        break;
    }
}

int main() {
    set_true_and_false();

    a3943445553 ( );
    a3809224601 ( );
    a3876335077 ( );
    a3859557458 ( );
    a3826002220 ( );
    a3792446982 ( );

    printf("n: ");
    ipprint(n);
    printf("\n");

    printf("f: ");
    ipprint(f);
    printf("\n");

    printf("b: ");
    ipprint(b);
    printf("\n");

    printf("c: ");
    ipprint(c);
    printf("\n");

    printf("g: ");
    ipprint(g);
    printf("\n");

    printf("a: ");
    ipprint(a);
    printf("\n");

    b3943445553( );
    printf("redefinition n: ");
    ipprint(n);
    printf("\n");

    b3876335077 ( );
    printf("redefinition b: ");
    ipprint(b);
    printf("\n");

    b3859557458 ( );
    printf("redefinition c: ");
    ipprint(c);
    printf("\n");


}


val* a;
val* some_fn1 () {

    val* a;
    a = 1;

    val* some_fn2 () {
        val* a;
        a = 2;

    }
}
