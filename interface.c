/* Flow: parse interface definitions */

#include "picoc.h"
#include "interpreter.h"

/* parse a struct or union declaration */
void
TypeParseInterface(struct ParseState *Parser, struct ValueType **Typ)
{
	struct Value *LexValue, *MemberValue;
	struct ValueType *MemberType;
	char   *MemberIdentifier, *StructIdentifier;
	enum LexToken Token;
	int	AlignBoundary;

	Picoc  *pc = Parser->pc;

	Token = LexGetToken(Parser, &LexValue, FALSE);
	if (Token == TokenIdentifier)
	{
		LexGetToken(Parser, &LexValue, TRUE);
		StructIdentifier = LexValue->Val->Identifier;
		Token = LexGetToken(Parser, NULL, FALSE);
	}
	else
	{
		ProgramFail(Parser, "interface definitions may not be anonymous");
	}

	*Typ = TypeGetMatching(pc, Parser, &Parser->pc->UberType, TypeInterface, 0, StructIdentifier, TRUE);
	if (Token == TokenLeftBrace && (*Typ)->Members != NULL)
	{
		ProgramFail(Parser, "interface '%t' is already defined", *Typ);
	}

	Token = LexGetToken(Parser, NULL, FALSE);
	if (Token != TokenLeftBrace)
	{
		/* use the already defined structure */
		return;
	}

	if (pc->TopStackFrame != NULL)
	{
		ProgramFail(Parser, "interface definitions must be global");
	}

	LexGetToken(Parser, NULL, TRUE);
	(*Typ)->Members = VariableAlloc(pc, Parser, sizeof(struct Table) + STRUCT_TABLE_SIZE * sizeof(struct TableEntry), TRUE);
	(*Typ)->Members->HashTable = (struct TableEntry **)((char *)(*Typ)->Members + sizeof(struct Table));
	TableInitTable((*Typ)->Members, (struct TableEntry **)((char *)(*Typ)->Members + sizeof(struct Table)), STRUCT_TABLE_SIZE, TRUE);

	do
	{
		TypeParse(Parser, &MemberType, &MemberIdentifier, NULL);
		if (MemberType == NULL || MemberIdentifier == NULL)
			ProgramFail(Parser, "invalid type in interface definition");

		MemberValue = VariableAllocValueAndData(pc, Parser, sizeof(int), FALSE, NULL, TRUE);
		MemberValue->Typ = MemberType;

		/* allocate this member's location in the struct */
		AlignBoundary = MemberValue->Typ->AlignBytes;
		if (((*Typ)->Sizeof & (AlignBoundary - 1)) != 0)
			(*Typ)->Sizeof += AlignBoundary - ((*Typ)->Sizeof & (AlignBoundary - 1));

		MemberValue->Val->Integer = (*Typ)->Sizeof;
		(*Typ)->Sizeof += TypeSizeValue(MemberValue, TRUE);

		/* make sure to align to the size of the largest member's
		 * alignment */
		if ((*Typ)->AlignBytes < MemberValue->Typ->AlignBytes)
			(*Typ)->AlignBytes = MemberValue->Typ->AlignBytes;

		/* define it */
		if (!TableSet(pc, (*Typ)->Members, MemberIdentifier, MemberValue, Parser->FileName, Parser->Line, Parser->CharacterPos))
			ProgramFail(Parser, "interface member '%s::%s' already defined", StructIdentifier, MemberIdentifier);

		if (LexGetToken(Parser, NULL, TRUE) != TokenSemicolon)
			ProgramFail(Parser, "semicolon expected");

	} while (LexGetToken(Parser, NULL, FALSE) != TokenRightBrace);
	/* now align the structure to the size of its largest member's
	 * alignment */
	AlignBoundary = (*Typ)->AlignBytes;
	if (((*Typ)->Sizeof & (AlignBoundary - 1)) != 0)
		(*Typ)->Sizeof += AlignBoundary - ((*Typ)->Sizeof & (AlignBoundary - 1));

	LexGetToken(Parser, NULL, TRUE);
}
