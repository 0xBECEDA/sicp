#include "primitives.h"

/* сборка */
/* gcc test.c environment.c -o env */
/* ./env */

val* make_bilding_error;
val* extend_environment_error;
val* lookup_value_error;
val* lookup_variable_error;

val* init_env_errors() {
    char *string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR MAKE_BINDING: var name isn't a symbol",
             max_symbol_name_length );
    make_bilding_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR EXTEND_ENVIRONMENT: every variable should have a value",
             max_symbol_name_length );
    extend_environment_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "lookup_value: value doens't found in that frame",
             max_symbol_name_length );
    lookup_value_error = error_val_constructor( string );

    string = malloc( sizeof( char[max_symbol_name_length] ) );
    strncpy( string, "ERR LOOKUP_VARIABLE: unussigned variable",
             max_symbol_name_length );
    lookup_variable_error = error_val_constructor( string );
}

val* make_empty_frame() {
    return nil_constructor();
}

val* make_binding(val* var_name, val* var_value) {
    if ( symbol_predicate( var_name ) ) {
        return cons( var_name, var_value );
    }
    return make_bilding_error;
}

val* var_binding(val* binding) {
    return car( binding );
}

val* value_binding(val* binding) {
    return cdr( binding );
}

/* добавляет связывание в кадр, если имя переменной раньше не встречалось в кадре
   иначе присваивает имени новое значение */


/* (define (add-binding-to-frame! var val frame) */
/*  (set-car! frame (cons var (car frame))) */
/*  (set-cdr! frame (cons val (cdr frame)))) */

val* add_binding( val* frame, val* var_name, val* var_value ) {
    /* printf("add_binding: frame: "); */
    /* ipprint( frame ); */
    /* printf("\n"); */

    set_car( frame, cons( var_name, car( frame ) ) );
    set_cdr( frame, cons(var_value, cdr( frame ) ) );
    return frame;
}

val* make_frame( val* vars, val* values ) {
    return cons( vars, values );
}

val* frame_variables( val* frame ) {
    car( frame );
}

val* frame_values( val* frame ) {
    cdr( frame );
}

val* extend_environment(val* vars, val* values, val* base_env ) {
    if ( ( length( vars ) ) == ( length( values ) ) ) {
        return cons( make_frame( vars, values ), base_env );
    } else {
        return extend_environment_error;
    }
}

/* ищет значение переменной в кадре, если она есть */
val* lookup_value( val* frame, val* var_name ) {
    val* lookup_value_rec( val* vars, val* values ) {
        if ( null_predicate( vars ) ) {
            return lookup_value_error;
        } else {
            val* key = car( vars );
            if ( ( symbol_predicate( key ) )  &&
                 ( symbol_predicate( var_name ) ) &&
                 (strcmp
                  ( key->uni_val.char_val, var_name->uni_val.char_val ) == 0 ) ) {

                /* printf("lookup_value: value is found\n "); */
                return car( values );
            }
            lookup_value_rec( cdr( vars ), cdr( values ) );
        }
    }
    return lookup_value_rec( frame_variables( frame ), frame_values( frame ) );
}

val* make_empty_environment() {
    return nil_constructor();
}

val* add_frame(val* frame, val* env) {
    env = cons(frame, env);
    return env;
}

/* удаляет самый "внутренний" кадр
   это нужно, когда при исполнении процедуры мы сначала расширили окружение новым
   кадром, а покидая процедуру, должны удалить его*/
val* delete_frame(val* env) {
    env = cdr( env );
    return env;
}

val* first_frame(val* env) {
    car( env );
}

val* rest_frames(val* env) {
    cdr( env );
}


/* ищет значение пременной в окружении, если она есть */
val* lookup_variable(val* var_name, val* env) {
    /* printf("lookup_variable: search value for variable: \n"); */
    /* ipprint( var_name ); */
    /* printf("\n"); */

    if ( null_predicate( env ) ) {
        /* printf("lookup_variable: value for variable "); */
        /* ipprint( var_name ); */
        /* printf(" is NOT found \n "); */

         return lookup_variable_error;
    } else {
        val* first = first_frame( env );
        val* value = lookup_value(first, var_name);

        if ( error_predicate( value ) ) {
            lookup_variable(var_name, rest_frames( env ) );
        } else {
            /* printf("lookup_variable: value for variable "); */
            /* ipprint( var_name ); */
            /* printf(" is found \n "); */
            return value;
        }
    }
}

