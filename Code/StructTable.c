#include"common.h"
#include"StructTable.h"

bool CheckStructTable(struct CharactInfoEntry_Struct* p)
{
	int i = 0 ; 
	for(; i < 100 ; i++)
	{
		if(StructTable[i].valid == 1)
#ifdef DEBUG
			if(!(strcmp(StructTable[i].Struct_name , p->Struct_name)))
				fprintf(stderr ,"Error type at Line %d : xxxx" , p->lineNumber);
#endif
#ifndef DEBUG
			if(IsHomoStruct(StructTable[i].entry , p->entry))
			{
				return FALSE;
			}
#endif
	}
	return TRUE;
}

bool IsHomoStruct(FieldList target , FieldList origin)
{
	if((origin == NULL) ^ (target == NULL))
		return FALSE;
	if(origin == NULL && target == NULL)
		return TRUE;

	if(!IsHomoType(target->type , origin->type))
		return FALSE;
	return IsHomoStruct(target->next , origin->next);

}

bool IsHomoType(Type target , Type origin)
{
	if(target->kind != origin->kind)
		return FALSE;
	if(origin->kind == BASIC)
	{
		if(target->u.basic != origin->u.basic)
			return FALSE;
	}
	else if(origin->kind == ARRAY)
	{
		if(target->u.array.size != origin->u.array.size)
			return FALSE;
		return IsHomoType(target->u.array.elem , origin->u.array.elem);

	}
	else if(origin->kind == STRUCT)
		return IsHomoStruct(target->u.structure , origin->u.structure);
	return FALSE;

}

bool IsSameInStruct(FieldList origin , char* name)
{
	if(origin == NULL)
		return FALSE;
	if(!strcmp(origin->name , name))
		return TRUE;
	return IsSameInStruct(origin->next , name);
}

void WriteStructTable(FieldList p , char* name)
{
	int i = 0;
	for(; i < 100 ; i++)
	{
		if(StructTable[i].valid == 0)
			break;
	}

	/*TODO*/
	/* to make sure that the array is not full DO something*/
	StructTable[i].valid = 1;

	if(name == NULL)
		StructTable[i].Struct_name = NULL;
	else
	{
		int length = strlen(name);
		StructTable[i].Struct_name = (char*)malloc(sizeof(char) * (length + 1));
		strcpy(StructTable[i].Struct_name , name);
	}

	StructTable[i].entry = p;
}

FieldList FindStruct(char* name)
{
	int i = 0;
	for(; i < 100 ; i++)
	{
		if(StructTable[i].valid == 1)
		{
			if(StructTable[i].Struct_name == NULL)
				continue;
			else if(!strcmp(StructTable[i].Struct_name , name))
				return StructTable[i].entry;
		}
	}
	return NULL;
}

