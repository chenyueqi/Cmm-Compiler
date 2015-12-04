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
	return -1;
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
		//	Operand y = current_code->code.u.assignop.y;
			if(x->kind == TEMP)//存在优化可能
				current_code = try_to_delete_death(current_code , end);
		}
		current_code = current_code->next;
	}
	current_code = current->begin;
	while(current_code != end->next)
	{
		if(current_code->code.kind == ARG)
			lookback_arg(current_code, current->begin);
		current_code = current_code->next;
	}
	current_code = current->begin;
	while(current_code != end->next)
	{
		if((current_code->code.kind == ADD) || (current_code->code.kind == SUB) || (current_code->code.kind == MUL) || (current_code->code.kind == DIV) )
		{
			Operand x = current_code->code.u.alop.x;
			if(x->kind == TEMP)
			{
				if((current_code->next->code.kind == ASSIGN) && (current_code->next->code.u.assignop.x->kind != READ_ADDRESS))
				{
					Operand y = current_code->next->code.u.assignop.y;
					if(x == y)
					{
						if(!is_used_operand1(current_code->next , end , x))
						{
							current_code->code.u.alop.x = current_code->next->code.u.assignop.x;
							deletecode(current_code->next);
						}
					}
				}
			}
		}
		if(current_code->code.kind == CALLFUNC)
		{
			Operand x = current_code->code.u.callfunc.x;
			if(x->kind == TEMP)
			{
				if((current_code->next->code.kind == ASSIGN) && (current_code->next->code.u.assignop.x->kind != READ_ADDRESS))
				{
					Operand y = current_code->next->code.u.assignop.y;
					if(x == y)
					{
						if(!is_used_operand1(current_code->next , end , x))
						{
							current_code->code.u.callfunc.x = current_code->next->code.u.assignop.x;
							deletecode(current_code->next);
						}
					}
				}
			}
		}
		current_code = current_code->next;
	}
	current_code = current->begin;
	while(current_code != end->next)
	{
		if(current_code->code.kind == ASSIGN)
		{
			Operand x = current_code->code.u.assignop.x;
			if(x->kind == TEMP)
			{
				if((current_code->next->code.kind == ADD) || (current_code->next->code.kind == SUB) || (current_code->next->code.kind == MUL) || (current_code->next->code.kind == DIV) )
				{
					if((x == current_code->next->code.u.alop.y) || (x == current_code->next->code.u.alop.z))
					{
						if(!is_used_operand1(current_code->next , end , x))
						{
							if(x == current_code->next->code.u.alop.y)
								current_code->next->code.u.alop.y = current_code->code.u.assignop.y;
							if(x == current_code->next->code.u.alop.z)
								current_code->next->code.u.alop.z = current_code->code.u.assignop.y;
							current_code = current_code->next;
							deletecode(current_code->prev);
						}

					}

				}
			}
		}
		current_code = current_code->next;
	}
}

int is_used_operand1(struct InterCodes* current_code , struct InterCodes* end , Operand x)
{
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
			default: break;
		}
		code_p = code_p->next;
	}
	return flag;
}


struct InterCodes* try_to_delete_death(struct InterCodes* current_code , struct InterCodes* end)//消除死代码
{
	Operand x = current_code->code.u.assignop.x;
	if(current_code->prev->code.kind == RELOP_GOTO)
		return current_code;
	if(current_code->next->code.kind == RELOP_GOTO)
		return current_code;
	if(!is_used_operand1(current_code , end , x))
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

/*int assign_again(struct InterCodes* current_code, struct InterCodes* end)//判断是否为后续表达式的左值
{
	Operand x = current_code->code.u.assignop.x;
	struct InterCodes* code_p = current_code->next;
	int flag = 0;
	while(code_p != end->next)
	{
		switch(code_p->code.kind)
		{
			case ASSIGN: if(is_used_operand(code_p->code.u.assignop.x , x)) flag = 1; break;
			case ADD:
			case SUB:
			case MUL:
			case DIV: if(is_used_operand(code_p->code.u.alop.x , x)) flag = 1; break;
			case CALLFUNC: if(is_used_operand(code_p->code.u.callfunc.x , x)) flag = 1; break;
			case WRITE: if(is_used_operand(code_p->code.u.write.x , x)) flag = 1; break; 
		}
		code_p = code_p->next;
	}
	return flag;
}

struct InterCodes*  replace(struct InterCodes* current_code , struct InterCodes* end)
{
	Operand x = current_code->code.u.assignop.x;
	Operand y = current_code->code.u.assignop.y;
	struct InterCodes* code_p = current_code->next;
	//如果x既不为左值，也不是右值，则在消除死代码的时候已经删除
	while(code_p != end->next)
	{
		switch(code_p->code.kind)
		{
			case ASSIGN:
				if(code_p->code.u.assignop.y == x) code_p->code.u.assignop.y = y;break;
			case ADD:
			case SUB:
			case MUL:
			case DIV: 
				if(code_p->code.u.alop.y == x) code_p->code.u.alop.y = y;
				if(code_p->code.u.alop.z == x) code_p->code.u.alop.z = y;break;
			case RELOP_GOTO: 
				if(code_p->code.u.relopgoto.x == x) code_p->code.u.relopgoto.x = y;
				if(code_p->code.u.relopgoto.y == x) code_p->code.u.relopgoto.y = y;break;
			case RETURN:
				if(code_p->code.u.ret.x == x) code_p->code.u.ret.x = y;break;
			case ARG:
				if(code_p->code.u.arg.x == x) code_p->code.u.arg.x = y;break;
			case WRITE:
				if(code_p->code.u.write.x == x) code_p->code.u.write.x = y;break;	

		}
		code_p = code_p->next;
	}

	current_code = current_code->next;
	deletecode(current_code->prev);
	return current_code;
}
*/
void lookback_arg(struct InterCodes* current_code , struct InterCodes* begin)
{
	Operand x = current_code->code.u.arg.x;
	struct InterCodes* code_p = current_code;
	while(code_p != begin->prev)
	{
		if(code_p->code.kind == ASSIGN)
		{
			if(code_p->code.u.assignop.x == x)
			{
				current_code->code.u.arg.x = code_p->code.u.assignop.y;
				deletecode(code_p);
				break;
			}
		}	
		code_p = code_p->prev;
	}
}
