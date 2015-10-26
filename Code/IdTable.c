#include"common.h"
#include"IdTable.h"

bool IsSameName(char *name)
{
	int i = 0;
	for(; i < 10 ; i++)
	{
		if(IdTable[i].valid == 1)
			if(!strcmp(IdTable[i].Id_name , name))
				return TRUE;
	}
	return FALSE;
}

void WriteIdTable(Type inh , char* name)
{
	int i = 0;
	for(; i < 10 ; i++)
	{
		if(IdTable[i].valid == 0)
			break;	
	}

	/* to make sure that the array is not full Do something*/

	IdTable[i].valid = 1;
	
	int length = strlen(name);
	IdTable[i].Id_name = (char*)malloc(sizeof(char) * (length + 1));
	strcpy(IdTable[i].Id_name , name);

	FillType(IdTable[i].type , inh);
}


