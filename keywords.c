#include <string.h>
#include <keywords.h>

char *keyword[] = { 
	"begin",
	"not",
	"and",
	"or",
	"div",
	"mod",
	"program",
	"procedure",
	"function",
	"var",
	"boolean",
	"integer",
	"real",
	"double",
	"if",
	"then",
	"else",
	"while",
	"do",
	"repeat",
	"until",
	"true",
	"false",
	"end",
};

int iskeyword(const char *name)
{
	int i;
	for (i = BEGIN; i < END + 1; i++) {
		if (strcmp(keyword[i - BEGIN], name) == 0) return i;
	}
	return 0;
}
