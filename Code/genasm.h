#ifndef __GEN__
#define __GEN__

#include "translate.h"

void common_output(FILE*);
void dec_process(FILE*);

typedef struct Operand_in_reg* Operand_in_reg;

struct Operand_in_reg
{
	Operand op;
	Operand_in_reg next;
};

struct Reg
{
	Operand_in_reg head;
	int Cnt;
};

struct Reg reg_t[10];	//use freely belong to caller
struct Reg reg_s[9];	//use freely belong to callee
struct Reg reg_a[4];	//for parameters
struct Reg reg_v[2];	//for return value


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

int get_reg(Operand , FILE*);

#endif
