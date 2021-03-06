/**@<symtab.c>::**/
#include <string.h>
#include <symtab.h>

SYMTAB symtab[MAXSTBENTRIES];

int symtab_nextentry = 0;

int symtab_lookup(const char *query)
{
	int i;
	for (i = symtab_nextentry - 1; i > -1; i--) {
		if (strcmp(query, symtab[i].name) == 0) break;
	}
	return i;
}

int symtab_append(const char *entry)
{
	int i = symtab_lookup(entry);
	if (i > -1) return -1;
	strcpy( symtab[symtab_nextentry].name, entry );
	return symtab_nextentry++;
}


