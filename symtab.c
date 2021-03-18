/**@<symtab.c>::**/

#include <symtab.h>

SYMTAB symtab[MAXSTBSIZE];

int symtab_nextentry = 0;

int symtab_lookup(const char *query, int lex_level)
{
	int i;
	for (i = symtab_nextentry - 1; i > -1 && symtab[i].lex_level == lex_level; i--) {
		if (strcmp(query, symtab[i].name) == 0) return i;
	}

	return -1;
}

int symtab_append(const char *entry, int lex_level, int access, int objtype)
{
	int i = symtab_lookup(entry, lex_level);

	if (i > -1) {
	/** if i is greater than 1, then the symbol has already been declared and an error should be raised **/
		fprintf(stderr, "symtab_append: %s already defined in lexical level %ds. Fatal error.\n",
            entry, lex_level);
		return -1;
	}

	strcpy( symtab[symtab_nextentry].name, entry );
	symtab[symtab_nextentry].lex_level = lex_level;
	symtab[symtab_nextentry].access = access;
	symtab[symtab_nextentry].objtype = objtype;
	sprintf(symtab[symtab_nextentry].offset, "%s[%d]", entry, lex_level);
	return symtab_nextentry++;
}

void symtab_type_create(int type, int initial_pos)
{
		for( ; initial_pos < symtab_nextentry; initial_pos++){
			symtab[initial_pos].type = type;
		}
}
