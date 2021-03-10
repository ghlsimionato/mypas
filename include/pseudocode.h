/**@<pseudocode.h>::**/
#pragma once

void L_value(int, const char *name);
void R_value(int, const char *name);
void add(int);
void subtract(int);
void multiply(int);
void divide(int);
void negate(int type);
void push(int);
void mov(int type, const char *src);
void gofalse(int);
void gotolabel(int);
void mklabel(int);

