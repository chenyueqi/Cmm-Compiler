#ifndef __OPT__
#define __OPT__

#include"translate.h"

struct basic_block
{
	struct InterCodes* begin;
	struct InterCodes* end;
	struct basic_block* next;
};

int change_relop(int );

void optimize();

void optimize_before_divide();

void optimize_level2();

void get_basic_block();

int leader_buffer[30];//首指令的序号，从1开始

void fill_leader_buffer();

int get_label_num(Operand op);

int is_label_in_use(Operand op);


#endif
