#include"common.h"
#include"translate.h"
#include"FuncTable.h"
#include"IdTable.h"

void pre_occupy_func()
{
	Type return_type = (Type)malloc(sizeof(struct Type_));
	return_type->kind = BASIC;
	return_type->u.basic = 0;
	int para_amount = 0;
	FieldList parameter = NULL;
	WriteFuncTable("read" , return_type , para_amount , parameter);
	
	return_type = (Type)malloc(sizeof(struct Type_));
	return_type->kind = BASIC;
	return_type->u.basic = 0;
	FuncTable[1].para_amount = 1;
	parameter = (FieldList)malloc(sizeof(struct FieldList_));
	parameter->name = NULL;
	parameter->type = (Type)malloc(sizeof(struct Type_));
	parameter->type->kind = BASIC;
	parameter->type->u.basic = 0;
	parameter->lineno = -1;
	parameter->next = NULL;
	WriteFuncTable("write" , return_type , para_amount , parameter);
}

void outputInterCode()
{
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p -> next;
		switch(intercode_p->code.kind)
		{
case LABEL:		outputlabel(intercode_p);		break;
case FUNCTION:		outputfunction(intercode_p);	break;
case ADD:
case SUB:
case MUL:
case DIV:		outputalop(intercode_p);		break;
case ADD_ASSIGN:
case REF_ASSIGN:
case ASSIGN_REF:
case ASSIGN:		outputassign(intercode_p);	break;
case GOTO:		outputgoto(intercode_p);		break;
case RELOP_GOTO: 	outputrelopgoto(intercode_p);	break;
case RETURN:		outputreturn(intercode_p);	break;
case DEC:		outputdec(intercode_p);		break;
case ARG:		outputarg(intercode_p);		break;
case CALLFUNC:		outputcallfunc(intercode_p);	break;
case PARAM:		outputparam(intercode_p);		break;
case READ:		outputread(intercode_p);		break;
case WRITE:		outputwrite(intercode_p);		break;
default:	fprintf(stderr , "%s , %d , %d \n" , __FILE__ , __LINE__ , intercode_p->code.kind);
		}
	}
}

void outputoperand(Operand op)
{
	switch(op->kind)
	{
		case VARIABLE: 	printf("v%d" , op->u.var_no);break;
		case TEMP:	printf("t%d" , op->u.temp_no);break;
		case CONSTANT:	printf("#%d" , op->u.value);break;
	}
}

void outputlabel(struct InterCodes* intercode_p)
{
	printf("LABEL lable%d :\n" , intercode_p->code.u.label.x->u.label_no);
}

void outputfunction(struct InterCodes* intercode_p)
{
	printf("FUNCTION %s :\n" , intercode_p->code.u.function.f->u.func_name);
}

void outputalop(struct InterCodes* intercode_p)
{
	outputoperand(intercode_p->code.u.alop.x);
	printf(" := ");
	outputoperand(intercode_p->code.u.alop.y);
	switch(intercode_p->code.kind)
	{
		case ADD: printf(" + ");break;
		case SUB: printf(" - ");break;
		case MUL: printf(" * ");break;
		case DIV: printf(" / ");break;
	}
	outputoperand(intercode_p->code.u.alop.z);
	printf("\n");

}

void outputassign(struct InterCodes* intercode_p)
{
	outputoperand(intercode_p->code.u.assignop.x);
	printf(" := ");
	outputoperand(intercode_p->code.u.assignop.y);
	printf("\n");
}

void outputgoto(struct InterCodes* intercode_p)
{
	printf("GOTO lable%d : \n" , intercode_p->code.u.gotolabel.x->u.label_no);
}

void outputrelopgoto(struct InterCodes* intercode_p)
{

}

void outputreturn(struct InterCodes* intercode_p)
{
	printf("RETURN \n");
}

void outputdec(struct InterCodes* intercode_p)
{

}


void outputarg(struct InterCodes* intercode_p)
{

}

void outputcallfunc(struct InterCodes* intercode_p)
{
	
}

void outputparam(struct InterCodes* intercode_p)
{
	
}

void outputread(struct InterCodes* intercode_p)
{
	
}

void outputwrite(struct InterCodes* intercode_p)
{
	
}

void initial_InterCodes()
{
	code_head = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	code_head->next = code_head;
	code_head->prev = code_head;
	current_code = code_head;
	variable_num = 0;
	temp_num = 0;
	label_num = 0;
}

