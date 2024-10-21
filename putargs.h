#ifndef PUTARGS_H_
#define PUTARGS_H_

#include <stdio.h>
#include "assembler.h"


char putarg_push(Dynamic_Token* token, size_t ind);

char putarg_pop(Dynamic_Token* token, size_t ind);


#endif // PUTARGS_H_