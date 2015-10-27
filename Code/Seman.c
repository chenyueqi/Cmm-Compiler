#include"common.h"
#include"Seman.h"
#include"StructTable.h"
#include"IdTable.h"
#include"FuncTable.h"


void Seman_analysis(struct tree_node* p)
{
	CurrentProgram(p);
	//display_tree(p , 0);
}

/*void FillFieldList(FieldList target , FieldList origin)
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
*/
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
	{
		target->u.structure = origin->u.structure;
	}
}

bool IsSameName(char* name)
{
	int i = 0;
	for(; i < 10 ; i++)//check Id Table
	{
		if(IdTable[i].valid == 1)
			if(!strcmp(IdTable[i].Id_name , name))
				return TRUE;
	}

	for(; i < 10 ; i++)//check Struct Table
	{
		if(StructTable[i].valid == 1)
		{
			if(StructTable[i].Struct_name == NULL)
				continue;
			if(!strcmp(StructTable[i].Struct_name , name))
				return TRUE;
		}
	}
	return FALSE;

}

void CurrentProgram(struct tree_node* p)
{
	return CurrentExtDefList(p->children[0]);
}

void CurrentExtDefList(struct tree_node* p)
{
	if(p == NULL)
		return;
	else 
	{
		CurrentExtDef(p->children[0]);
		CurrentExtDefList(p->children[1]);
	}
}

void CurrentExtDef(struct tree_node* p)
{
	Type inh = CurrentSpecifier(p->children[0]);

	if(p->children_num == 3)
	{

		if(!strcmp(p->children[1]->token_name , "ExtDecList"))
			CurrentExtDecList(inh , p->children[1]);
		else if(!strcmp(p->children[1]->token_name , "FunDec"))
			CurrentFunDec(inh , p->children[1]);
			/* TODO */
	}
}

Type CurrentSpecifier(struct tree_node* p)
{
	if(!strcmp(p->children[0]->token_name , "TYPE"))
	{
		Type target = (Type)malloc(sizeof(struct Type_));
		target->kind = BASIC;
		if(!strcmp(p->children[0]->unit_name , "int"))
			target->u.basic = 0;
		else if(!strcmp(p->children[0]->unit_name , "float"))
			target->u.basic = 1;
		return target;
	}
	else if(!strcmp(p->children[0]->token_name , "StructSpecifier"))
	{
		Type target =  CurrentStructSpecifier(p->children[0]);
		return target;
	}
	return NULL;
	
}

void CurrentExtDecList(Type inh , struct tree_node* p)
{
	if(p->children_num == 1)
	{
		CurrentVarDec(inh , p->children[0] , FALSE);

	}
	else if(p->children_num == 3)
	{
		CurrentExtDecList(inh , p->children[2]);
	}
}


Type CurrentStructSpecifier(struct tree_node* p)
{
	Type target = (Type)malloc(sizeof(struct Type_));
	target->kind = STRUCT;

	if(p->children_num == 5)
	{
		char* name = CurrentOptTag(p->children[1]);
		if(IsSameName(name))
			fprintf(stderr , "Error type 16 at Line %d : Redefined struct name  '%s'\n" , p->lineno , name);
		else
		{
			target->u.structure = CurrentDefList(p->children[3] , TRUE);
			WriteStructTable(target->u.structure , name);
		}
	}
	else if(p->children_num == 2)
	{
		char* name = CurrentTag(p->children[1]);
		target->u.structure = FindStruct(name);
		if (target->u.structure == NULL)
			fprintf(stderr , "Error type 17 at Line %d : Undefined struct '%s'\n" , p->lineno , name);
	}
	return target;

}

char* CurrentTag(struct tree_node* p)
{
	return p->children[0]->unit_name;
}

char* CurrentOptTag(struct tree_node* p)
{
	if(p == NULL)
		return NULL;
	else 
		return p->children[0]->unit_name;
}
	

Type CurrentVarDec(Type inh , struct tree_node* p , bool InStructure)
{
	if(p->children_num == 1)
	{
		if(IsSameName(p->children[0]->unit_name))
		{
			if(InStructure)
				fprintf(stderr , "Error type 15 at Line %d : Redefined variable '%s' in struct\n" , p->lineno , p->children[0]->unit_name);
			else
				fprintf(stderr , "Error type 3 at Line %d : Redefined variable '%s'\n" , p->lineno , p->children[0]->unit_name);
			return NULL;
		}
		else
		{
			WriteIdTable(inh , p->children[0]->unit_name);
			return inh;
		}
	}
	else if(p->children_num == 4)
	{
		Type target = (Type)malloc(sizeof(struct Type_));
		target->kind = ARRAY;
		target->u.array.elem = inh;
		target->u.array.size = atoi(p->children[2]->unit_name);
		return CurrentVarDec(target , p->children[0] , InStructure);
	}
	return NULL;
}

FieldList CurrentDefList(struct tree_node* p , bool InStructure)
{
	if(p == NULL)
		return NULL;
	else
	{
		FieldList target = (FieldList)malloc(sizeof(struct FieldList_)) ; 
		target->type = CurrentDef(p->children[0] , InStructure);
		target->next = CurrentDefList(p->children[1] , InStructure);

		return target;
	}

}

Type CurrentDef( struct tree_node* p , bool InStructure)
{
	Type target = CurrentSpecifier(p->children[0]);

	Type inh = target;

	CurrentDecList(inh , p->children[1] , InStructure);

	return target;
}

void CurrentDecList(Type inh , struct tree_node* p , bool InStructure)
{
	CurrentDec(inh , p->children[0] , InStructure);
	
	if(p->children_num == 3)
		CurrentDecList(inh , p->children[2] , InStructure);
}

void CurrentDec(Type inh , struct tree_node* p , bool InStructure)
{
	if(p->children_num == 1)
	{
		CurrentVarDec(inh , p->children[0] , InStructure);

	}
	else if(p->children_num == 3)
	{
		if(InStructure)
			fprintf(stderr , "Error type 15 at Line %d : Try to assign in struct definition\n" , p->lineno);
		else
		{

		/*TODO*/
		}

	}

}

void CurrentFunDec(Type inh , struct tree_node* p)
{
	if(p->children_num == 4)
	{
		char* name = p->children[0]->unit_name;
		if(IsSameFuncName(name))
		{
			fprintf(stderr , "Error type 4 at Line %d : Refined function\n" , p->lineno);
		}
		else
		{	
			Type return_type = inh;
			int para_amount = 0;
			FieldList parameter = CurrentVarList(p->children[2] , &para_amount);
			WriteFuncTable(name , return_type , para_amount , parameter);
		}
	}
	else if(p->children_num == 3)//call Function
	{
		/*TODO*/
	}
}

FieldList CurrentVarList(struct tree_node* p , int* para_amount)
{
	FieldList target = (FieldList)malloc(sizeof(struct FieldList_));
	target->name = NULL;
	target->type = CurrentParamDec(p->children[0]);

	if(target->type != NULL)
	{
	
		(*para_amount)++;
		if(p->children_num == 3)
			target->next = CurrentVarList(p->children[2] , para_amount);
		else 
			target->next = NULL;
	}
	else
	{
		if(p->children_num == 3)
			target = CurrentVarList(p->children[2] , para_amount);
		else
			target = NULL;
	}
	return target;
}

Type CurrentParamDec(struct tree_node* p)
{
	Type inh =  CurrentSpecifier(p->children[0]);
	Type target = CurrentVarDec(inh , p->children[1] , FALSE);
	return target;
}

