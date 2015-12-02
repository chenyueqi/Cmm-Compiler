#include"optimize.h"

void deletecode(struct InterCodes* code)
{
	code->prev->next = code->next;
	code->next->prev = code->prev;
	free(code);
}

int is_label_in_use(Operand op)
{
	struct InterCodes* current_code = code_head->next;
	while(current_code != code_head)
	{
		if((current_code->code.kind == GOTO) || (current_code->code.kind == RELOP_GOTO))
		{
			Operand t1 = current_code->code.kind == GOTO ? current_code->code.u.gotolabel.x : current_code->code.u.relopgoto.z;
			if(t1 == op)
				return 1;
			else
				current_code = current_code->next;

		}
		else
			current_code = current_code->next;
	}
	return 0;
}

void optimize()
{
	optimize_before_divide();
	get_basic_block();
	optimize_level1();
}

void optimize_before_divide()//在基本块分割之前对于一些条件判断语句做优化和无用label的消除
{
	struct InterCodes* current_code = code_head->next;
	while(current_code != code_head)
	{
		if(current_code->code.kind == RELOP_GOTO)
		{
			struct InterCodes* second_code = current_code->next;
			struct InterCodes* third_code = second_code->next;
			if((second_code->code.kind == GOTO) && (third_code->code.kind == LABEL) &&(current_code->code.u.relopgoto.z == third_code->code.u.gotolabel.x))
			{
				current_code->code.u.relopgoto.z = second_code->code.u.gotolabel.x;
				int relop = current_code->code.u.relopgoto.relop->u.relop;
				current_code->code.u.relopgoto.relop->u.relop = change_relop(relop);
				deletecode(second_code);
				current_code = current_code->next->next;
			}
			else
				current_code = current_code->next;

		}
		else
			current_code = current_code->next;
	}
	current_code = code_head->next;
	while(current_code != code_head)
	{
		if(current_code->code.kind == LABEL)
		{
			int i = is_label_in_use(current_code->code.u.label.x);
			if(i == 0)
			{
				current_code = current_code->next;
				free(current_code->prev->code.u.label.x);
				deletecode(current_code->prev);
			}
			else
				current_code = current_code->next;
		}
		else
			current_code = current_code->next;
	}
}


int change_relop(int origin)
{
	switch(origin)
	{
		case 0: return 3; //> <=
		case 1: return 2; //< >=
		case 2: return 1; //>= <
		case 3: return 0; //<= >
		case 4: return 5; //== !=
		case 5: return 4; //!= ==
	}
}


void get_basic_block()
{
	initial_basic_block();
	struct InterCodes* current_code = code_head->next;
	struct InterCodes* begin;
	struct InterCodes* end;
	while(current_code != code_head)
	{
		if(current_code->code.kind == FUNCTION)
		{
//			outputfunction(current_code , stderr);
			begin = current_code->next;
		}
		else if(current_code->code.kind == LABEL)
		{
//			outputlabel(current_code , stderr);
			end = current_code->prev;
			insert_basic_block(begin , end);
			begin = current_code->next;
		}
		else if((current_code->code.kind == RELOP_GOTO) || (current_code->code.kind == GOTO))
		{
//			current_code->code.kind == RELOP_GOTO ? outputrelopgoto(current_code , stderr) : outputgoto(current_code , stderr);
			end = current_code;
			insert_basic_block(begin , end);
			begin = current_code->next;
		}
		else if(current_code->code.kind == RETURN)
		{
//			outputreturn(current_code , stderr);
			end = current_code;
			insert_basic_block(begin , end);
			begin = current_code->next;
		}
		
		current_code = current_code->next;
	}
}

void initial_basic_block()
{
	basic_block_head = (struct basic_block*)malloc(sizeof(struct basic_block));
	basic_block_head->prev = basic_block_head;
	basic_block_head->next = basic_block_head;
	current_basic_block = basic_block_head;
}

void insert_basic_block(struct InterCodes* begin , struct InterCodes* end)
{
	struct basic_block* new_block = (struct basic_block*)malloc(sizeof(struct basic_block));
	new_block->begin = begin;
	new_block->end = end;
	new_block->prev = current_basic_block;
	new_block->next = current_basic_block->next;
	current_basic_block->next = new_block;
	current_basic_block = new_block;
}

void optimize_level1()
{
	struct basic_block* current_basic_block = basic_block_head ->next;
	while(current_basic_block != basic_block_head)
	{
		do_level2_optimize(current_basic_block);
		current_basic_block = current_basic_block->next;
	}
}

void do_level2_optimize(struct basic_block* current)
{
	struct InterCodes* current_code = current->begin;
	struct InterCodes* end = current->end;
	while(current_code != end->next)
	{
		if(current_code->code.kind == ASSIGN)
		{
			Operand x = current_code->code.u.assignop.x;
			Operand y = current_code->code.u.assignop.y;
			if(x->kind == TEMP)//存在优化可能
				current_code = try_to_delete_death(current_code , end);
		}
		current_code = current_code->next;
	}
}

struct InterCodes* try_to_delete_death(struct InterCodes* current_code , struct InterCodes* end)
{
	Operand x = current_code->code.u.assignop.x;
	struct InterCodes* code_p = current_code->next;
	int flag = 0;
	while(code_p != end->next)
	{
		switch(code_p->code.kind)
		{
			case ASSIGN: if(is_used_operand(code_p->code.u.assignop.x , x) || is_used_operand(code_p->code.u.assignop.y , x)) flag = 1; break;
			case ADD:
			case SUB:
			case MUL:
			case DIV: if(is_used_operand(code_p->code.u.alop.x , x) || is_used_operand(code_p->code.u.alop.y , x) || (is_used_operand)(code_p->code.u.alop.z , x)) flag = 1; break;
			case RELOP_GOTO: if(is_used_operand(code_p->code.u.relopgoto.x , x) || (is_used_operand(code_p->code.u.relopgoto.y , x))) flag = 1; break;
			case RETURN: if(is_used_operand(code_p->code.u.ret.x , x)) flag = 1; break;
			case ARG: if(is_used_operand(code_p->code.u.arg.x , x)) flag = 1; break;
			case CALLFUNC: if(is_used_operand(code_p->code.u.callfunc.x , x)) flag = 1; break;
			case WRITE: if(is_used_operand(code_p->code.u.write.x , x)) flag = 1; break; 
		}
		code_p = code_p->next;
	}
	if(flag == 0)
	{
		current_code = current_code->next;
		deletecode(current_code->prev);
	}
	return current_code;
}

int is_used_operand(Operand x , Operand y)
{
	//已知y 是 TEMP类型的
	if(x->kind == TEMP)
	{
		if(x == y)
			return 1;
		else
			return 0;
	}
	else if(x->kind == READ_ADDRESS)
	{
		if(x->u.temp_no == y->u.temp_no)
			return 1;
		else
			return 0;
	}
	
	return 0;
}
