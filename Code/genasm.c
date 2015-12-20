#include"common.h"
#include"genasm.h"

void common_output(FILE* des)
{
	fprintf(des , ".data\n");
	dec_process(des);
	fprintf(des , "_prompt: .asciiz \"Enter an integer:\"\n");
	fprintf(des , "_ret: .asciiz \"\\n\"\n");
	fprintf(des , ".globl main\n");
	fprintf(des , ".text\n");
	fprintf(des , "read:\n");
	fprintf(des , "\tli $v0, 4\n");
	fprintf(des , "\tla $a0, _prompt\n");
	fprintf(des , "\tsyscall\n");
	fprintf(des , "\tli $v0, 5\n");
	fprintf(des , "\tsyscall\n");
	fprintf(des , "\tjr $ra\n");
	fprintf(des , "\n");
	fprintf(des , "write:\n");
	fprintf(des , "\tli $v0, 1\n");
	fprintf(des , "\tsyscall\n");
	fprintf(des , "\tli $v0, 4\n");
	fprintf(des , "\tla $a0, _ret\n");
	fprintf(des , "\tsyscall\n");
	fprintf(des , "\tmove $v0, $0\n");
	fprintf(des , "\tjr $ra\n");
	fprintf(des , "\n");
	return;
}

void dec_process(FILE* des)
{
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p->next;
		if(intercode_p->code.kind == DEC)
		{
			fprintf(des , "v%d: .space %d\n" , intercode_p->code.u.dec.x->u.var_no , intercode_p->code.u.dec.size);

		}
	}
}

void init_regfile()
{
	int i = 0;
	for( ; i < 10 ; i++)
	{
		reg_t[i].head = NULL;
		reg_t[i].Cnt = 0;
	}

	i = 0;
	for(; i < 9 ; i++)
	{
		reg_s[i].head = NULL;
		reg_s[i].Cnt = 0;
	}

	i = 0;
	for(; i < 4 ; i++)
	{
		reg_a[i].head = NULL;
		reg_a[i].Cnt = 0;
	}

	i = 0;
	for(; i < 2 ; i++)
	{
		reg_v[i].head = NULL;
		reg_v[i].Cnt = 0;
	}

}

void gen_asm(FILE* des)
{
	init_regfile();
	common_output(des);
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p->next;
		switch(intercode_p->code.kind)
		{
case LABEL: 		outasmlabel(intercode_p , des);		break;
case FUNCTION: 		outasmfunc(intercode_p , des);		break;
case ADD:		outasmadd(intercode_p , des);		break;
case SUB:		outasmsub(intercode_p , des);		break;
case MUL:		outasmmul(intercode_p , des);		break;
case DIV:		outasmdiv(intercode_p , des);		break;
case ADD_ASSIGN:
case REF_ASSIGN:
case ASSIGN_REF:
case ASSIGN:		outasmassign(intercode_p , des);	break;
case GOTO:		outasmgoto(intercode_p , des);		break;
case RELOP_GOTO:	outasmrelopgoto(intercode_p , des);	break;
case RETURN:		outasmreturn(intercode_p , des);	break;
case DEC:		outasmdec(intercode_p , des);		break;
case ARG:		outasmarg(intercode_p , des);		break;
case CALLFUNC:		outasmcallfunc(intercode_p , des);	break;
case PARAM:		outasmparam(intercode_p , des);		break;
case READ:		outasmread(intercode_p , des);		break;
case WRITE:		outasmwrite(intercode_p , des);		break;
		}
	}
}


void outasmlabel(struct InterCodes* p  , FILE* des)
{
	fprintf(des , "label%d:\n" , p->code.u.label.x->u.label_no);
	return;
}

void outasmfunc(struct InterCodes* p  , FILE* des)
{
	fprintf(des , "%s:\n" , p->code.u.function.f->u.func_name);
	return;
}

void outasmadd(struct InterCodes* p  , FILE* des)
{
	if(p->code.u.alop.y->kind == CONSTANT && p->code.u.alop.z->kind == CONSTANT)
	{
		int reg_num = get_reg(p->code.u.alop.x , des);
		fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.y->u.value);
		fprintf(des , "\tli $t1, %d\n" , p->code.u.alop.z->u.value);
		fprintf(des , "\tadd ");
		if(reg_num < 10)
			fprintf(des , "$t%d" , reg_num);
		else
			fprintf(des , "$s%d" , reg_num - 10);
		fprintf(des , " $t0, $t1\n");
	}
	else
	{
		if(p->code.u.alop.y->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\taddi ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , " ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "%d\n" , p->code.u.alop.y->u.value);
		}
		if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\taddi ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , " ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "%d\n" , p->code.u.alop.z->u.value);
		}
	}
}

