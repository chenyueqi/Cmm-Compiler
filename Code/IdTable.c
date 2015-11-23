#include"common.h"
#include"IdTable.h"

extern int variable_num ;

void WriteIdTable(Type inh , char* name)
{
	int i = 0;
	for(; i < 100 ; i++)
	{
		if(IdTable[i].valid == 0)
			break;	
	}

	/* to make sure that the array is not full TODO*/

	IdTable[i].valid = 1;
	
	int length = strlen(name);
	IdTable[i].Id_name = (char*)malloc(sizeof(char) * (length + 1));
	strcpy(IdTable[i].Id_name , name);

	IdTable[i].type = inh;
	IdTable[i].var_no = 0;
}

Type FindId(char* name)
{
	int i = 0;
	for(; i < 100; i++)
	{
		if(IdTable[i].valid == 1)
		{
			if(!strcmp(IdTable[i].Id_name , name))
				return IdTable[i].type;
			else
				continue;
		}
	}
	return NULL;
}

int lookup(char* name)
{
	int i = 0;
	for(; i < 100 ; i++)
	{
		if(IdTable[i].valid == 1)
		{
			if(!strcmp(IdTable[i].Id_name , name))
			{
				if(IdTable[i].var_no == 0)
					IdTable[i].var_no = ++variable_num;
				return IdTable[i].var_no;
			}
			else
				return -1;
		}
	}
}
