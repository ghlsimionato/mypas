/**@<mypas.h>::**/

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <tokens.h>

extern int lookahead;
extern int column;
extern int linenumber;

void mypas(void);
int gettoken(FILE *);
