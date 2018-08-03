default: shell

shell.o: shell.c
	gcc -c shell.c -o shell.o

shell: shell.o
	gcc shell.o -o shell

clean:
	-rm -f shell.o
	-rm -f shell