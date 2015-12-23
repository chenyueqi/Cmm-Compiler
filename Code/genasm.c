#include"common.h"
#include"genasm.h"

void common_output(FILE* des)
{
	fprintf(des , ".data\n");
	vt_process(des);
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

void vt_process(FILE* des)
{
	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p->next;
		switch(intercode_p->code.kind)
		{
case ADD:
case SUB:
case MUL:
case DIV:		add_space_in_alop(intercode_p , des); 	break;
case ADD_ASSIGN:
case REF_ASSIGN:
case ASSIGN_REF:
case ASSIGN:		add_space_in_assign(intercode_p , des);	break;
case CALLFUNC:		add_space_in_call(intercode_p , des);	break;
case PARAM:		add_space_in_param(intercode_p , des);	break;
case READ:		add_space_in_read(intercode_p , des);	break;
case DEC:		fprintf(des , "_v%d: .space %d\n" , intercode_p->code.u.dec.x->u.var_no , intercode_p->code.u.dec.size);
		}
	}
}

void add_space_in_alop(struct InterCodes* p , FILE* des)
{
	Operand op = p->code.u.alop.x;
	switch(op->kind)
	{
		case VARIABLE:	add_v(op , des);	break;
		case TEMP:	add_t(op , des);	break;
	}
}


void add_space_in_assign(struct InterCodes* p , FILE* des)
{
	Operand op = p->code.u.assignop.x;
	switch(op->kind)
	{
		case VARIABLE:	add_v(op , des);	break;
		case TEMP:	add_t(op , des);	break;
	}
}

void add_space_in_call(struct InterCodes* p , FILE* des)
{
	Operand op = p->code.u.callfunc.x;
	switch(op->kind)
	{
		case VARIABLE:	add_v(op , des);	break;
		case TEMP:	add_t(op , des);	break;
	}
}

void add_space_in_param(struct InterCodes* p , FILE* des)
{
	Operand op = p->code.u.param.x;
	switch(op->kind)
	{
		case VARIABLE:	add_v(op , des);	break;
		case TEMP:	add_t(op , des);	break;
	}
}

void add_space_in_read(struct InterCodes* p , FILE* des)
{
	Operand op = p->code.u.read.x;
	switch(op->kind)
	{
		case VARIABLE:	add_v(op , des);	break;
		case TEMP:	add_t(op , des);	break;
	}
}

void add_v(Operand op , FILE* des)
{
	struct vt_chain* p = v_chain_head;
	while(p->next != NULL)
	{
		p = p->next;
		if(p->op->kind == VARIABLE && op->kind == VARIABLE && p->op->u.var_no == op->u.var_no)
			return ;
	}
	fprintf(des , "_v%d: .space %d\n" , op->u.var_no , 4);
	p->next = (struct vt_chain*)malloc(sizeof(struct vt_chain));
	p->next->op = op;
	p->next->next = NULL;
	return ;
}

void add_t(Operand op , FILE* des)
{
	struct vt_chain* p = t_chain_head;
	while(p->next!= NULL)
	{
		p = p->next;
		if(p->op->kind == TEMP && op->kind == TEMP && p->op->u.var_no == op->u.var_no)
			return ;
	}
	fprintf(des , "_t%d: .space %d\n" , op->u.var_no , 4);
	p->next = (struct vt_chain*)malloc(sizeof(struct vt_chain));
	p->next->op = op;
	p->next->next = NULL;
	return ;
}

void init_regfile()
{
	int i = 0;
	for( ; i < 10 ; i++)
		reg_t[i].op = NULL;

	i = 0;
	for(; i < 9 ; i++)
		reg_s[i].op = NULL;

	i = 0;
	for(; i < 4 ; i++)
		reg_a[i].op = NULL;

	i = 0;
	for(; i < 2 ; i++)
		reg_v[i].op = NULL;

}

void init_vt_chain()
{
	v_chain_head = (struct vt_chain*)malloc(sizeof(struct vt_chain));
	v_chain_head->op = NULL;
	v_chain_head->next = NULL;

	t_chain_head = (struct vt_chain*)malloc(sizeof(struct vt_chain));
	t_chain_head->op = NULL;
	t_chain_head->next = NULL;
}

