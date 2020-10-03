#ifndef SYNTAX_PROCS_H
#define SYNTAX_PROCS_H

#include "primitives.h"

int var_predicate( val* exp );

int self_evaluating_predicate( val* exp );

int quote_predicate( val* exp );

val* text_of_quotation( val* exp );

int assigment_predicate( val* exp );

val* assignment_variable( val* exp );

val* assignment_value( val* exp );

int lambda_predicate( val* exp );

val* lambda_params( val* exp );

val* lambda_body( val* exp );

val* make_lambda( val* params, val*body );

int let_predicate( val* exp );

val* let_vars_bindings( val* exp );

val* let_body( val* exp );

int if_predicate( val* exp );

val* if_condition( val* exp );

val* if_consequent( val* exp );

val* if_alternative( val* exp );

val* make_if( val* predicate, val* consequent, val* alternative );

int define_predicate( val* exp );

val* definition_var( val* exp );

val* definition_value( val* exp );

int begin_predicate( val* exp );

val* begin_actions( val *exp );

int last_exp_predicate( val* seq );

val* first_exp( val* seq );

val* rest_exps( val* seq );

val* make_begin( val* seq );

val* trasform_seq_to_exp( val* seq );

int application_predicate( val* exp );

val* operator( val* exp );

val* operands( val* exp );

int no_operands_predicate( val* ops );

val* firs_operand( val* ops );

val* rest_operands( val* ops );

#endif
