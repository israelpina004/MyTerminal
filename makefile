all: main.o shell.o redirections.o
	gcc -o program main.o shell.o redirections.o
main.o: main.c shell.h redirections.h
	gcc -c main.c
shell.o: shell.c shell.h redirections.h
	gcc -c shell.c
redirections.o: redirections.c redirections.h shell.h
	gcc -c redirections.c
run:
	./program
clean:
	rm -rf *.o program