void gen_asm(FILE* des)
{
	init_regfile();
	init_vt_chain();
	common_output(des);
	param_cnt = 0;
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
	int i = 0;
	for(; i < 9 ; i++)
	{
		if(reg_s[i].op != NULL)
		{
			Operand replaced_op = reg_s[i].op;
			fprintf(des , "\tla $t2, _");
			if(replaced_op->kind == VARIABLE)
				fprintf(des , "v%d\n" , replaced_op->u.var_no);
			else if(replaced_op->kind == TEMP)
				fprintf(des , "t%d\n" , replaced_op->u.temp_no);
			fprintf(des , "\tsw $s%d, 0($t2)\n" , i);
			reg_s[i].op = NULL;
		}
	}
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
		return;
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

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "%d\n" , p->code.u.alop.y->u.value);
			return;
		}
		else if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\taddi ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "%d\n" , p->code.u.alop.z->u.value);
			return ;
		}
		else
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			int reg_num3 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tadd ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);
			
			fprintf(des , ", ");

			if(reg_num3 < 10)
				fprintf(des , "$t%d" , reg_num3);
			else
				fprintf(des , "$s%d" , reg_num3 - 10);

			fprintf(des , "\n");
			return ;
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
		return ;
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

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "-%d\n" , p->code.u.alop.y->u.value);
			return ;
		}
		else if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\taddi ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");
			fprintf(des , "-%d\n" , p->code.u.alop.z->u.value);
			return ;
		}
		else
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			int reg_num3 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tsub ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);
			
			fprintf(des , ", ");

			if(reg_num3 < 10)
				fprintf(des , "$t%d" , reg_num3);
			else
				fprintf(des , "$s%d" , reg_num3 - 10);

			fprintf(des , "\n");
			return ;
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
		return ;
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
			fprintf(des , ", ");

			fprintf(des , "$t0, ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , "\n");
			return ;
		}
		else if(p->code.u.alop.z->kind == CONSTANT)
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			fprintf(des , "\tli $t0, %d\n" , p->code.u.alop.z->u.value);
			fprintf(des , "\tmul ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);
			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", $t0\n");
			return ;
		}
		else
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			int reg_num3 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tmul ");

			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);

			fprintf(des , ", ");

			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);
			
			fprintf(des , ", ");

			if(reg_num3 < 10)
				fprintf(des , "$t%d" , reg_num3);
			else
				fprintf(des , "$s%d" , reg_num3 - 10);

			fprintf(des , "\n");
			return ;
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
		return ;
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
			return ;
		}
		else if(p->code.u.alop.z->kind == CONSTANT)
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
			return ;
		}
		else
		{
			int reg_num1 = get_reg(p->code.u.alop.x , des);
			int reg_num2 = get_reg(p->code.u.alop.y , des);
			int reg_num3 = get_reg(p->code.u.alop.z , des);
			fprintf(des , "\tdiv ");
			if(reg_num2 < 10)
				fprintf(des , "$t%d" , reg_num2);
			else
				fprintf(des , "$s%d" , reg_num2 - 10);

			fprintf(des , ", ");

			if(reg_num3 < 10)
				fprintf(des , "$t%d" , reg_num3);
			else
				fprintf(des , "$s%d" , reg_num3 - 10);
			fprintf(des , "\n");

			fprintf(des , "\tmflo ");
			if(reg_num1 < 10)
				fprintf(des , "$t%d" , reg_num1);
			else
				fprintf(des , "$s%d" , reg_num1 - 10);
			fprintf(des , "\n");
			return ;
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
	if(p->code.u.ret.x->kind == CONSTANT)
	{
		fprintf(des , "\tli $t1, %d\n" , p->code.u.ret.x->u.value);
	}
	else
	{
		int reg_num = get_reg(p->code.u.ret.x , des);
		fprintf(des , "\tmove $v0, ");
		if(reg_num < 10)
			fprintf(des , "$t%d\n" , reg_num);
		else
			fprintf(des , "$s%d\n" , reg_num - 10);
	}
	fprintf(des, "\tjr $ra\n");

}

void outasmarg(struct InterCodes* p  , FILE* des)
{
	int reg_num = get_reg(p->code.u.arg.x , des);
	fprintf(des , "\tmove $a%d, " , param_cnt);	
	if(reg_num < 10)
		fprintf(des , "$t%d\n" , reg_num);
	else
		fprintf(des , "$s%d\n" , reg_num - 10);
	return ;
}

