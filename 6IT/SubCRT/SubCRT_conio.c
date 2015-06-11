/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../SubCRT.h"

// todo crz: work out something for this lot

//int lib_conio_getche(struct frame_t *frame, struct evaluator_stack_t *stack)
//{
//	char ch;
//	ch = _getche(); // crz
//
//	return ch;
//}

//int lib_conio_putch(struct frame_t *frame, struct evaluator_stack_t *stack)
//{
//	struct value_t v = pop_value(stack, TYPE_INT);
//	int value = (char const *)v.data;
//
//	machine->printf(machine, "%c", value); // todo crz: console output
//
//	return value;
//}
//
//int lib_conio_puts(struct frame_t *frame, struct evaluator_stack_t *stack)
//{
//	if (scanner->token.type != TOKEN_TYPE_STRING) die("should be a string");
//
//	machine->printf(machine, "%s", scanner->token.token);
//
//	return 0;
//}
//

_6IT_PRIVATE void _6IT_MACHINECALL(add_library_conio)
{
	//int putch_parms[] = { TYPE_CHAR, 0 };
	//machine_add_builtin(machine, TYPE_INT, "putch", lib_conio_putch, putch_parms);

	//machine_add_builtin(machine, TYPE_INT, "getche", lib_conio_getche, 0);

	//int puts_parms[] = { TYPE_CHAR | TYPE_POINTER | TYPE_CONST, 0 };
	//machine_add_builtin(machine, TYPE_INT, "puts", lib_conio_puts, puts_parms);
}