/* Flow: Declaration attributes */

#include "picoc.h"
#include "interpreter.h"

/* Parse declaration attributes:
 * 
 * [name(value, value, value...), ...] type ident ... ; */

int
TypeParseAttributes(struct ParseState *Parser)
{
	enum LexToken Token;

	do
	{
		Token = LexGetToken(Parser, NULL, TRUE);
		if (Token == TokenEOF)
		{
			ProgramFail(Parser, "unexpected end of file within attribute list");
		}
		if (Token == TokenRightSquareBracket)
		{
			break;
		}
	}
	while (Token != TokenRightSquareBracket && Token != TokenEOF);

	return 0;
}
