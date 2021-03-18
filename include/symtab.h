/**@<symtab.h>::**/

#pragma once

#include <stdio.h>
#include <string.h>

#include <constants.h>

/**
* Struct to represent a symbol item in the symbol table
*TODO: document variables
*/
typedef struct __symtab__ {
	char name[MAXIDLEN+1];
	int type;
	int lex_level;
	/** access = 1 -> local; access = 2 -> parameter by value; access = 3 => parameter by reference **/
	int access; 
	/** objtype = 1 => variable; objtype = 2 => procedure; objtype = 3 => function**/
	int objtype;
	char offset[OFFSETSIZE+1];
} SYMTAB;

/* aula 23 - 1:00:00 +- */
/* 2:00:00 */
/** example for offset: -3(%rbp) or x[lex_level] **/
extern SYMTAB symtab[];

extern int symtab_nextentry;

int symtab_lookup(const char *, int);
int symtab_append(const char *, int, int, int);
int symtab_type_create(int, int);

