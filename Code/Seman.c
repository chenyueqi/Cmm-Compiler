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
	return CurrentExtDefList(p->children[0]);//Program -> ExtDefList
}

void CurrentExtDefList(struct tree_node* p)
{
	if(p == NULL)//ExtDefList -> NULL
		return;
	else // ExtDefList -> ExtDef ExtDefList
	{
		CurrentExtDef(p->children[0]);
		CurrentExtDefList(p->children[1]);
	}
}

void CurrentExtDef(struct tree_node* p)
{
	Type inh = CurrentSpecifier(p->children[0]);
	if(inh == NULL)
		return ;

	if(p->children_num == 3)
	{

		if(!strcmp(p->children[1]->token_name , "ExtDecList"))//ExtDef -> Specifier ExtDecList SEMI
			CurrentExtDecList(inh , p->children[1]);
		else if(!strcmp(p->children[1]->token_name , "FunDec"))//ExtDef -> Specifer FunDec SEMI
			CurrentFunDec(inh , p->children[1]);
			/* TODO */
	}
	else if(p->children_num == 2){}//ExDef -> Specifier SEMI
}

void CurrentExtDecList(Type inh , struct tree_node* p)
{
	CurrentVarDec(inh , p->children[0] , FALSE , NULL);// CurrentExtDecList -> VarDec

	if(p->children_num == 3)// CurrentExtDecList -> VarDec COMMA ExtDecList
		CurrentExtDecList(inh , p->children[2]);
}

Type CurrentSpecifier(struct tree_node* p)
{
	if(!strcmp(p->children[0]->token_name , "TYPE"))//Specifier -> TYPE
	{
		Type target = (Type)malloc(sizeof(struct Type_));
		target->kind = BASIC;
		if(!strcmp(p->children[0]->unit_name , "int"))
			target->u.basic = 0;
		else if(!strcmp(p->children[0]->unit_name , "float"))
			target->u.basic = 1;
		return target;
	}
	else if(!strcmp(p->children[0]->token_name , "StructSpecifier"))//Specifier -> StructSpecifier
	{
		Type target =  CurrentStructSpecifier(p->children[0]);
		return target;
	}
	return NULL;
}


Type CurrentStructSpecifier(struct tree_node* p)
{
	Type target = (Type)malloc(sizeof(struct Type_));
	target->kind = STRUCT;

	if(p->children_num == 5)// StructSpecifier -> STRUCT OptTag LC DefList RC
	{
		char* name = CurrentOptTag(p->children[1]);
		if(IsSameName(name) && name != NULL)
		{
			fprintf(stderr , "Error type 16 at Line %d : Redefined struct name  '%s'\n" , p->lineno , name);
			return NULL;
		}
		else
		{
			target->u.structure = CurrentDefList(p->children[3] , TRUE);
			WriteStructTable(target->u.structure , name);
		}
	}
	else if(p->children_num == 2)//StructSpecifier -> STRUCT Tag
	{
		char* name = CurrentTag(p->children[1]);
		target->u.structure = FindStruct(name);
		if (target->u.structure == NULL)
		{
			fprintf(stderr , "Error type 17 at Line %d : Undefined struct '%s'\n" , p->lineno , name);
			return NULL;
		}
	}
	return target;
}


char* CurrentOptTag(struct tree_node* p)
{
	if(p == NULL)//OptTag -> e
		return NULL;
	else 
		return p->children[0]->unit_name;//OptTag -> ID
}
	
char* CurrentTag(struct tree_node* p)
{
	return p->children[0]->unit_name;//Tag -> ID
}

