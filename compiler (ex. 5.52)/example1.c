#include <stdio.h>
 #include <stdlib.h>
 #include "primitives.h"
val* a847326544770713 ( ) {
    val* a847326544898440 ( ) {
        val* a2133253803914630 ( ) {
            int* ptr_a873244444701044 = malloc ( sizeof ( int ) );
            * ptr_a873244444701044 = 1;
            val* a873244444701044 = int_val_constructor ( ptr_a873244444701044 );
            int* ptr_a923577301281205 = malloc ( sizeof ( int ) );
            * ptr_a923577301281205 = 2;
            val* a923577301281205 = int_val_constructor ( ptr_a923577301281205 );
            return make_list ( 2,  a873244444701044,  a923577301281205 );
        }
        val* a62111787537478 ( ) {
            int* ptr_a906799682984780 = malloc ( sizeof ( int ) );
            * ptr_a906799682984780 = 3;
            val* a906799682984780 = int_val_constructor ( ptr_a906799682984780 );
            int* ptr_a822911587272986 = malloc ( sizeof ( int ) );
            * ptr_a822911587272986 = 4;
            val* a822911587272986 = int_val_constructor ( ptr_a822911587272986 );
            return make_list ( 2,  a906799682984780,  a822911587272986 );
        }
        val* a3956398523195373 ( ) {
            int* ptr_a806133968962643 = malloc ( sizeof ( int ) );
            * ptr_a806133968962643 = 5;
            val* a806133968962643 = int_val_constructor ( ptr_a806133968962643 );
            int* ptr_a856466825821811 = malloc ( sizeof ( int ) );
            * ptr_a856466825821811 = 6;
            val* a856466825821811 = int_val_constructor ( ptr_a856466825821811 );
            return make_list ( 2,  a806133968962643,  a856466825821811 );
        }
        return make_list ( 3,  a2133253803914630 ( ) ,  a62111787537478 ( ) ,  a3956398523195373 ( )  );
    }
    return a847326544898440 ( );
}
int main ( ) {
    set_true_and_false ( );
    ipprint ( a847326544770713 ( ) );
    printf("\n");
}
