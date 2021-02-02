/**@<parser.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <tokens.h>

extern char lexeme[];

void mypas(void);
void declarative(void);
void imperative(void);
void vardecl(void);
void sbrdecl(void);
void varlist(void);
void typemod(void);
void procedure(void);
void function(void);
void sbrhead(void);
void sbrtail(void);
void formparm(void);
void stmt(void);
void ifstmt(void);
void whlstmt(void);
void rptstmt(void);
int expr(int E_type);

int smpexpr(int);
int T(int);
int F(int);

void match(int expected);
int gettoken(FILE *);
extern int lookahead;
extern FILE *source;
