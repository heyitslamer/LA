CFLAGS=-Wall -g -ansi -Wextra -pedantic -O2
FICHEIROS=cgi.h estado.c estado.h exemplo.c menu.c about.c Makefile

install: exemplo
	sudo cp exemplo menu about /usr/lib/cgi-bin
	touch install

exemplo: exemplo.o estado.o menu.o about.o
	cc -o exemplo exemplo.o estado.o
	cc -o menu menu.c
	cc -o about about.c

exemplo.zip: $(FICHEIROS)
	zip -9 exemplo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o exemplo menu about Doxyfile latex html install

estado.o: estado.c estado.h
exemplo.o: exemplo.c cgi.h estado.h
about.o: about.c
menu.o: menu.c
