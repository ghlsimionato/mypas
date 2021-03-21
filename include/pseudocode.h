/**@<pseudocode.h>::**/

#pragma once

#include <stdio.h>

#include <types.h>

extern int semantic_error;
/*
void L_value(int, const char *name);
void R_value(int, const char *name);
*/
void add(int);
void subtract(int);
void multiply(int);
void divide(int);
void not(void);
void cmp(int relop, int type, char *aux, char *acc);
void negate(int type);
void push(int);
void mov(int type, const char *src, const char *dest);
void gofalse(int);
void gotolabel(int);
void mklabel(int);

