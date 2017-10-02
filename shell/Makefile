all: shell

CFLAGS = -g -O2

shell: shell.o mytoc.o stringHelper.o
	cc -o shell shell.o mytoc.o stringHelper.c
	./shell
clean:
	rm -f *.o shell shell.exe

cleanBackups:
	rm -f *~ 
