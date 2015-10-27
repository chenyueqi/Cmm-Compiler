#include"Seman.h"

#ifndef __ID__
#define __ID__
struct CharactInfoEntry_Id
{
	int valid;
	char* Id_name;
	Type type;
};

struct CharactInfoEntry_Id IdTable[10];

bool CheckIdTable_name(char* name);

void WriteIdTable(Type inh , char *name);

#endif
