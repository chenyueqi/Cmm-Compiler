#include"optimize.h"

void optimize()
{
	optimize_level1();
	optimize_level2();
}

void optimize_level1()
{
	struct InterCodes* current_code = code_head;
	while(current_code->next != code_head)
	{
		if(current_code->code.kind == ASSIGN)
		{
			Operand x = current_code->code.u.assignop.x;
			Operand y = current_code->code.u.assignop.y;
		}

	}
}

void optimize_level2()
{

}
