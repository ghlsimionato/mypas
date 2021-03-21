/**@<parser.h>::**/

#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <tokens.h>
#include <types.h>
#include <constants.h>
#include <keywords.h>
#include <symtab.h>
#include <pseudocode.h>

void declarative(void);
void imperative(void);
void vardecl(void);
void sbpdecl(void);
void varlist(void);
int typemod(void);
void sbphead(void);
void sbptail(void);
void formparm(void);
void stmt(void);
void ifstmt(void);
void whlstmt(void);
void rptstmt(void);
int expr(int);
int smpexpr(int);
int term(int);
int fact(int);


void match(int expected);
int gettoken(FILE *);

extern int lookahead;
extern int linenumber;
extern int column;
extern char lexeme[];
extern FILE *source;
