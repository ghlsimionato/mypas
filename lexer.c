/**@<lexer.c>::**/
// Tue Nov 17 07:55:34 PM -03 2020
/*
 * this is the start project for lexical analyser to be used
 * as the interface to the parser of the project mybc (my basic
 * calculator).
 *
 * In order to have a pattern scan we have first to implement
 * a method to ignore spaces.
 */


#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <lexer.h>


int lookahead;
char lexeme[MAXIDLEN+1];
int column = 1; // TODO: Make column counter
int linenumber = 1;

/**
 * Function to skip unused characters
 * (such as space characters and anything inside a comment block)
 */
void skipunused(FILE *tape)
{
	int head;
	_skipunused:
 	while ( isspace( head = getc(tape) ) )
	{
		if (head == '\n') linenumber++;
	}
	/** Emulates a epsilon-transition: **/
    ungetc ( head, tape );

    /** Skip comments **/
	if ( (head = getc(tape)) == '{') {
        while ( (head = getc(tape)) != '}' && head != EOF) {
            if (head == '\n') linenumber++;
        }
        
        if (head == '}') {
            goto _skipunused;
        }
    }
}
/* Now we need a predicate function to recognize a string
 * begining with a letter (alpha) followed by zero or more
 * digits and letters.
 *
 * REGEX: [A-Za-z][A-Za-z0-9]*
 *
 * isalpha(x) returns 1 if x \in [A-Za-z]
 *            returns 0 otherwise
 * isalnum(x) returns 1 if x \in [A-Za-z0-9]
 *            returns 0 otherwise
 */
int isID(FILE *tape)
{
	int i = 0;
	if ( isalpha ( lexeme[i] = getc (tape) ) ) {
		i++;
		while ( isalnum ( lexeme[i] = getc (tape) ) ) {
			i++;
		}
		ungetc ( lexeme[i], tape );
		lexeme[i] = 0;
		int token = iskeyword(lexeme);
		if (token) return token;
		return ID;
	}
	ungetc ( lexeme[i], tape );
	lexeme[i] = 0;
	return 0;
}
/* Next, we have to implement a method to recognize decimal
 * pattern (unsigned int)
 * 
 * REGEX: [1-9][0-9]* | 0
 */
int isUINT(FILE *tape)
{
	int i = 0;
	if ( isdigit ( lexeme[i] = getc (tape) ) ) {
		if ( lexeme[i] == '0' ) {
			i++;
			lexeme[i] = 0;
			;
		} else {
			i++;
			while ( isdigit ( lexeme[i] = getc (tape) ) ) {
				i++;
			}
			ungetc ( lexeme[i], tape );
			lexeme[i] = 0;
		}
		return UINT;
	}
	ungetc ( lexeme[i], tape );
	return 0;
}
/** Trabalho de 08/12/2020: desenvolver um analisador léxico para ponto flutuante
 *
 * REGEX:
 * ( uint '.' [0-9]*  |  '.' [0-9]+ ) ee?  |  uint ee
 * uint = [1-9][0-9]* | 0
 * ee = [eE] ['+''-']? [0-9]+
 */
