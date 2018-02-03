/* Flow: Module definitions */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "picoc.h"
#include "interpreter.h"

/* Parse a module definition
 * 
 * Modules are exported sets of functions which can be invoked via RPC but aren't
 * instances of classes. */
void
TypeParseModule(struct ParseState *Parser, struct ValueType **Typ)
{
	struct ParseState after;
	struct Value *LexValue, *MemberValue;
	struct ValueType *MemberType;
	char   *MemberIdentifier, *StructIdentifier;
	enum LexToken Token;
	int	AlignBoundary;
	Picoc *pc = Parser->pc;

/*	fprintf(stderr, "%s:%d:%d: processing module\n", Parser->FileName, Parser->Line, Parser->CharacterPos); */
	Token = LexGetToken(Parser, &LexValue, FALSE);
	if (Token == TokenIdentifier)
	{
		LexGetToken(Parser, &LexValue, TRUE);
		StructIdentifier = LexValue->Val->Identifier;
		Token = LexGetToken(Parser, NULL, FALSE);
	}
	else
	{
		ProgramFail(Parser, "module definitions cannot be anonymous");
	}

	*Typ = TypeGetMatching(pc, Parser, &Parser->pc->UberType, TypeModule, 0, StructIdentifier, TRUE);
	if (Token == TokenLeftBrace && (*Typ)->Members != NULL)
	{
		ProgramFail(Parser, "module '%t' is already defined", *Typ);
	}

	Token = LexGetToken(Parser, NULL, FALSE);
	if (Token != TokenLeftBrace)
	{
		/* use the already defined structure */
		return;
	}
	/* Swallow the brace */
	LexGetToken(Parser, NULL, TRUE);

	if (pc->TopStackFrame != NULL)
	{
		ProgramFail(Parser, "module definitions must be global");
	}
	/* XXX we shouldn't need this for modules */
	(*Typ)->Members = VariableAlloc(pc, Parser, sizeof(struct Table) + STRUCT_TABLE_SIZE * sizeof(struct TableEntry), TRUE);
	(*Typ)->Members->HashTable = (struct TableEntry **)((char *)(*Typ)->Members + sizeof(struct Table));
	TableInitTable((*Typ)->Members, (struct TableEntry **)((char *)(*Typ)->Members + sizeof(struct Table)), STRUCT_TABLE_SIZE, TRUE);

	/* Enter a scope for the module */
	VariableStackFrameAdd(Parser, StructIdentifier, 0);
	pc->TopStackFrame->ScopeType = *Typ;
	(*Typ)->Scope = pc->TopStackFrame;
	do
	{
		Token = LexGetToken(Parser, NULL, FALSE);
		if (Token == TokenRightBrace)
		{
			break;
		}
		if (Token == TokenSemicolon)
		{
			/* Swallow the semicolon and move on */
			LexGetToken(Parser, NULL, TRUE);
			continue;
		}
		TypeParse(Parser, &MemberType, &MemberIdentifier, NULL);
		if (MemberType == NULL || MemberIdentifier == NULL)
			ProgramFail(Parser, "invalid type in module definition");

		Token = LexGetToken(Parser, NULL, TRUE);
		/* XXX remove */
		if (Token == TokenSemicolon)
		{
			MemberValue = VariableAllocValueAndData(pc, Parser, sizeof(int), FALSE, NULL, TRUE);
			MemberValue->Typ = MemberType;

			/* allocate this member's location in the struct */
			AlignBoundary = MemberValue->Typ->AlignBytes;
			if (((*Typ)->Sizeof & (AlignBoundary - 1)) != 0)
				(*Typ)->Sizeof += AlignBoundary - ((*Typ)->Sizeof & (AlignBoundary - 1));

			MemberValue->Val->Integer = (*Typ)->Sizeof;
			(*Typ)->Sizeof += TypeSizeValue(MemberValue, TRUE);

			/* make sure to align to the size of the largest
			 * member's alignment */
			if ((*Typ)->AlignBytes < MemberValue->Typ->AlignBytes)
				(*Typ)->AlignBytes = MemberValue->Typ->AlignBytes;

			/* define it */
			if (!TableSet(pc, (*Typ)->Members, MemberIdentifier, MemberValue, Parser->FileName, Parser->Line, Parser->CharacterPos))
				ProgramFail(Parser, "module member '%s::%s' already defined", StructIdentifier, MemberIdentifier);
			continue;
		}
		if (Token == TokenOpenBracket)
		{
/*			fprintf(stderr, "%s:%d:%d: processing function definition %s::%s\n", Parser->FileName, Parser->Line, Parser->CharacterPos, StructIdentifier, MemberIdentifier); */
			
			ParseFunctionDefinition(Parser, MemberType, MemberIdentifier);
			continue;
		}
		if (Token != TokenSemicolon)
		{
			ProgramFail(Parser, "expected: semicolon (end of member declaration)");
			continue;
		}
	} while (Token != TokenRightBrace);
	LexGetToken(Parser, NULL, TRUE);
	ParserCopy(&after, Parser);
/*	fprintf(stderr, "%s:%d:%d: module %s done\n", Parser->FileName, Parser->Line, Parser->CharacterPos, StructIdentifier); */
	VariableStackFramePop(Parser);
	ParserCopy(Parser, &after);
	/* Swallow the brace */
	
	/* now align the structure to the size of its largest member's
	 * alignment */
	AlignBoundary = (*Typ)->AlignBytes;
	if (((*Typ)->Sizeof & (AlignBoundary - 1)) != 0)
		(*Typ)->Sizeof += AlignBoundary - ((*Typ)->Sizeof & (AlignBoundary - 1));
}
