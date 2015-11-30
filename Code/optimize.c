#include"optimize.h"

void deletecode(struct InterCodes* code)
{
	code->prev->next = code->next;
	code->next->prev = code->prev;
	free(code);
}

void optimize()
{
	optimize_before_divide();
	get_basic_block();
	optimize_level2();
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

void get_basic_block()
{
	fill_leader_buffer();

}

void fill_leader_buffer()
{
	int index = 0;
	int code_num = 0;
	struct InterCodes* current_code = code_head->next;
	while(current_code != code_head)
	{
		if(code_num == 0)//第一行指令
		{
			leader_buffer[index] = ++code_num;
		}
		if((current_code->code.kind == RELOP_GOTO) || (current_code->code.kind == GOTO))
		{
			int num = current_code->code.kind == RELOP_GOTO ? get_label_num(current_code->code.u.relopgoto.z) : get_label_num(current_code->code.u.gotolabel.x);
			leader_buffer[++index] = num;
			code_num ++;
			leader_buffer[++index] = code_num;
			current_code = current_code->next;
		}
		else
		{
			code_num ++;
			current_code = current_code->next;
		}
	}
	int i = 0;
	for(; i < index ; i++)
	{
		int j = 0;
		int max = leader_buffer[0];
		int x = 0;
		for(  ; j < index - i ; j++)
		{
			if(leader_buffer[j] > max)
			{
				max = leader_buffer[j];
				x = j;
			}
		}
		int temp = leader_buffer[index - i - 1];
		leader_buffer[index - i - 1] = max;
		leader_buffer[x] = temp;
	}
}

int get_label_num(Operand op)
{
	struct InterCodes* current_code = code_head->next;
	int num = 0;
	while(current_code != code_head)
	{
		num++;
		if(current_code->code.kind == LABEL)
		{
			if(op == current_code->code.u.label.x)
				return num;
		}
		current_code = current_code->next;
	}
}

void optimize_level2()
{

}