int isNUM(FILE *tape)
{
	int token = 0, i = 0;
	if ( (token = isUINT(tape)) ) {
		i += strlen(lexeme);
		if ( (lexeme[i] = getc(tape)) == '.' ) {
			i++;
			token = FLOAT;
			while ( isdigit(lexeme[i] = getc(tape)) ) {
				i++;
			}
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
		} else {
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
	} else if ( (lexeme[i] = getc(tape)) == '.' ) {
		i++;
		/** decimal point has been already read **/
		if ( isdigit(lexeme[i] = getc(tape)) ) {
			i++;
			token = FLOAT;
			while ( isdigit(lexeme[i] = getc(tape)) ) {
				i++;
			}
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
		} else {
			/** after decimal point another char, non-digit has been read **/
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
			ungetc ('.', tape);
		}
	} else {
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
	}
	int eE;
	if ( token > 0 ) {
		if ( toupper(lexeme[i] = eE = getc(tape)) == 'E' ) {
			i++;
			int plusminus;
			if ( (lexeme[i] = plusminus = getc(tape)) == '+' || plusminus == '-' ) {
				i++;
			} else {
				ungetc (plusminus, tape);
				plusminus = 0;
			}
			if ( isdigit(lexeme[i] = getc(tape)) ) {
				i++;
				token = FLOAT;
				while ( isdigit(lexeme[i] = getc(tape)) ) {
					i++;
				}
				ungetc (lexeme[i], tape);
				lexeme[i] = 0;
			} else {
				ungetc (lexeme[i], tape);
				i--;
				if (plusminus) {
					i--;
					ungetc(plusminus, tape);
				}
				i--;
				ungetc(eE, tape);
			}
		} else {
			ungetc(eE, tape);
		}
	}
	lexeme[i] = 0;
	return token;
}
/* Octal pattern is defined as
 * REGEX: 0[0-7]+
 */
int isOCT(FILE *tape)
{
	int i = 0;

	if ( ( lexeme[i] = getc (tape) ) == '0' ) {
		i++;
		if ( isdigit( lexeme[i] = getc (tape) ) && lexeme[i] <= 7 ) {
			i++;
			while  ( isdigit( lexeme[i] = getc (tape) ) && lexeme[i] <= 7 ) {
				i++;
			}
			ungetc( lexeme[i], tape );
			lexeme[i] = 0;
			return OCT;
		} else {
			ungetc( lexeme[i], tape );
			i--;
			ungetc( '0', tape );
			lexeme[i] = 0;
			return 0;
		}
	}
	ungetc( lexeme[i], tape );
	lexeme[i] = 0;
	return 0;
}
/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int i = 0;
	if ( ( lexeme[i] = getc (tape) ) == '0' ) {
		i++;
		if ( toupper( lexeme[i] = getc (tape) ) == 'X' ) {
			i++;
			if ( isxdigit ( lexeme[i] = getc (tape) ) ) {
				i++;
				while ( isxdigit ( lexeme[i] = getc (tape) ) ) {
					i++;
				}
				ungetc ( lexeme[i], tape);
				lexeme[i] = 0;
				return HEX;
			} else {
				ungetc ( lexeme[i], tape );
				i--;
				ungetc ( lexeme[i], tape );
				i--;
				ungetc ( lexeme[i], tape );
				lexeme[i] = 0;
				return 0;
			}
		}
		ungetc ( lexeme[i], tape );
		ungetc ( '0', tape );
		return 0;
	}
	ungetc (lexeme[i], tape);
	lexeme[i] = 0;
	return 0;
}
int isASGN(FILE *tape)
{
	int i = 0;
	if ( (lexeme[i] = getc(tape)) == ':' ) {
		i++;
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			i++;
			lexeme[i] = 0;
			return ASGN;
		}
		ungetc(lexeme[i], tape); i--;
	}
	ungetc(lexeme[i], tape);
	return 0;
}
int isRELOP(FILE *tape)
{
	int i = 0;
	switch ( (lexeme[i] = getc(tape)) ) {
	case '=':
		i++;
		lexeme[i] = 0; return '=';
	case '>':
		i++;
		if ( (lexeme[i] = getc(tape)) == '=') {
			i++;
			lexeme[i] = 0;
			return GEQ;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '>';
	case '<':
		i++;
		if ( (lexeme[i] = getc(tape)) == '=') {
			i++;
			lexeme[i] = 0;
			return LEQ;
		}
		if ( lexeme[i] == '>') {
			i++;
			lexeme[i] = 0;
			return NEQ;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '<';
	}
	ungetc(lexeme[i], tape);
	return lexeme[i] = 0;
}

int gettoken(FILE *source)
{
	int token;

	skipunused(source);

	if ( (token = isID(source)) ) return token;

	if ( (token = isOCT(source)) ) return token;

	if ( (token = isHEX(source)) ) return token;

	if ( (token = isNUM(source)) ) return token;

	if ( (token = isASGN(source)) ) return token;

	if ( (token = isRELOP(source)) ) return token;

	token = getc(source);

	return token;
}
