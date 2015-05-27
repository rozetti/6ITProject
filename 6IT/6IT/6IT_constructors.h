/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

// todo crz: de-hack these?
extern int machine_first_chance_exception_handler(struct exception_t*);

extern _6IT_CONSTRUCTOR(bios, 6IOS);

extern _6IT_CONSTRUCTOR(evaluator, 6EV);
extern _6IT_CONSTRUCTORX(expression_parser, 6X, struct scanner_t*);
extern _6IT_CONSTRUCTOR(downcompiler, 6YM);
extern _6IT_CONSTRUCTOR(processor, 6IL);
extern _6IT_CONSTRUCTOR(compiler, SubC);
extern _6IT_CONSTRUCTOR(scanner, 6OO);
extern _6IT_CONSTRUCTORX(thread, thread, struct machine_config_t*);
extern _6IT_CONSTRUCTOR(scheduler, scheduler);
extern _6IT_CONSTRUCTOR(interrupt_controller, interrupt_controller);
extern _6IT_CONSTRUCTOR(expression_emitter, expression_emitter);
extern _6IT_CONSTRUCTOR(instruction_emitter, instruction_emitter);
extern _6IT_CONSTRUCTOR(debugger, 6UG);
extern _6IT_CONSTRUCTORX(frame, stack_frame, int position);

extern void construct_default_H6VM_config(struct machine_config_t *config);
extern _6IT_CONSTRUCTORX(machine, H6VM, struct machine_config_t config);
//extern void construct_H6VM(struct machine_t*, struct machine_config_t);
//extern void construct_evaluator_stack_value(struct value_t *value);
extern _6IT_CONSTRUCTOR(value, evaluator_stack_value);

// todo crz: formalise these
void construct_expression_parser_state(struct expression_parser_state_t*, struct expression_parser_t*);
void construct_expression(struct expression_t*);
struct environment_t construct_environment();
void construct_token(struct token_t*);

// todo crz: get rid of this
void init_new_expression(struct machine_t*, struct expression_t*);
