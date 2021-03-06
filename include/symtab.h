/**@<symtab.h>::**/

#pragma once

#include <constants.h>

typedef struct __symtab__ {
	char name[MAXIDLEN+1];
	int  type;
	char offset[OFFSETSIZE+1];
} SYMTAB;

extern SYMTAB symtab[];

extern int symtab_nextentry;

int symtab_lookup(const char *);

int symtab_append(const char *);


