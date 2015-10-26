#include"Seman.h"
#ifndef __STRUCT__
#define __STRUCT__

struct CharactInfoEntry_Struct
{
	int valid;
	int lineNumber;
	char* Struct_name;
	FieldList entry;
};

struct CharactInfoEntry_Struct StructTable[10];

bool CheckStructTable(struct CharactInfoEntry_Struct* p);

bool IsHomoStruct(FieldList target , FieldList origin);

bool IsHomoType(Type target , Type origin);

void WriteStructTable(struct CharactInfoEntry_Struct* p , int lineNumber);

#endif
