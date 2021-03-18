/**@<keywords.h>::**/

#pragma once

#include <string.h>


int iskeyword(const char *);

enum {
	BEGIN =  10001,
	NOT,
	AND,
	OR,
	DIV,
	MOD,
	PROGRAM,
	PROCEDURE,
	FUNCTION,
	VAR,
	BOOLEAN,
	INTEGER,
	REAL,
	DOUBLE,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	REPEAT,
	UNTIL,
	TRUE,
	FALSE,
	END,
};
