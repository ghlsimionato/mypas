# GROUP 03
#
# Guilherme Henrique Lorenzetti Simionato
# Danillo Santos Miranda

# Default C-flags and inclusion of header files from /include directory with
# the -g flag to allow debugging
CFLAGS=-g -I./include

# creates executable by the recipe target (mypas) using the $@ automatic variable
# while the prerequistes (.o object files) are all referenced by the $^ automatic variable
mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o
	$(CC) -o $@ $^

# removes all generated object files
clean:
	$(RM) -v *.o

# mostlyclean recipe that invokes clean recipe
mostlyclean: clean
	$(RM) -v *~
