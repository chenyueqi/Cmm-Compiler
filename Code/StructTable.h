#include"Seman.h"
#ifndef __STRUCT__
#define __STRUCT__

struct CharactInfoEntry_Struct
{
	int valid;
	char* Struct_name;
	FieldList entry;
};

struct CharactInfoEntry_Struct StructTable[10];

bool CheckStructTable(struct CharactInfoEntry_Struct* p);

FieldList FindStruct(char* name);

bool IsHomoStruct(FieldList target , FieldList origin);

bool IsHomoType(Type target , Type origin);

void WriteStructTable(FieldList p , char* name);

bool IsSameInStruct(FieldList origin , char* name);

#endif
