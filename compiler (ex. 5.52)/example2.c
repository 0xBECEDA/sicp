#include <stdio.h>
 #include <stdlib.h>
 #include "primitives.h"
val* test ( val* g ) {
    val* a1919378405312539 ( ) {
        val* a3792446982829261 ( ) {
            return g;
        }
        val* a2705548723353238 ( val* x ) {
            val* a29050258166163 ( ) {
                val* a4245442695579296 ( ) {
                    return x;
                }
                val* a873244444289875 ( ) {
                    int* ptr_a87324444418274 = malloc ( sizeof ( int ) );
                    * ptr_a87324444418274 = 1;
                    val* a87324444418274 = int_val_constructor ( ptr_a87324444418274 );
                    return a87324444418274;
                }
                return add ( make_list ( 2,  a4245442695579296 ( ) ,  a873244444289875 ( )  ) );
            }
            return a29050258166163 ( );
        }
        return a2705548723353238 ( a3792446982829261 ( )  );
    }
    return a1919378405312539 ( );
}
val* a1029958558124344 ( ) {
    val* a923577301182646 ( ) {
        int* ptr_a923577301357896 = malloc ( sizeof ( int ) );
        * ptr_a923577301357896 = 2;
        val* a923577301357896 = int_val_constructor ( ptr_a923577301357896 );
        return a923577301357896;
    }
    return test ( a923577301182646 ( )  ) ;
}
int main ( ) {
    set_true_and_false ( );
    ipprint ( test );
    printf("\n");
}
