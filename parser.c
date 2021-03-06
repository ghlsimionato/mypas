/**@<parser.c>::**/

#include <keywords.h>
#include <pseudocode.h>
#include <symtab.h>
#include <parser.h>

/******************************
 * Type token convenction table
 * ----------------------------
 * incomptbl	==	-1
 * void		==	0
 * bool		==	1
 * int32	==	2
 * int64	==	3
 * flt32	==	4
 * flt64	==	5
 ******************************/
enum {
	INCOMPTBL = -1,
	VOID,
	BOOL,
	INT32,
	INT64,
	FLT32,
	FLT64,
};
#define ILGLTYP	-1

 /***************************************************************************************
  *	   bool  int32  int64  flt32 flt64    +      -      *      /     OR   AND   NOT
  * bool   bool  -----  -----  ----- -----  -----  -----  -----  -----  bool  bool  bool
  * int32  ----  int32  int64  flt32 flt64  int32  int32  int32  int32  ----  ----  ----
  * int64  ----  int64  int64  flt32 flt64  int64  int64  int64  int64  ----  ----  ----
  * flt32  ----  flt32  flt32  flt32 flt64  flt32  flt32  flt32  flt32  ----  ----  ----
  * flt64  ----  flt64  flt64  flt64 flt64  flt64  flt64  flt64  flt64  ----  ----  ----
  ***************************************************************************************/

int iscompat(int acc_type, int syn_type)
{
	switch (acc_type) {
	case VOID:
		return syn_type;
	case BOOL:
		if (syn_type == BOOL) return BOOL;
		break;
	case INT32:
		if (syn_type >= INT32 && syn_type <= FLT64) return syn_type;
		break;
	case FLT32:
		if (syn_type > FLT32 && syn_type <= FLT64) return syn_type;
		if (syn_type == INT32) return FLT32;
		break;
	case FLT64:
		if (syn_type >= INT32 && syn_type <= FLT64) return FLT64;
	}
	return INCOMPTBL;
}

/*****************************************************************************
 * mypas -> PROGRAM ID ; declarative imperative .
 *****************************************************************************/
void mypas(void)
{
	match(PROGRAM);
	match(ID);
	match(';');
	declarative();
	imperative();
	match('.');
}
/*****************************************************************************
 * declarative -> vardecl sbrdecl
 *****************************************************************************/
void declarative(void)
{
	vardecl(); sbrdecl();
}
/*****************************************************************************
 * vardecl -> [ VAR varlist : typemod ;  { varlist : typemod ; } ]
 *****************************************************************************/
void vardecl(void)
{
	if (lookahead == VAR) {
		match(VAR);
		/**/int initial_pos = symtab_nextentry;/**/
		_varlist_head:
		varlist();
		match(':');
		/**/int type = /**/typemod();
		/**/for( ; initial_pos < symtab_nextentry; initial_pos++){symtab[initial_pos].type = type;}/**/
		match(';');
		if (lookahead == ID) { goto _varlist_head; }
	}
}
/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
/**/int sem_error = 0;/**/
void varlist(void)
{
	_id_head:
	/**/
	if (symtab_append(lexeme) < 0) {
		fprintf(stderr, "symtab_append: %s already defined. Fatal error.\n", lexeme);
		sem_error = -1;
	}
	/**/
	match(ID);
	if (lookahead == ',') { match(','); goto _id_head; }
}
/*****************************************************************************
 * typemod -> BOOLEAN | INTEGER | REAL | DOUBLE
 *****************************************************************************/
int typemod(void)
{
	/**/int type;/**/
	switch(lookahead) {
	case INTEGER:
		/**/type = INT32;	
	case REAL:
		/**/type = FLT32;/**/
	case DOUBLE:
		/**/type = FLT64;/**/
		match(lookahead); break;
	default:
		/**/type = BOOL;/**/
		match(BOOLEAN);
	}
}
/****************************************************************************
 * sbrdecl -> { procedure | function }
 ****************************************************************************/
void sbrdecl(void) 
{ 
	_sbrdecl_head:
	switch(lookahead) {
		case PROCEDURE: procedure(); goto _sbrdecl_head;
		case FUNCTION: function(); goto _sbrdecl_head;
	}
}
/****************************************************************************
 * procedure -> PROCEDURE sbrhead ; sbrtail
 ****************************************************************************/
void procedure(void)
{
	match(PROCEDURE); sbrhead(); match(';'); sbrtail();
}
/****************************************************************************
 * function -> FUNCTION sbrhead : typemod ; sbrtail
 ****************************************************************************/
void function(void)
{
	match(FUNCTION); sbrhead(); match(':'); typemod(); match(';'); sbrtail();
}
/*****************************************************************************
 * sbrhead -> ID formparm
 *****************************************************************************/
void sbrhead(void) { match(ID); formparm(); }
/*****************************************************************************
 * sbrtail -> declarative imperative ;
 *****************************************************************************/
void sbrtail(void) { declarative(); imperative(); match(';'); }
/*****************************************************************************
 * formparm -> [ ( [VAR] varlist : typemod { ; [VAR] varlist : typemod } ) ]
 *****************************************************************************/
void formparm(void)
{
	if (lookahead == '(') {
		match('(');
		_parm_head:
		if (lookahead == VAR) { match(VAR); }
		varlist();
		match(':');
		typemod();
		if (lookahead == ';') { match(';'); goto _parm_head; }
		match(')');
	}
}
/*****************************************************************************
 * imperative -> BEGIN stmt { ; stmt } END
 *****************************************************************************/
void imperative(void)
{
	match(BEGIN);
	_stmt_head:
	stmt();
	if (lookahead == ';') { match(';'); goto _stmt_head; }
}
/*****************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | F | <empty>
 *****************************************************************************/
