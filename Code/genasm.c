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

void gen_asm(FILE* des)
{
	common_output(des);
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p->next;
		switch(intercode_p->code.kind)
		{
case LABEL: 		outasmlabel(intercode_p , des);		break;
case FUNCTION: 		outasmfunc(intercode_p , des);		break;
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
