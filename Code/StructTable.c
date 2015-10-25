#include"common.h"
#include"StructTable.h"

bool CheckStructTable(struct CharactInfoEntry_Struct* p)
{
	int i = 0 ; 
	for(; i < 10 ; i++)
	{
		if(StructTable[i].valid == 1)
#ifdef DEBUG
			if(!(strcmp(StructTable[i].Struct_name , p->Struct_name)))
				fprintf(stderr ,"Error type at Line %d : xxxx" , p->lineNumber);
#endif
#ifndef DEBUG
			if(IsHomoStruct(StructTable[i].entry , p->entry))
				return FALSE;
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
		if(target->u.basic != origin->u.basic)
			return FALSE;
	else if(origin->kind == ARRAY)
	{
		if(target->u.array.size != origin->u.array.size)
			return FALSE;
		return IsHomoType(target->u.array.elem , origin->u.array.elem);

	}
	else if(origin->kind == STRUCT)
		return IsHomoStruct(target->u.structure , origin->u.structure);

}

void WriteStructTable(struct CharactInfoEntry_Struct* p , int lineNumber)
{
	int i = 0;
	for(; i < 10 ; i++)
	{
		if(StructTable[i].valid == 0)
			break;
	}

	/* to make sure that the array is not full DO something*/

	StructTable[i].valid = 1;
	StructTable[i].lineNumber = p->lineNumber;

	int length = strlen(p->Struct_name);
	StructTable[i].Struct_name = (char*)malloc(sizeof(char) * (length + 1));
	strcpy(StructTable[i].Struct_name , p->Struct_name);

	FillFieldList(StructTable[i].entry , p->entry);
}

void FillFieldList(FieldList target , FieldList origin)
{
	if(origin == NULL)
	{
		target = NULL;
		return;
	}

	target = (FieldList)malloc(sizeof(struct FieldList_));

	int length = strlen(origin->name);
	target->name = (char*)malloc(sizeof(char) * (length + 1));
	strcpy(target->name , origin->name);

	FillType(target->type , origin->type);

	FillFieldList(target->next , origin->next);
}

void FillType(Type target , Type origin)
{
	if(origin == NULL)
	{
		target = NULL;
		return;	
	}

	target = (Type)malloc(sizeof(struct Type_));

	target->kind = origin->kind;
	if(origin->kind == BASIC)
		target->u.basic = origin->u.basic;
	else if(origin->kind == ARRAY)
	{
		FillType(target->u.array.elem , origin->u.array.elem);
		target->u.array.size = origin->u.array.size;
	}
	else if(origin->kind == STRUCT)
		FillFieldList(target->u.structure , origin->u.structure);
}

