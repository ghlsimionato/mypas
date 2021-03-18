/**@<parser.h>::**/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <keywords.h>
#include <pseudocode.h>
#include <symtab.h>
#include <constants.h>
#include <tokens.h>


void mypas(void);
void declarative(void);
void imperative(void);
void vardecl(/**/int/**/);
void sbrdecl(void);
void varlist(/**/int/**/);
int typemod(void);
void procedure(void);
void function(void);
void sbrhead(/**/int/**/);
void sbrtail(void);
void formparm(/**/int/**/);
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
extern char lexeme[];
extern FILE *source;
