#ifndef SYNTAX_PROCS_H
#define SYNTAX_PROCS_H

#include "primitives.h"
#include "syntax.h"
#include "env.h"

int var_predicate( val* exp );

int self_evaluating_predicate( val* exp );

val* init_syntax_procs_errors();

int quote_predicate( val* exp );

val* text_of_quotation( val* exp );

int assignment_predicate( val* exp );

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

val* make_procedure( val* params, val* body, val* env );

val* procedure_parameters( val* proc );

val* procedure_body( val* proc );

val* procedure_environment( val* proc );

int define_predicate( val* exp );

val* definition_var( val* exp );

val* definition_value( val* exp );

int begin_predicate( val* exp );

val* begin_actions( val *exp );

int last_exp_predicate( val* seq );

val* first_exp( val* seq );

val* rest_exps( val* seq );

val* first_operand( val* operands );

val* rest_operands( val* operands );

int and_predicate( val* exp );

val* and_clauses( val* exp );

int or_predicate( val* exp );

val* or_clauses( val* exp );

int not_predicate( val* exp );

val* not_clause( val* exp );

val* make_begin( val* seq );

val* trasform_seq_to_exp( val* seq );

val* transform_cond_to_if( val* exp );

int cond_predicate( val* exp );

int cond_else_predicate( val* exp );

val* cond_clauses( val* exp );

val* clause_predicate( val* caluse );

val* cond_actions( val* clause );

val* expand_clauses( val* clauses );

val* init_syntax_procs_errors();

int application_predicate( val* exp );

val* operator( val* exp );

val* operands( val* exp );

int no_operands_predicate( val* ops );

val* firs_operand( val* ops );

val* rest_operands( val* ops );

#endif