Type CurrentVarDec(Type inh , struct tree_node* p , bool InStructure , char *name)
{
	if(p->children_num == 1)//VarDec -> ID
	{
		if(IsSameName(p->children[0]->unit_name))
		{
			fprintf(stderr , "Error type 3 at Line %d : Redefined variable '%s'\n" , p->lineno , p->children[0]->unit_name);
			return NULL;
		}
		else
		{
			if(InStructure)
			{
				int length = strlen(p->children[0]->unit_name);
				name = (char*)malloc(sizeof(char) * (length + 1));
				strcpy(name , p->children[0]->unit_name);;
				printf("%s %s %d\n" , name , __FILE__ , __LINE__);
			}
			WriteIdTable(inh , p->children[0]->unit_name);
			return inh;
		}
	}
	else if(p->children_num == 4)//VarDec -> VarDec LB INT RB
	{
		Type target = (Type)malloc(sizeof(struct Type_));
		target->kind = ARRAY;
		target->u.array.elem = inh;
		target->u.array.size = atoi(p->children[2]->unit_name);
		return CurrentVarDec(target , p->children[0] , InStructure , name);
	}
	return NULL;
}

void CurrentFunDec(Type inh , struct tree_node* p)
{
	if(p->children_num == 4)//FunDec -> ID LP VarList RP
	{
		char* name = p->children[0]->unit_name;
		if(IsSameFuncName(name))
		{
			fprintf(stderr , "Error type 4 at Line %d : Redefined function\n" , p->lineno);
		}
		else
		{	
			Type return_type = inh;
			int para_amount = 0;
			FieldList parameter = CurrentVarList(p->children[2] , &para_amount);
			WriteFuncTable(name , return_type , para_amount , parameter);
		}
	}
	else if(p->children_num == 3)//FunDec -> ID LP RP
	{
		char* name = p->children[0]->unit_name;
		if(IsSameFuncName(name))
		{
			fprintf(stderr , "Error type 4 at Line %d : Redefined function\n" , p->lineno);
		}
		else
		{
			Type return_type = inh;
			int para_amount = 0;
			WriteFuncTable(name , return_type , para_amount , NULL);
		}
		/*TODO*/
	}
}

FieldList CurrentVarList(struct tree_node* p , int* para_amount)
{
	FieldList target = (FieldList)malloc(sizeof(struct FieldList_));
	target->name = NULL;
	target->type = CurrentParamDec(p->children[0]);

	if(target->type != NULL)//current ParamDec is valid
	{
	
		(*para_amount)++;
		if(p->children_num == 3)//VarList -> ParamDec COMMA VarList
			target->next = CurrentVarList(p->children[2] , para_amount);
		else 
			target->next = NULL;//VarList -> ParamDec
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
	Type target = CurrentVarDec(inh , p->children[1] , FALSE , NULL);//ParamDec -> Specifier VarDec
	return target;
}




FieldList CurrentDefList(struct tree_node* p , bool InStructure)
{
	if(p == NULL)//DefList -> e
		return NULL;
	else//DefList -> Def DefList
	{
		FieldList target = (FieldList)malloc(sizeof(struct FieldList_)) ; 

		Type type = CurrentDef(p->children[0] , InStructure , target->name);
		printf("%s %s %d\n" , target->name , __FILE__ , __LINE__);
		if(type == NULL)
			target = CurrentDefList(p->children[1] , InStructure);
		else
		{
			target->type = type;
			target->next = CurrentDefList(p->children[1] , InStructure);
		}
		return target;
	}
}

Type CurrentDef( struct tree_node* p , bool InStructure ,  char* name)
{
	Type target = CurrentSpecifier(p->children[0]);
	if(target == NULL)
		return NULL;
	Type inh = target;
	CurrentDecList(inh , p->children[1] , InStructure  , name);//Def -> Specifier DecList SEMI
	return target;
}

void CurrentDecList(Type inh , struct tree_node* p , bool InStructure  , char* name)
{
	CurrentDec(inh , p->children[0] , InStructure  , name);//DecList -> Dec
	
	if(p->children_num == 3)//DecList -> Dec COMMA DecList
		CurrentDecList(inh , p->children[2] , InStructure  , name);
}

void CurrentDec(Type inh , struct tree_node* p , bool InStructure , char* name) 
{
	if(p->children_num == 1)//Dec -> VarDec
	{
		CurrentVarDec(inh , p->children[0] , InStructure  , name);
	}
	else if(p->children_num == 3)//Dec -> VarDec ASSIGNOP Exp
	{
		if(InStructure)
			fprintf(stderr , "Error type 15 at Line %d : Try to assign in struct definition\n" , p->lineno);
		else
		{
		/*TODO*/
		}
	}
}




