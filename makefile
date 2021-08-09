
hello:hello.o
	gcc hello.c -o hello

hello.o:hello.c
	gcc -c hello.c -o hello.o

