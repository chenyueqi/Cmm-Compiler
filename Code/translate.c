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
	fprintf(des , "***%s***" , file_name);
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		switch(intercode_p->code.kind)
		{
LABLE:		outputlable(intercode_p , des);		break;
FUNCTION:	outputfunction(intercode_p , des);	break;
ADD:
SUB:
MUL:
DIV:		outputalop(intercode_p , des);		break;
ADD_ASSIGN:
REF_ASSIGN:
ASSIGN_REF:
ASSIGN:		outputassign(intercode_p , des);	break;
GOTO:		outputgoto(intercode_p , des);		break;
RELOP_GOTO: 	outputrelopgoto(intercode_p , des);	break;
RETURN:		outputreturn(intercode_p , des);	break;
DEC:		outputdec(intercode_p , des);		break;
ARG:		outputarg(intercode_p , des);		break;
CALLFUNC:	outputcallfunc(intercode_p , des);	break;
PARAM:		outputparam(intercode_p , des);		break;
READ:		outputread(intercode_p , des);		break;
WRITE:		outputwrite(intercode_p , des);		break;
		}


		intercode_p = intercode_p -> next;
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

void translate(struct tree_node* root)
{
	code_head = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	code_head->next = code_head;
	code_head->prev = code_head;

}
