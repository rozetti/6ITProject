/*****************************************************************************
The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org.
******************************************************************************
The MIT License (MIT)

Copyright(c) 2015 Conrad Rozetti

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#pragma once

#include "6IT.h"

#define DEBUG_SHOW_TOKENS(S) ((S)->verbosity >= 1)

enum
{
	ERROR_SCANNER_NO_ERROR = SCANNER_ERRORS,

	ERROR_SCANNER_UNEXPECTED_TOKEN,
	ERROR_SCANNER_UNEXPECTED_TOKEN_TYPE,

	ERROR_SCANNER_CHAR_LITERAL_MISSING_INITIAL_QUOTE,
	ERROR_SCANNER_CHAR_LITERAL_MISSING_CLOSING_QUOTE,
	ERROR_SCANNER_EOF_IN_CHAR_LITERAL,

	ERROR_SCANNER_STRING_LITERAL_MISSING_INITIAL_QUOTE,
	ERROR_SCANNER_STRING_LITERAL_MISSING_CLOSING_QUOTE,
	ERROR_SCANNER_EOF_IN_STRING_LITERAL,
	ERROR_SCANNER_EOL_IN_STRING_LITERAL,
	ERROR_SCANNER_STRING_STORE_OVERFLOW,

	ERROR_SCANNER_IDENTIFIER_EXPECTED,
	ERROR_SCANNER_WRONG_IDENTIFIER,
	ERROR_SCANNER_UNRECOGNISED_TOKEN,
	ERROR_SCANNER_TOKEN_STRING_NOT_FOUND,
	ERROR_SCANNER_TOKENISER_GONE_WRONG,
	ERROR_SCANNER_EXTERNAL_ERROR
};