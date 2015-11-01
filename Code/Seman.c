#include"common.h"
#include"Seman.h"
#include"StructTable.h"
#include"IdTable.h"
#include"FuncTable.h"
#include<assert.h>

void Seman_analysis(struct tree_node* p)
{
	CurrentProgram(p);
}

bool IsSameName(char* name)
{
	int i = 0;
	for(; i < 100 ; i++)//check Id Table
	{
		if(IdTable[i].valid == 1)
			if(!strcmp(IdTable[i].Id_name , name))
				return TRUE;
	}

	for(; i < 100 ; i++)//check Struct Table
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

bool IsSameType(Type target , Type origin)
{
	if((target == NULL) && (origin == NULL))
		return TRUE;
	else if((target != NULL) && (origin != NULL))
	{
		if(target->kind != origin->kind)
			return FALSE;
		if(target->kind == BASIC)
		{
			if(target->u.basic == origin->u.basic)
				return TRUE;
			else
				return FALSE;
		}
		else if(target->kind == ARRAY)
		{
			if(target->u.array.size != origin->u.array.size)
				return FALSE;
			else if(IsSameType(target->u.array.elem , origin->u.array.elem))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if(IsSameStructure(target->u.structure , origin->u.structure))
				return TRUE;
			else
				return FALSE;
		}
	}
	else
		return FALSE;
}

bool IsSameStructure(FieldList target , FieldList origin)
{
	if( (target == NULL) && (origin == NULL))
		return TRUE;
	else if( (target != NULL) && (origin != NULL))
	{
		if(IsSameType(target->type , origin->type))
			return IsSameStructure(target->next , origin->next);
		else
			return FALSE;
	}
	else
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
		else if(!strcmp(p->children[1]->token_name , "FunDec"))//ExtDef -> Specifer FunDec CompSt
		{
			bool return_right = FALSE;
			CurrentCompSt(inh , p->children[2] , &return_right);
			if(!return_right)
			{
				fprintf(stderr , "Error type 8 at Line %d : unmatched return type\n" , p->lineno);
				return;
			}
			CurrentFunDec(inh , p->children[1]);
		}
	}
	else if(p->children_num == 2)//ExtDef -> Specifier SEMI
		return;
}

void CurrentExtDecList(Type inh , struct tree_node* p)
{
	CurrentVarDec(inh , p->children[0]);// CurrentExtDecList -> VarDec

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
		assert(target != NULL);
		return target;
	}
	else if(!strcmp(p->children[0]->token_name , "StructSpecifier"))//Specifier -> StructSpecifier
	{
		Type target =  CurrentStructSpecifier(p->children[0]);
		return target;
	}
	else
		return NULL;
}


void IsSameNameInStructure(FieldList field)
{
	FieldList current = field;
	while(current != NULL)
	{
		FieldList tail = current->next;
		FieldList pre = tail;
		while(tail != NULL)
		{
			if(!strcmp(current->name , tail->name))
			{
				fprintf(stderr , "Error type 15 at Line %d : Redefined  name '%s' in same structure\n" , tail->lineno , tail->name);
				pre->next = tail->next; 
				free(tail->name);
				free(tail);
				tail = pre->next;
			}
			else
			{
				current = tail;
				tail = tail->next;
			}
		}
		current = current->next;
	}
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
			target->u.structure = CurrentDefList(p->children[3]);
			IsSameNameInStructure(target->u.structure);
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

FieldList CurrentVarDec(Type inh , struct tree_node* p)
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
			FieldList field = (FieldList)malloc(sizeof(struct FieldList_));
			int length = strlen(p->children[0]->unit_name);
			field->name = (char*)malloc(sizeof(char) * (length + 1));
			strcpy(field->name , p->children[0]->unit_name);
			field->type = inh;
			field->lineno = p->lineno;
			return field;
		}
	}
	else if(p->children_num == 4)//VarDec -> VarDec LB INT RB
	{
		Type target = (Type)malloc(sizeof(struct Type_));
		target->kind = ARRAY;
		target->u.array.elem = inh;
		target->u.array.size = atoi(p->children[2]->unit_name);
		return CurrentVarDec(target , p->children[0]);
	}
	else
		return NULL;
}

void CurrentFunDec(Type inh ,struct tree_node* p)
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
	Type type = CurrentSpecifier(p->children[0]);
	CurrentVarDec(type , p->children[1]);
	return type;
}

FieldList CurrentDefList(struct tree_node* p)
{
	if(p == NULL)//DefList -> e
		return  NULL;
	else//DefList -> Def DefList
	{
		FieldList field = CurrentDef(p->children[0]);
		if(field == NULL)
			field  = CurrentDefList(p->children[1]);
		else
		{
			FieldList nextfield = field;
			while(nextfield->next != NULL)
				nextfield = nextfield->next;
			nextfield->next = CurrentDefList(p->children[1]);
		}
		return field;
	}
}

