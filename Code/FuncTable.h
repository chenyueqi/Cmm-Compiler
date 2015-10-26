#include"Seman.h"

struct CharactInfoEntry_Func
{
	int valid;
	char* func_name;
	int lineNumber;
	Type return_type;
	int para_amount;
	Type* parameter;
};

struct CharactInfoEntry_Func FuncTable[10];

