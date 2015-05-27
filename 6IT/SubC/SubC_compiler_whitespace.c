/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"

//#include "SubC.h"
//#include "6IL_opcodes.h"

static void move_past_multiline_comment(struct scanner_t *scanner)
{
	while (!scanner->matches_string(scanner, "*/"))
	{
		if (!scanner->move_next_char(scanner))
		{
			THROW(&scanner->exception, 1);
			return;
		}
	}

	scanner->move_next_char(scanner);
	scanner->move_next_char(scanner);
}

static void move_past_comments(struct scanner_t *scanner)
{
	if (scanner->is_char(scanner, '/'))
	{
		if (!scanner->move_next_char(scanner))
		{
			THROW(&scanner->exception, 1);
			return;
		}

		if (scanner->is_char(scanner, '*'))
		{
			move_past_multiline_comment(scanner);
		}
		else if (scanner->is_char(scanner, '/'))
		{
			scanner->move_to_end_of_line(scanner);
		}
		else
		{
			scanner->move_previous_char(scanner);
			return;
		}
	}
}

void override_move_past_whitespace(struct scanner_t *scanner)
{
	while (!scanner->is_eof(scanner))
	{
		int offset = scanner->state.source_offset;

		scanner->base->move_past_whitespace(scanner);
		move_past_comments(scanner);

		if (offset == scanner->state.source_offset) break;
	}
}