FieldList CurrentDef(struct tree_node* p)
{
	Type type = CurrentSpecifier(p->children[0]);
	if(type == NULL)
		return NULL;
	return CurrentDecList(type , p->children[1]);//Def -> Specifier DecList SEMI
}

FieldList CurrentDecList(Type type , struct tree_node* p)
{
	FieldList field = CurrentDec(type  ,  p->children[0]);//DecList -> Dec
	
	if(p->children_num == 3)//DecList -> Dec COMMA DecList
	{
		if(field != NULL)
			field->next = CurrentDecList(type , p->children[2]);
		else
			field = CurrentDecList(type , p->children[2]);
	}
	return field;
}

FieldList CurrentDec(Type type , struct tree_node* p) 
{
	if(p->children_num == 1)//Dec -> VarDec
	{
		return CurrentVarDec(type , p->children[0]);
	}
	else if(p->children_num == 3)//Dec -> VarDec ASSIGNOP Exp
	{
		fprintf(stderr , "Error type 15 at Line %d : Try to assign in struct definition\n" , p->lineno);
		return NULL;
	}
	else
		return NULL;
}

void CurrentCompSt(Type type , struct tree_node* p , bool* return_right)//CompSt -> LC DefList StmtList Rc
{
	CurrentDefList_1(p->children[1]);//may be something wrong
	CurrentStmtList(type , p->children[2] , return_right);
}

void CurrentStmtList(Type type , struct tree_node* p , bool* return_right)
{
	if(p == NULL)
		return;
	CurrentStmt(type , p->children[0] , return_right);
	CurrentStmtList(type , p->children[1] , return_right);
}

void CurrentStmt(Type type , struct tree_node* p , bool* return_right)
{
	if(p->children_num == 2)//Stmt -> Exp SEMI
		CurrentExp(p->children[0]);
	else if(p->children_num == 1)//Stmt -> CompSt
		CurrentCompSt(type , p->children[0] , return_right);
	else if(p->children_num == 3)//Stmt -> RETURN Exp SEMI
		CurrentReturnExp(type , p->children[1] , return_right);
	else if(p->children_num == 5)//Stmt->IF LP Exp RP Stmt
	{
		CurrentExp(p->children[1]);
		CurrentStmt(type , p->children[4] , return_right);
	}
	else if(p->children_num == 7)//Stmt->IF LP Exp RP Stmt ELSE Stmt
	{
		CurrentExp(p->children[1]);
		/*TODO*/
		CurrentStmt(type , p->children[4] , return_right);
		CurrentStmt(type , p->children[6] , return_right);
	}
	else if(p->children_num == 5)//Stmt->WHILE LP Exp Rp Stmt
	{
		CurrentExp(p->children[1]);
		/*TODO*/
		CurrentStmt(type , p->children[4] , return_right);
	}
	return ;
}


void CurrentDefList_1(struct tree_node* p)
{
	if(p == NULL)
		return;
	CurrentDef_1(p->children[0]);
	CurrentDefList_1(p->children[1]);
}

void CurrentDef_1(struct tree_node* p)
{
	Type type = CurrentSpecifier(p->children[0]);
	assert(type != NULL);
	if(type == NULL)
		return;
	CurrentDecList_1(type , p->children[1]);//Def -> Specifier DecList SEMI
}

void CurrentDecList_1(Type type , struct tree_node* p)
{
	if(p->children_num == 1)//DecList -> Dec
		CurrentDec_1(type , p->children[0]);
	else if(p->children_num == 3)//DecList -> Dec COMMA DecList
	{
		CurrentDec_1(type , p->children[0]);
		CurrentDecList_1(type ,p->children[2]);
	}
	else
		return;
}

void CurrentDec_1(Type type , struct tree_node* p)
{
	FieldList field = CurrentVarDec_1(type , p->children[0]);//Dec -> VarDec/Dec -> VarDec ASSIGNOP Exp
	if(p->children_num == 1)
	{
		if(field == NULL)
			return;
	}
	else if(p->children_num == 3)
	{
		if(field == NULL)
		{
			fprintf(stderr , "Error type 5 at Line %d : Not same type between ASSIGNOP\n" , p->lineno);
			return;
		}
		if(IsSameType(type , CurrentExp(p->children[2])))
		{
			fprintf(stderr , "Error type 5 at Line %d : Not same type between ASSIGNOP\n" , p->lineno);
			return;
		}
	}
}

