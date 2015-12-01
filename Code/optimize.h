#ifndef __OPT__
#define __OPT__

#include"translate.h"

struct basic_block
{
	struct InterCodes* begin;
	struct InterCodes* end;
	struct basic_block* prev;
	struct basic_block* next;
};

struct basic_block* current_basic_block;
struct basic_block* basic_block_head;

int change_relop(int );

void optimize();

void optimize_before_divide();

void optimize_level1();

void get_basic_block();

void initial_basic_block();

void insert_basic_block(struct InterCodes* , struct InterCodes*);

void do_level2_optimize(struct basic_block*);

struct InterCodes* try_to_delete_death(struct InterCodes* , struct InterCodes*);

int is_used_operand(Operand x , Operand y);
#endif
