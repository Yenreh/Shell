ejecutar: clean shell
	./shell
shell: libshell.a
	gcc -L. -o shell mainShell.c -lshell
libshell.a: shell.o
	ar -cvq libshell.a shell.o
shell.o:
	gcc -c shell.c
clean:
	rm shell *.output -f lib*.a *.o
