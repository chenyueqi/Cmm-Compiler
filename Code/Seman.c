#include"common.h"
#include"Seman.h"
#include"StructTable.h"
#include"IdTable.h"


void Seman_analysis(struct tree_node* p)
{
#ifdef DEBUG
	printf("here is semantic analysis !\n");
#endif

	if(!strcmp(p->token_name , "ExtDef"))
	{
		CurrentExtDef(p);
		return ;
	}

	if(p->type == 1)
	{
		return ;
	}
	int i = 0;
	for(i = 0 ; i < p->children_num ; i++)
	{
		if(p->children[i] == NULL)
			continue;
		Seman_analysis(p->children[i]);
	}
	
	//display_tree(p , 0);
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

void CurrentExtDef(struct tree_node* p)
{
	if(p->children_num == 2)
	{
		//check the Specifier is defined or not
	}
	else if(p->children_num == 3)
	{
		Type inh = CurrentSpecifier(p->children[0]);

		if(!strcmp(p->children[1]->token_name , "ExtDecList"))
			CurrentExtDecList(inh , p->children[1]);
		else if(!strcmp(p->children[1]->token_name , "FunDec"))
		{
			/* DO */

		}

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
	}
	else if(!strcmp(p->children[0]->token_name , "StructSpecifier"))
	{
		Type target =  CurrentStructSpecifier(p->children[0]);
	}
	
	return target;
}

void CurrentExtDecList(Type inh , struct tree_node* p)
{
	if(p->children_num == 1)
	{
		CurrentVarDec(inh , p->children[0]);

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
		target->u.structure = CurrentDefList(p->children[3]);
	}
	else if(p->children_num == 2)
	{

	}

}

void CurrentVarDec(Type inh , struct tree_node* p)
{
	if(IsSameName(p->children[0]->unit_name))
		fprintf(stderr , "Error type 3 at Line %d : Redefined variable '%s'\n" , p->lineno , p->children[0]->unit_name);
	else
	{
		WriteIdTable(inh , p->children[0]->unit_name);
	}
}

FieldList CurrentDefList(struct tree_node* p)
{


}