void insertcode(struct InterCodes* new_code)
{
	new_code->prev = current_code;
	new_code->next = current_code->next;
	current_code->next = new_code;
	current_code = new_code;
	code_head->prev = current_code;
}

void translate(struct tree_node* p)
{
	if(p == NULL)
		return;
	if(!strcmp(p->token_name , "FunDec"))
	{
		translate_function(p);
		return;
	}
	else if(!strcmp(p->token_name , "Exp"))
	{	Operand op1;
		op1 = (Operand)malloc(sizeof(struct Operand_));
		translate_exp(p, op1);
		return ;
	}
	else
	{
		int i = 0;
		for(i = 0 ; i < p->children_num ; i++)
			translate(p->children[i]);
		return;
	}

}

void translate_function(struct tree_node* p)
{
	char* func_name = p->children[0]->unit_name;
	struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	new_code->code.kind = FUNCTION;
	new_code->code.u.function.f = (Operand)malloc(sizeof(struct Operand_));
	new_code->code.u.function.f->kind = FUNC;
	new_code->code.u.function.f->u.func_name = (char*)malloc(sizeof(char) * (strlen(func_name) + 1));
	strcpy(new_code->code.u.function.f->u.func_name , func_name);

	insertcode(new_code);
	return;
}

void translate_exp(struct tree_node* p , Operand place)
{
	if(p->children_num == 1)
	{
		if(!strcmp(p->children[0]->token_name , "INT"))
		{
			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand right;
			right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = CONSTANT;
			right->u.value = atoi(p->children[0]->unit_name);
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = right;

			insertcode(new_code);
			return;
		}
		else if(!strcmp(p->children[0]->token_name , "FLOAT"))
		{
			/* it seems never appear*/
			return;
		}
		else if(!strcmp(p->children[0]->token_name , "ID"))
		{
			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand right;
			right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = VARIABLE;
			right->u.var_no = lookup(p->children[0]->unit_name);
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = right;

			insertcode(new_code);

			return;
		}
		else
			return;

	}
	else if(p->children_num == 2)
	{
		if(!strcmp(p->children[0]->token_name , "MINUS"))
		{
			Operand t1;
			t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[1] , t1);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand mid;
			mid = (Operand)malloc(sizeof(struct Operand_));
			mid->kind = CONSTANT;
			mid->u.value = 0;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = SUB;
			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = mid;
			new_code->code.u.alop.z = t1;

			insertcode(new_code);
		}
		else if(!strcmp(p->children[0]->token_name , "NOT"))
		{

		}
		else
			return;

	}
	else if(p->children_num == 3)
	{
		if(!strcmp(p->children[1]->token_name , "ASSIGNOP"))
		{
			char* id_name = p->children[0]->children[0]->unit_name;
			int var_no = lookup(id_name);
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t1);

			place->kind = VARIABLE;
			place->u.var_no = var_no;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;


			insertcode(new_code);
		}
		else if(!strcmp(p->children[1]->token_name , "PLUS"))
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ADD;
			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = t1;
			new_code->code.u.alop.z = t2;

			insertcode(new_code);
		}
		else if(!strcmp(p->children[1]->token_name , "MINUS"))
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = SUB;
			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = t1;
			new_code->code.u.alop.z = t2;

			insertcode(new_code);
		}
		else if(!strcmp(p->children[1]->token_name , "STAR"))
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = MUL;
			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = t1;
			new_code->code.u.alop.z = t2;

			insertcode(new_code);
		}
		else if(!strcmp(p->children[1]->token_name , "DIV"))
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = DIV;
			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = t1;
			new_code->code.u.alop.z = t2;

			insertcode(new_code);
		}
		else if(!strcmp(p->children[1]->token_name , "RELOP"))
		{
			Operand label1;
			label1 = (Operand)malloc(sizeof(struct Operand_));
			label1->kind = LABEL_SIGN;
			label1->u.label_no = ++label_num;

			Operand label2;
			label2 = (Operand)malloc(sizeof(struct Operand_));
			label2->kind = LABEL_SIGN;
			label2->u.label_no = ++label_num;


		}
		else if(!strcmp(p->children[1]->token_name , "AND"))
		{

		}
		else if(!strcmp(p->children[1]->token_name , "OR"))
		{

		}

	}
	else if(p->children_num == 4)
	{

	}
	else 
		return;
}
