#include"common.h"
#include"translate.h"
#include"FuncTable.h"

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

void outputInterCode(char* file_name)
{
	FILE* des = fopen(file_name , "w");
	if(!des)
	{
		perror(file_name);
		return;
	}
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p -> next;
		switch(intercode_p->code.kind)
		{
case LABLE:		outputlable(intercode_p , des);		break;
case FUNCTION:		outputfunction(intercode_p , des);	break;
case ADD:
case SUB:
case MUL:
case DIV:		outputalop(intercode_p , des);		break;
case ADD_ASSIGN:
case REF_ASSIGN:
case ASSIGN_REF:
case ASSIGN:		outputassign(intercode_p , des);	break;
case GOTO:		outputgoto(intercode_p , des);		break;
case RELOP_GOTO: 	outputrelopgoto(intercode_p , des);	break;
case RETURN:		outputreturn(intercode_p , des);	break;
case DEC:		outputdec(intercode_p , des);		break;
case ARG:		outputarg(intercode_p , des);		break;
case CALLFUNC:		outputcallfunc(intercode_p , des);	break;
case PARAM:		outputparam(intercode_p , des);		break;
case READ:		outputread(intercode_p , des);		break;
case WRITE:		outputwrite(intercode_p , des);		break;
default:	fprintf(stderr , "%s , %d , %d \n" , __FILE__ , __LINE__ , intercode_p->code.kind);
		}
	}
	fclose(des);
	
}

void outputlable(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "LABLE lable%d :\n" , intercode_p->code.u.lable.x->u.lable_no);
}

void outputfunction(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "FUNCTION %s :\n" , intercode_p->code.u.function.f->u.func_name);
}

void outputalop(struct InterCodes* intercode_p , FILE* des)
{

}

void outputassign(struct InterCodes* intercode_p , FILE* des)
{

}

void outputgoto(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "GOTO lable%d : \n" , intercode_p->code.u.gotolable.x->u.lable_no);
}

void outputrelopgoto(struct InterCodes* intercode_p , FILE* des)
{

}

void outputreturn(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "RETURN \n");
}

void outputdec(struct InterCodes* intercode_p , FILE* des)
{

}


void outputarg(struct InterCodes* intercode_p , FILE* des)
{

}

void outputcallfunc(struct InterCodes* intercode_p , FILE* des)
{
	
}

void outputparam(struct InterCodes* intercode_p , FILE* des)
{
	
}

void outputread(struct InterCodes* intercode_p , FILE* des)
{
	
}

void outputwrite(struct InterCodes* intercode_p , FILE* des)
{
	
}

void initial_InterCodes()
{
	code_head = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	code_head->next = code_head;
	code_head->prev = code_head;
	current_code = code_head;
	variable_num = 0;
}

void insertcode(struct InterCodes* new_code)
{
	new_code->prev = current_code;
	new_code->next = current_code->next;
	current_code->next = new_code;
	current_code = new_code;

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
	{
		translate_exp(p);
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

void translate_exp(struct tree_node* p)
{
	if(p->children_num == 1)
	{
		if(!strcmp(p->children[0]->token_name , "ID"))
		{

		}
		else if(!strcmp(p->children[0]->token_name , "INT"))
		{
			fprintf(stderr , "%s , %d\n" , __FILE__ , __LINE__);


		}
		else if(!strcmp(p->children[0]->token_name , "FLOAT"))
		{

		}
		else
			return;

	}
	else if(p->children_num == 2)
	{

	}
	else if(p->children_num == 3)
	{

	}
	else if(p->children_num == 4)
	{

	}
	else 
		return;
}
