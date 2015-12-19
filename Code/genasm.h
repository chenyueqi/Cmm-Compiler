#ifndef __GEN__
#define __GEN__

#include "translate.h"

void common_output(FILE*);
void dec_process(FILE*);

void outasmlabel(struct InterCodes* , FILE*);
void outasmfunc(struct InterCodes* , FILE*);

#endif
