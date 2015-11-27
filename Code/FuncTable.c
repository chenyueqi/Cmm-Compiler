#include"common.h"
#include"FuncTable.h"

int WriteFuncTable(char* name , Type return_type , int para_amount , FieldList parameter)
{
	int i = 0;
	for(; i < 10 ; i++)
	{
		if(FuncTable[i].valid == 0)
			break;
	}

	FuncTable[i].valid = 1;

	FuncTable[i].return_type = return_type;
	FuncTable[i].para_amount = para_amount;
	FuncTable[i].parameter = parameter;

	int length = strlen(name);
	FuncTable[i].func_name = (char*)malloc(sizeof(char) * (length + 1));
	strcpy(FuncTable[i].func_name , name);
	return i;
}

bool IsSameFuncName(char* name)
{
	int i = 0;
	for(; i < 10 ; i++)
	{
		if(FuncTable[i].valid == 1)
		{
			if(!strcmp(FuncTable[i].func_name , name))
				return TRUE;
		}
	}
	return FALSE;
}

int FindFunc(char* name)
{
	int i = 0;
	for(; i < 10 ; i++)
	{
		if(FuncTable[i].valid == 1)
			if(!strcmp(FuncTable[i].func_name , name))
				return i;
	}
	return -1;
}

struct CharactInfoEntry_Func* lookup_functable(char* func_name)
{
	int i = 0;
	for( ; i < 10 ; i++)
	{
		if(FuncTable[i].valid == 1)
			if(!strcmp(FuncTable[i].func_name , func_name))
				return &FuncTable[i];
	}
	return NULL;

}
