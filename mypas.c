/**@<mypas.c>::**/

#include <stdio.h>
#include <stdlib.h>

#include <tokens.h>
#include <mypas.h>

FILE *source;

/* TODO: Document this file */
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
