/**@<mypas.h>::**/

/******************************************
 * 
 * GRUPO 03
 * Guilherme Henrique Lorenzetti Simionato
 * Danillo Santos Miranda
 *
 *****************************************/

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <tokens.h>

extern int lookahead;
extern int column; // TODO: Add column counter for error messages
extern int linenumber;

void mypas(void);
int gettoken(FILE *);
