/**@<parser.c>::**/

#include <keywords.h>
#include <pseudocode.h>
#include <parser.h>

/*****************************************************************************
 *
 * mypas -> PROGRAM ID ; declarative imperative .
 *
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
		_varlist_head:
		varlist();
		match(':');
		typemod();
		match(';');
		if (lookahead == ID) { goto _varlist_head; }
	}
}
/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
void varlist(void)
{
	_id_head:
	match(ID);
	if (lookahead == ',') { match(','); goto _id_head; }
}
/*****************************************************************************
 * typemod -> BOOLEAN | INTEGER | REAL | DOUBLE
 *****************************************************************************/
void typemod(void)
{
	switch(lookahead) {
	case INTEGER:
	case REAL:
	case DOUBLE:
		match(lookahead); break;
	default:
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
void stmt(void)
{
	/**/int F_type = 0;/**/
	switch(lookahead) {
	case IF: 	ifstmt();  break;
	case WHILE: 	whlstmt(); break;
	case REPEAT: 	rptstmt(); break;
	default:
		/**/F_type = /**/F(F_type);
	}
}
/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 * whlstmt -> WHILE expr DO stmt
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
/*****************************************************************************
 * expr -> smpexpr [ relop smpexpr ]
 * relop -> "=" | ">=" | "<=" | "<>"
 *****************************************************************************/
int expr(int E_type);

/******************************
 * Type token convenction table
 * ----------------------------
 * bool		==	1
 * int32	==	2
 * int64	==	3
 * flt32	==	4
 * flt64	==	5
 ******************************/
 enum {
	 BOOL = 1,
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


int iscompat(int acc_type, int sel_type);

/* E -> ['+''-'] T { (+) T } */
int smpexpr(int E_type)
{
	/**/int signal = 0, oplus; int T_type;/**/
	if (lookahead == '+' || lookahead == '-') {
		/**/signal = lookahead;/**/
		/**/E_type = iscompat(E_type, signal);/**/

		match(lookahead);
	}

	/**/T_type = /**/T(E_type);
	/**/E_type = iscompat(E_type, T_type);/**/
	/**/if (signal == '-') negate(E_type);/**/

	while ( lookahead == '+' || lookahead == '-' ) { 
		/**/oplus = lookahead;/**/
		/**/E_type = iscompat(E_type, oplus);/**/
		/**/push(E_type);/**/

	 	match (lookahead); /**/T_type = /**/T(E_type);

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
int T(int T_type)
{ 
	/**/int F_type;/**/

	/**/F_type = /**/F(T_type);
	/**/T_type = iscompat(T_type, F_type);/**/

	while ( lookahead == '*' || lookahead == '/' ) {
		/**/int otimes = lookahead;/**/
		/**/T_type = iscompat(T_type, otimes);/**/
		/**/push(T_type);/**/

		match(lookahead); /**/F_type = /**/F(T_type);

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

/*  F ->  ( E )
 *      | n
 *      | v [ = E ] */

int F(int F_type)
{ 
	/**/char name[MAXIDLEN+1]; int E_type;/**/
	switch (lookahead) {
		case '(':
			match('('); /**/E_type = /**/smpexpr(F_type); match(')');
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
			match(ID);
			if (lookahead == ASGN) {
				match(ASGN); smpexpr(F_type);
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