void test_frames() {
    val* frame = make_empty_frame();

    char* ptr_a = malloc(sizeof(char[1]));
    strncpy( ptr_a, "a", 1 );
    val* a_val = string_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = symbol_val_constructor( ptr_b );

    char* ptr_c = malloc(sizeof(char[1]));
    strncpy( ptr_c, "c", 1 );
    val* c_val = string_val_constructor( ptr_c );

    char* ptr_d = malloc(sizeof(char[1]));
    strncpy( ptr_d, "d", 1 );
    val* d_val = symbol_val_constructor( ptr_d );

    int* ptr_1 = malloc(sizeof(int));
    *ptr_1 = 1;
    val* val_1 = int_val_constructor( ptr_1 );

    int* ptr_2 = malloc(sizeof(int));
    *ptr_2 = 2;
    val* val_2 = int_val_constructor( ptr_2 );

    int* ptr_3 = malloc(sizeof(int));
    *ptr_3 = 3;
    val* val_3 = int_val_constructor( ptr_3 );

    int* ptr_4 = malloc(sizeof(int));
    *ptr_4 = 4;
    val* val_4 = int_val_constructor( ptr_4 );

    /* последовательно добавляем новые связывания к кадру */
    frame = add_binding(frame, a_val, val_1);
    frame = add_binding(frame, b_val, val_2);
    frame = add_binding(frame, c_val, val_3);
    frame = add_binding(frame, d_val, val_4);
    ipprint(frame);
    printf("\n");

    /* пробуем заменить значение переменной */
    frame = add_binding(frame, d_val, val_2);
    ipprint(frame);
    printf("\n");

    /* ищем значение переменной */
    val* value = lookup_value(frame, c_val);
    ipprint(value);
    printf("\n");

    /* пробуем найти значение переменной, которой нет - тест обработчика ошибки*/
    char* ptr_f = malloc(sizeof(char[1]));
    strncpy( ptr_d, "f", 1 );
    val* f_val = string_val_constructor( ptr_f );

    value = lookup_value(frame, f_val);
    ipprint(value);
    printf("\n");

    /* пробуем добавить связывание, где имя переменной не является символом */
    frame = add_binding(frame, val_4, d_val);
}

void test_env() {
    val* frame1 = make_empty_frame();
    val* frame2 = make_empty_frame();

    char* ptr_a = malloc(sizeof(char[1]));
    strncpy( ptr_a, "a", 1 );
    val* a_val = symbol_val_constructor( ptr_a );

    char* ptr_b = malloc(sizeof(char[1]));
    strncpy( ptr_b, "b", 1 );
    val* b_val = symbol_val_constructor( ptr_b );

    char* ptr_c = malloc(sizeof(char[1]));
    strncpy( ptr_c, "c", 1 );
    val* c_val = symbol_val_constructor( ptr_c );

    char* ptr_d = malloc(sizeof(char[1]));
    strncpy( ptr_d, "d", 1 );
    val* d_val = symbol_val_constructor( ptr_d );

    char* ptr_e = malloc(sizeof(char[1]));
    strncpy( ptr_e, "e", 1 );
    val* e_val = symbol_val_constructor( ptr_e );

    char* ptr_f = malloc(sizeof(char[1]));
    strncpy( ptr_f, "f", 1 );
    val* f_val = symbol_val_constructor( ptr_f );

    int* ptr_1 = malloc(sizeof(int));
    *ptr_1 = 1;
    val* val_1 = int_val_constructor( ptr_1 );

    int* ptr_2 = malloc(sizeof(int));
    *ptr_2 = 2;
    val* val_2 = int_val_constructor( ptr_2 );

    int* ptr_3 = malloc(sizeof(int));
    *ptr_3 = 3;
    val* val_3 = int_val_constructor( ptr_3 );

    int* ptr_4 = malloc(sizeof(int));
    *ptr_4 = 4;
    val* val_4 = int_val_constructor( ptr_4 );

    int* ptr_5 = malloc(sizeof(int));
    *ptr_5 = 5;
    val* val_5 = int_val_constructor( ptr_5 );

    int* ptr_6 = malloc(sizeof(int));
    *ptr_6 = 6;
    val* val_6 = int_val_constructor( ptr_6 );

    /* создаем 2 кадра */
    /* frame1 = add_binding(frame1, a_val, val_1); */
    /* frame1 = add_binding(frame1, b_val, val_2); */
    /* frame1 = add_binding(frame1, c_val, val_3); */
    /* frame1 = add_binding(frame1, d_val, val_4); */

    add_binding(frame1, a_val, val_1);
    add_binding(frame1, b_val, val_2);
    add_binding(frame1, c_val, val_3);
    add_binding(frame1, d_val, val_4);

    printf("frame1: \n");
    ipprint(frame1);
    printf("\n");

    frame2 = add_binding(frame2, e_val, val_5);
    frame2 = add_binding(frame2, f_val, val_6);

    printf("frame2: \n");
    ipprint(frame2);
    printf("\n");

    /* добавляем кадры в окружение */
    val* env = make_empty_environment();
    env = add_frame(frame1, env);
    env = add_frame(frame2, env);

    printf("env: \n");
    ipprint(env);
    printf("\n");

    /* ищем значения переменных в кадрах */
    val* value = lookup_variable(d_val, env);
    ipprint(value);
    printf("\n");

    value = lookup_variable(f_val, env);
    ipprint(value);
    printf("\n");

    /* удаляем кадр */
    printf("env : \n");
    env = delete_frame(env);
    ipprint(env);
    printf("\n");

    /* пробуем найти значение переменной, которой в окружении нет */
    value = lookup_variable(f_val, env);
    ipprint(value);
    printf("\n");
}

/* int main (void) { */
/*     /\* test_frames(); *\/ */
/*     test_env(); */
/*     return 0; */
/* } */