void outasmsub(struct InterCodes* p  , FILE* des)
{
	if(p->code.u.alop.y->kind == CONSTANT && p->code.u.alop.z->kind == CONSTANT)
	{
		int reg_num = get_reg(p->code.u.alop.x , des);
		fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.y->u.value);
		fprintf(des , "\tli $t1, %d\n" , p->code.u.alop.z->u.value);
		fprintf(des , "\tsub ");
		if(reg_num < 10)
			fprintf(des , "$t%d" , reg_num);
		else
			fprintf(des , "$s%d" , reg_num - 10);
		fprintf(des , " $t0, $t1\n");
	}
	else
	{
		if(p->code.u.alop.y->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tsubi ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , " ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "%d\n" , p->code.u.alop.y->u.value);
		}
		if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\tsubi ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , " ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "%d\n" , p->code.u.alop.z->u.value);
		}
	}
}

void outasmmul(struct InterCodes* p  , FILE* des)
{
	if(p->code.u.alop.y->kind == CONSTANT && p->code.u.alop.z->kind == CONSTANT)
	{
		int reg_num = get_reg(p->code.u.alop.x , des);
		fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.y->u.value);
		fprintf(des , "\tli $t1, %d\n" , p->code.u.alop.z->u.value);
		fprintf(des , "\tmul ");
		if(reg_num < 10)
			fprintf(des , "$t%d" , reg_num);
		else
			fprintf(des , "$s%d" , reg_num - 10);
		fprintf(des , " $t0, $t1\n");
	}
	else
	{
		if(p->code.u.alop.y->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.y->u.value);
			fprintf(des , "\tmul ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);
			fprintf(des , " ");

			fprintf(des , "$t0, ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , "\n");
		}
		if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.z->u.value);
			fprintf(des , "\tmul ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);
			fprintf(des , " ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , ", $t0\n");
		}
	}
}
void outasmdiv(struct InterCodes* p  , FILE* des)
{
	if(p->code.u.alop.y->kind == CONSTANT && p->code.u.alop.z->kind == CONSTANT)
	{
		int reg_num = get_reg(p->code.u.alop.x , des);
		fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.y->u.value);
		fprintf(des , "\tli $t1, %d\n" , p->code.u.alop.z->u.value);
		fprintf(des , "\tdiv $t0, $t1\n");

		fprintf(des , "\tmflo ");
		if(reg_num < 10)
			fprintf(des , "$t%d" , reg_num);
		else
			fprintf(des , "$s%d" , reg_num - 10);
		fprintf(des , "\n");
	}
	else
	{
		if(p->code.u.alop.y->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.y->u.value);
			fprintf(des , "\tdiv ");
			fprintf(des , "$t0, ");
			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , "\n\tmflo ");
			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);
			fprintf(des , "\n");
		}
		if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.z->u.value);
			fprintf(des , "\tdiv ");
			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);
			fprintf(des , ", $t0\n");

			fprintf(des , "\tmflo ");
			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);
			fprintf(des , "\n");
		}
	}
}
void outasmassign(struct InterCodes* p  , FILE* des)
{
	if(p->code.u.assignop.y->kind == CONSTANT)
	{
		int reg_num = get_reg(p->code.u.assignop.x , des);
		fprintf(des , "\tli ");
		if(reg_num < 10)
			fprintf(des , "$t%d" , reg_num);
		else
			fprintf(des , "$s%d" , reg_num - 10);
		fprintf(des , ", %d\n" , p->code.u.assignop.y->u.value);
	}
	else
	{
		int reg_num1 = get_reg(p->code.u.assignop.x , des);
		int reg_num2 = get_reg(p->code.u.assignop.y , des);
		fprintf(des , "\tmove ");
		if(reg_num1 < 10)
			fprintf(des , "$t%d" , reg_num1);
		else
			fprintf(des , "$s%d" , reg_num1 - 10);
		fprintf(des , ", ");

		if(reg_num2 < 10)
			fprintf(des , "$t%d" , reg_num2);
		else
			fprintf(des , "$s%d" , reg_num2 - 10);
		fprintf(des , "\n");
	}
}
void outasmgoto(struct InterCodes* p  , FILE* des)
{
	fprintf(des , "\tj label%d\n" , p->code.u.gotolabel.x->u.label_no);
}
void outasmrelopgoto(struct InterCodes* p  , FILE* des)
{

}
void outasmdec(struct InterCodes* p  , FILE* des)
{

}
void outasmreturn(struct InterCodes* p  , FILE* des)
{

}
void outasmarg(struct InterCodes* p  , FILE* des)
{

}
void outasmcallfunc(struct InterCodes* p  , FILE* des)
{

}
void outasmparam(struct InterCodes* p  , FILE* des)
{

}
void outasmread(struct InterCodes* p  , FILE* des)
{

}
void outasmwrite(struct InterCodes* p  , FILE* des)
{

}

int get_reg(Operand op , FILE* des)//op可能是一个变量，临时变量或者地址或者要读取地址中的内容
{
	return 8;
}