void outasmcallfunc(struct InterCodes* p  , FILE* des)
{
	int reg_num = get_reg(p->code.u.callfunc.x , des);
	param_cnt = 0;//清空reg_a
	fprintf(des , "\taddi $sp, $sp, -4\n");
	fprintf(des , "\tsw $ra, 0($sp)\n");
	fprintf(des , "\tjal %s\n" , p->code.u.callfunc.f->u.func_name);
	fprintf(des , "\tlw $ra, 0($sp)\n");
	fprintf(des , "\tmove ");
	if(reg_num < 10)
		fprintf(des , "$t%d, " , reg_num);
	else
		fprintf(des , "$s%d, " , reg_num - 10);
	fprintf(des , "$v0\n");
	return ;

}
void outasmparam(struct InterCodes* p  , FILE* des)
{

}
void outasmread(struct InterCodes* p  , FILE* des)
{

}
void outasmwrite(struct InterCodes* p  , FILE* des)
{
	int reg_num = get_reg(p->code.u.write.x , des);
	fprintf(des , "\tmove $a0, ");
	if(reg_num < 10)
		fprintf(des , "$t%d\n" , reg_num);
	else
		fprintf(des , "$s%d\n" , reg_num - 10);

	fprintf(des , "\taddi $sp, $sp, -4\n");
	fprintf(des , "\tsw $ra, 0($sp)\n");
	fprintf(des , "\tjal write\n");
	fprintf(des , "\tlw $ra, 0($sp)\n");
	return;
}

int get_reg(Operand op , FILE* des)//op可能是一个变量，临时变量或者地址或者要读取地址中的内容
{
	Operand new_op;
	if(op->kind == ADDRESS)
	{
		new_op = (Operand)malloc(sizeof(struct Operand_));
		new_op->kind = VARIABLE;
		new_op->u.var_no = op->u.var_no;
	}
	else if(op->kind == READ_ADDRESS)
	{
		new_op = (Operand)malloc(sizeof(struct Operand_));
		new_op->kind = TEMP;
		new_op->u.var_no = op->u.temp_no;
	}
	else
		new_op = op;
	int seed1 = rand();
	int i = 0;
	int hit = -1;
	int free = -1;

	for(; i < 4 ; i++)
	{
		if(reg_a[i].op == NULL)
			continue;
		if(reg_a[i].op->kind == new_op->kind)
		{
			if(new_op->kind == VARIABLE && reg_t[i].op->u.var_no == new_op->u.var_no)
			{
				hit = i;
				break;
			}
			else if(new_op->kind == TEMP && reg_t[i].op->u.temp_no == new_op->u.temp_no)
			{
				hit = i;
				break;
			}
		}
	}
	if(hit != -1)
		return -i;//因为结构体和数组不作为参数，所以可以直接返回

	i = 3;
	hit = -1;
	for(; i < 10 ; i++)
	{
		if(reg_t[i].op == NULL)
		{
			if(free == -1)
				free = i;
			continue;
		}
		if(reg_t[i].op->kind == op->kind)
		{
			if(new_op->kind == VARIABLE && reg_t[i].op->u.var_no == new_op->u.var_no)
			{
				hit = i;
				break;
			}
			else if(new_op->kind == TEMP && reg_t[i].op->u.temp_no == new_op->u.temp_no)
			{
				hit = i;
				break;
			}
		}
	}
	if(hit != -1)
		return hit;
	else if(free != -1)
	{
		reg_t[free].op = op;
		return free;
	}

	i = 0;
	hit = -1;
	free = -1;
	for(;i < 9 ; i++)
	{
		if(reg_s[i].op == NULL)
		{
			if(free == -1)
				free = i;
			continue;
		}
		if(new_op->kind == VARIABLE && reg_s[i].op->u.var_no == new_op->u.var_no)
		{
			hit = i;
			break;
		}
		else if(new_op->kind == TEMP && reg_s[i].op->u.temp_no == new_op->u.temp_no)
		{
			hit = i;
			break;
		}
	}
	if(hit != -1)
		return hit + 10;
	else if(free != -1)
	{
		reg_s[free].op = op;
		return free + 10;
	}


	int replace_num = seed1%16 + 3;
	if(replace_num < 10)
	{
		Operand replaced_op = reg_t[replace_num].op;
		fprintf(des , "\tla $t2, _");
		if(replaced_op->kind == VARIABLE)
			fprintf(des , "v%d\n" , replaced_op->u.var_no);
		else if(replaced_op->kind == TEMP)
			fprintf(des , "t%d\n" , replaced_op->u.temp_no);
		fprintf(des , "\tsw $t%d, 0($t2)\n" , replace_num);
		reg_t[replace_num].op = op;
		return replace_num;

	}
	else
	{
		Operand replaced_op = reg_s[replace_num - 10].op;
		fprintf(des , "\tla $t2, _");
		if(replaced_op->kind == VARIABLE)
			fprintf(des , "v%d\n" , replaced_op->u.var_no);
		else if(replaced_op->kind == TEMP)
			fprintf(des , "t%d\n" , replaced_op->u.temp_no);
		fprintf(des , "\tsw $s%d, 0($t2)\n" , replace_num - 10);
		reg_s[replace_num-10].op = op;
		return replace_num;
	}
}