int is_first_fact(void)
{
	switch (lookahead) {
	case '(':
	case ID:
	case UINT:
	case FLOAT:
	case OCT:
	case HEX:
	case '+':
	case '-':
	case NOT:
	case TRUE:
	case FALSE:
		return 1;
	}
	return 0;
}
void stmt(void)
{
	/**/int F_type = 0;/**/
	switch(lookahead) {
	case BEGIN:	imperative(); break;
	case IF: 	ifstmt();  break;
	case WHILE: 	whlstmt(); break;
	case REPEAT: 	rptstmt(); break;
	default:
		if (is_first_fact()) {
			/**/F_type = /**/fact(F_type);
		} else {
			;
		}
	}
}
/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *****************************************************************************/
/**/int labelcount = 1;/**/
void ifstmt(void)
{
	/**/int expr_type;/**/
	/**/int elsecount, endifcount;/**/
	match(IF); /**/expr_type = /**/expr(BOOL); match(THEN);
	/**/printf("\tgofalse .L%d\n", elsecount = endifcount = labelcount++);/**/
	stmt();
	if (lookahead == ELSE) {
		match(ELSE);
		/**/printf("\tgoto .L%d\n", endifcount = labelcount++);/**/ 
		/**/printf(".L%d:\n", elsecount);/**/
		stmt();
	}
	/**/printf(".L%d:\n", endifcount);/**/
}
/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
void whlstmt(void)
{
	/**/int expr_type, whilelbl, whendlbl;/**/
	match(WHILE);
	/**/printf(".L%d:\n", whilelbl = labelcount++);/**/
	/**/expr_type = /**/expr(BOOL);
	match (DO);
	/**/printf("\tgofalse .L%d\n", whendlbl = labelcount++);/**/
	stmt();
	/**/printf(".L%d:\n", whendlbl);/**/
}
/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
void rptstmt(void)
{
	/**/int expr_type;/**/
	match(REPEAT);
	stmt_head:
	stmt();
	if (lookahead == ';') { match(';'); goto stmt_head; }
	match(UNTIL); /**/expr_type = /**/expr(BOOL);
}
/*****************************************************************************
 * expr -> smpexpr [ relop smpexpr ]
 * relop -> "=" | ">=" | "<=" | "<>"
 *****************************************************************************/
int isrelop(void)
{
	switch(lookahead) {
	case '=': case '<': case '>': case GEQ: case LEQ: case NEQ:
		return 1;
	}
	return 0;
}
int expr(int expr_type)
{
	/**/int smpexpr1_type, smpexpr2_type;/**/
	/**/smpexpr1_type = /**/smpexpr(0);
	if (isrelop()) {
		/**/smpexpr2_type = /**/smpexpr(smpexpr1_type);
	} else {
		;
	}
}
/* smpexpr -> ['+''-'] T { (+) T } */
int smpexpr(int E_type)
{
	/**/int signal = 0, oplus; int T_type;/**/
	if (lookahead == '+' || lookahead == '-') {
		/**/signal = lookahead;/**/
		/**/E_type = iscompat(E_type, signal);/**/

		match(lookahead);
	}

	/**/T_type = /**/term(E_type);
	/**/E_type = iscompat(E_type, T_type);/**/
	/**/if (signal == '-') negate(E_type);/**/

	while ( lookahead == '+' || lookahead == '-' ) { 
		/**/oplus = lookahead;/**/
		/**/E_type = iscompat(E_type, oplus);/**/
		/**/push(E_type);/**/

	 	match (lookahead); /**/T_type = /**/term(E_type);

		/**/E_type = iscompat(E_type, T_type);/**/

		/**/
		switch(oplus) {
		case '+':
			add(E_type);
			break;
		default:
			subtract(E_type);
		}
		/**/
	}

	/**/return E_type;/**/
}

/* T -> F { (*) F } */
int term(int T_type)
{ 
	/**/int F_type;/**/

	/**/F_type = /**/fact(T_type);
	/**/T_type = iscompat(T_type, F_type);/**/

	while ( lookahead == '*' || lookahead == '/' ) {
		/**/int otimes = lookahead;/**/
		/**/T_type = iscompat(T_type, otimes);/**/
		/**/push(T_type);/**/

		match(lookahead); /**/F_type = /**/fact(T_type);

		/**/T_type = iscompat(T_type, F_type);/**/

		/**/
		switch(otimes) {
		case '*':
			multiply(T_type);
			break;
		default:
			divide(T_type);
		}
		/**/
	}

	/**/return T_type;/**/
}

/*  F ->  ( expr )
 *      | n
 *      | v [ = expr ] */

int fact(int F_type)
{ 
	/**/char name[MAXIDLEN+1]; int E_type; int var_descriptor;/**/
	switch (lookahead) {
		case '(':
			match('('); /**/E_type = /**/expr(F_type); match(')');
			break;
		case UINT:
			/**/mov(F_type, lexeme);/**/
			match(UINT);
			break;
		case FLOAT:
			match(FLOAT);
			break;
		default:
			/**/strcpy(name, lexeme);/**/
			/**/
			if ( (var_descriptor = symtab_lookup(name)) < 0) {
				fprintf(stderr, "symtab_lookup: %s not declared\n", name);
				sem_error = -2;
			}
			/**/
			match(ID);
			if (lookahead == ASGN) {
				match(ASGN); expr(F_type);
				/**/L_value(F_type, name);/**/
			} else {
				/**/R_value(F_type, name);/**/
				;
			}
	}
}

void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch: expected %d whereas found %d\n",
		expected, lookahead);
		exit(-2);
	}
}
