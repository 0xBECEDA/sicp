#ifndef ENV_H
#define ENV_H

#include "primitives.h"

val* var_binding(val* binding);

val* make_empty_frame();

val* make_binding(val* var_name, val* var_value);

val* var_binding(val* binding);

val* value_binding(val* binding);

val* add_binding(val* frame, val* var_name, val* var_value);

val* lookup_value(val* frame, val* var_name);

val* make_empty_environment();

val* add_frame(val* frame, val* env);

val* delete_frame(val* env);

val* first_frame(val* env);

val* rest_frames(val* env);

val* lookup_variable(val* var_name, val* env);

#endif
