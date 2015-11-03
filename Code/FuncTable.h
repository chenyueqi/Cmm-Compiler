#include"Seman.h"

#ifndef __FUNC__
#define __FUNC__

struct CharactInfoEntry_Func
{
	int valid;
	char* func_name;
	Type return_type;
	int para_amount;
	FieldList parameter;
};

struct CharactInfoEntry_Func FuncTable[10];

int WriteFuncTable(char* name , Type return_type , int para_amount , FieldList parameter);

bool IsSameFuncName(char* name);

int FindFunc(char* name);

#endif
