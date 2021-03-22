/**@<mypas.c>::**/

/******************************************
 * 
 * GRUPO 03
 * Guilherme Henrique Lorenzetti Simionato
 * Danillo Santos Miranda
 *
 *****************************************/

#include <mypas.h>

FILE *source;

/**
 * Main function that calls the starting grammar production (mypas)
 * for the given filename, which is received as a command line argument
 */
int main(int argc, char const *argv[])
{
	source = fopen (argv[1], "r");

	switch (argc) {
		case 1:
			source = stdin;
			break;
		default:
			if (source == NULL) {
				fprintf (stderr, 
					"%s: cannot open... exiting with error status\n", 
					argv[1]);
				exit (-1);
			}
	}

	
	lookahead = gettoken(source);

	mypas();

	exit(0);
}