FieldList CurrentVarDec_1(Type inh , struct tree_node* p)
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
			FieldList field = (FieldList)malloc(sizeof(struct FieldList_));
			int length = strlen(p->children[0]->unit_name);
			field->name = (char*)malloc(sizeof(char) * (length + 1));
			strcpy(field->name , p->children[0]->unit_name);
			field->type = inh;
			field->lineno = p->lineno;
			WriteIdTable(inh , p->children[0]->unit_name);
			return field;
		}
	}
	else if(p->children_num == 4)//VarDec -> VarDec LB INT RB
	{
		Type target = (Type)malloc(sizeof(struct Type_));
		target->kind = ARRAY;
		target->u.array.elem = inh;
		target->u.array.size = atoi(p->children[2]->unit_name);
		return CurrentVarDec(target , p->children[0]);
	}
	else
		return NULL;
}

void CurrentReturnExp(Type return_type , struct tree_node* p , bool* return_right)
{
	Type type = CurrentExp(p);
	if(IsSameType(return_type , type))
		*return_right = TRUE;
	else
		fprintf(stderr , "Error type 8 at Line %d : unmatched return type\n" , p->lineno);
}

Type CurrentExp(struct tree_node* p)
{
	if(p->children_num == 4)
	{

	}
	if(p->children_num == 3)
	{
		if(!strcmp(p->children[1]->token_name , "ASSIGNOP"))
		{
			if(!IsLeft(p->children[0]))
			{
				fprintf(stderr , "Error type 6 at Line %d : right value in the left of ASSIGNOP\n" , p->lineno);
				return NULL;
			}
		}
		if((!strcmp(p->children[0]->token_name , "Exp")) && (!strcmp(p->children[2]->token_name , "Exp")))
		{
			Type type1 = CurrentExp(p->children[0]);
			Type type2 = CurrentExp(p->children[2]);
			if(!IsSameType(type1 , type2))
			{
				fprintf(stderr , "Error type 5 at Line %d : Not same type\n" , p->lineno);
				return NULL;
			}
			return type1;
		}
		if((!strcmp(p->children[0]->token_name , "LP")) && (!strcmp(p->children[2]->token_name , "RP")))//Exp -> LP Exp RP
			return CurrentExp(p->children[1]);
		
		if(!strcmp(p->children[0]->token_name , "ID"))//Exp -> ID LP RP
		{
		 	int rank = FindFunc(p->children[0]->unit_name);
			if(rank == -1)
			{
				fprintf(stderr , "Error type 2 at Line %d : undefined function \n" , p->lineno);
				Type type = FindId(p->children[0]->unit_name);
				if(type != NULL)
					 fprintf(stderr , "Error type 11 at Line %d : use () in variable \n" , p->lineno);
				return NULL;
			}
			else
			{
				if(FuncTable[rank].para_amount != 0)
				{
					 fprintf(stderr , "Error type 7 at Line %d : unmatched parameter \n" , p->lineno);
					 return NULL;
				}
				return FuncTable[rank].return_type;
			}
		}
		if(!strcmp(p->children[1]->token_name , "DOT"))//Exp -> Exp DOT ID
		{

		}
	}
	else if(p->children_num == 2)
		return CurrentExp(p->children[1]);
	else if(p->children_num == 1)
	{
		if(!strcmp(p->children[0]->token_name , "ID"))
		{
			Type type =  FindId(p->children[0]->unit_name);
			if(type == NULL)
				fprintf(stderr , "Error type 1 at Line %d : undefined variable\n" , p->lineno);
			return type;
		}
		else if(!strcmp(p->children[0]->token_name , "INT"))
		{
			Type type = (Type)malloc(sizeof(struct Type_));
			type->kind = BASIC;
			type->u.basic = 0;
			return type;
		}
		else if(!strcmp(p->children[0]->token_name , "FLOAT"))
		{
			Type type = (Type)malloc(sizeof(struct Type_));
			type->kind = BASIC;
			type->u.basic = 1;
			return type;
		}
		else
			return NULL;
	}
		return NULL;
}

bool IsLeft(struct tree_node* p)
{
	if(p->children_num == 1)//Exp -> ID
	{
		if(!strcmp(p->children[0]->token_name , "ID"))
			return TRUE;
		else
			return FALSE;

	}
	else if(p->children_num == 4)//Exp -> Exp LB Exp RB
	{
		if(!strcmp(p->children[0]->token_name , "Exp"))
			return TRUE;
		else
			return FALSE;
	}
	else if(p->children_num == 3)//Exp -> Exp DOT ID
	{
		if(!strcmp(p->children[1]->token_name , "DOT"))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}
