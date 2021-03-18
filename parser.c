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
 * mypas -> PROGRAM ID ';' declarative imperative '.'
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
		/* if symtab returns a negative number, then an error has occured and the global
		*	error flag is raised, and code generation is interrupted
		*/
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
	return type;
}
/****************************************************************************
 * sbrdecl -> { procedure | function }
 ****************************************************************************/
void sbrdecl(void) 
{ 
	_sbrdecl_head:
	switch(lookahead) {
		case PROCEDURE: procedure(); goto _sbrdecl_head; break;
		case FUNCTION: function(); goto _sbrdecl_head; break;
		default: /** emulates epsilon-transition: **/;
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
	/**/int fact_type = 0;/**/
	switch(lookahead) {
	case BEGIN:	imperative(); break;
	case IF: 	ifstmt();  break;
	case WHILE: 	whlstmt(); break;
	case REPEAT: 	rptstmt(); break;
	default:
		if (is_first_fact()) {
			/**/fact_type = /**/fact(fact_type);
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
	/**/gofalse(elsecount = endifcount = labelcount++);/**/
	stmt();
	if (lookahead == ELSE) {
		match(ELSE);
		/**/gotolabel(endifcount = labelcount++);/**/
		/**/mklabel(elsecount);/**/
		stmt();
	}
	/**/mklabel(endifcount);/**/
}
/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
void whlstmt(void)
{
	/**/int expr_type, whilelbl, whendlbl;/**/
	match(WHILE);
	/**/mklabel(whilelbl = labelcount++);/**/
	/**/expr_type = /**/expr(BOOL);
	match (DO);
	/**/gofalse(whendlbl = labelcount++);/**/
	stmt();
	/**/gotolabel(whilelbl);/**/
	/**/mklabel(whendlbl);/**/
}
/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
void rptstmt(void)
{
	/**/int expr_type, rptlbl;/**/
	match(REPEAT);
	/**/mklabel(rptlbl = labelcount++);/**/
	stmt_head:
	stmt();
	if (lookahead == ';') { match(';'); goto stmt_head; }
	match(UNTIL); /**/expr_type = /**/expr(BOOL);
	/**/gofalse(rptlbl);/**/
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

	return expr_type;
}

/* smpexpr -> ['+''-'] T { (+) T } */
int smpexpr(int smpexpr_type)
{
	/**/int signal = 0, oplus, T_type;/**/
	if (lookahead == '+' || lookahead == '-') {
		/**/signal = lookahead;/**/
		/**/smpexpr_type = iscompat(smpexpr_type, signal);/**/

		match(lookahead);
	}

	/**/T_type = /**/term(smpexpr_type);
	/**/smpexpr_type = iscompat(smpexpr_type, T_type);/**/
	/**/if (signal == '-') negate(smpexpr_type);/**/

	while ( lookahead == '+' || lookahead == '-' ) { 
		/**/oplus = lookahead;/**/
		/**/smpexpr_type = iscompat(smpexpr_type, oplus);/**/
		/**/push(smpexpr_type);/**/

	 	match(lookahead); /**/T_type = /**/term(smpexpr_type);

		/**/smpexpr_type = iscompat(smpexpr_type, T_type);/**/

		/**/
		switch(oplus) {
		case '+':
			add(smpexpr_type);
			break;
		default:
			subtract(smpexpr_type);
		}
		/**/
	}

	/**/return smpexpr_type;/**/
}

/* T -> F { (*) F } */
int term(int term_type)
{ 
	/**/int fact_type;/**/

	/**/fact_type = /**/fact(term_type);
	/**/term_type = iscompat(term_type, fact_type);/**/

	while ( lookahead == '*' || lookahead == '/' ) {
		/**/int otimes = lookahead;/**/
		/**/term_type = iscompat(term_type, otimes);/**/
		/**/push(term_type);/**/

		match(lookahead); /**/fact_type = /**/fact(term_type);

		/**/term_type = iscompat(term_type, fact_type);/**/

		/**/
		switch(otimes) {
		case '*':
			multiply(term_type);
			break;
		default:
			divide(term_type);
		}
		/**/
	}

	/**/return term_type;/**/
}

/*  F ->  ( expr )
 *      | n
 *      | v [ = expr ] */

int fact(int fact_type)
{ 
	/**/char name[MAXIDLEN+1]; int smpexpr_type; int var_descriptor;/**/
	switch (lookahead) {
		case '(':
			match('('); /**/smpexpr_type = /**/expr(fact_type); match(')');
			break;
		case UINT:
			/**/mov(fact_type, lexeme);/**/
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
				match(ASGN); expr(fact_type);
				/**/L_value(fact_type, name);/**/
			} else {
				/**/R_value(fact_type, name);/**/
				;
			}
	}

	return fact_type;
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
