#ifndef __GEN__
#define __GEN__

#include "translate.h"

void common_output(FILE*);
void dec_process(FILE*);

struct Reg
{
	Operand op;
};

struct vt_chain
{
	Operand op;
	struct vt_chain* next;
};

struct Reg reg_t[10];	//use freely belong to caller
/*t寄存器中t0 , t1分配给常数使用，t2用于la指令*/

struct Reg reg_s[9];	//use freely belong to callee
struct Reg reg_a[4];	//for parameters
struct Reg reg_v[2];	//for return value

struct vt_chain* v_chain_head;
struct vt_chain* t_chain_head;


void outasmlabel(struct InterCodes* , FILE*);
void outasmfunc(struct InterCodes* , FILE*);
void outasmadd(struct InterCodes* , FILE*);
void outasmsub(struct InterCodes* , FILE*);
void outasmmul(struct InterCodes* , FILE*);
void outasmdiv(struct InterCodes* , FILE*);
void outasmassign(struct InterCodes* , FILE*);
void outasmgoto(struct InterCodes* , FILE*);
void outasmrelopgoto(struct InterCodes* , FILE*);
void outasmreturn(struct InterCodes* , FILE*);
void outasmdec(struct InterCodes* , FILE*);
void outasmarg(struct InterCodes* , FILE*);
void outasmcallfunc(struct InterCodes* , FILE*);
void outasmparam(struct InterCodes* , FILE*);
void outasmread(struct InterCodes* , FILE*);
void outasmwrite(struct InterCodes* , FILE*);

void init_regfile();
void init_vt_chain();

void common_output(FILE*);
void vt_process(FILE*);

void add_space_in_alop(struct InterCodes* , FILE*);
void add_space_in_assign(struct InterCodes* , FILE*);
void add_space_in_call(struct InterCodes* , FILE*);
void add_space_in_param(struct InterCodes* , FILE*);
void add_space_in_read(struct InterCodes* , FILE*);

void add_v(Operand , FILE*);
void add_t(Operand , FILE*);

int get_reg(Operand , FILE*);

#endif
