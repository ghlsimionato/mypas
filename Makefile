# Default C-flags and inclusion of header files from /include directory
CFLAGS=-g -I./include

# TODO: Comment makefile
mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o
	$(CC) -o $@ $^

# removes all generated object files
clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~
