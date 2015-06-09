/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "6X.h"

_6X_METHODX(data_type_t, parse_data_type, struct expression_parser_state_t *state)
{
	struct machine_t *machine = _This->machine;
	struct scanner_t *scanner = _This->scanner;

	struct token_t save = scanner->token;
	struct scanner_state_t save_state = scanner->state;

	data_type_t type = TYPE_INVALID;

	if (scanner->try_consume_string(scanner, "extern")) type |= TYPE_EXTERN;

	if (scanner->try_consume_string(scanner, "const")) type |= TYPE_CONST;

	if (scanner->try_consume_string(scanner, "signed"))
	{
		type &= (((unsigned)-1) - TYPE_UNSIGNED); // todo crz: see if this works
	}
	else if (scanner->try_consume_string(scanner, "unsigned"))
	{
		type |= TYPE_UNSIGNED;
	}

	if (scanner->try_consume_string(scanner, "int"))
	{
		type |= TYPE_INT;
	}
	else if (scanner->try_consume_string(scanner, "char"))
	{
		type |= TYPE_CHAR;
	}
	else if (scanner->try_consume_string(scanner, "float"))
	{
		type |= TYPE_FLOAT;
	}
	else if (scanner->try_consume_string(scanner, "double"))
	{
		type |= TYPE_DOUBLE;
	}
	else if (scanner->try_consume_string(scanner, "void"))
	{
		type |= TYPE_VOID;
	}
	else if (scanner->try_consume_string(scanner, "union"))
	{
		type |= TYPE_UNION;

		scanner->move_next_token(scanner); // todo crz: dumping unions for now;
	}
	else if (scanner->try_consume_string(scanner, "struct"))
	{
		type |= TYPE_STRUCT;

		scanner->move_next_token(scanner); // todo crz: dumping structs for now;
	}
	else
	{
		scanner->set_token(scanner, save);
		scanner->state = save_state;
		return type;
	}

	if (scanner->try_consume_string(scanner, "const")) type |= TYPE_CONST;
	if (scanner->try_consume_token(scanner, TOKEN_ASTERISK)) type |= TYPE_POINTER;
	if (scanner->try_consume_token(scanner, TOKEN_ASTERISK)) type |= TYPE_POINTER2;

	return type;
}
