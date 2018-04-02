compileAll : myRead.o reverse.o
	gcc -g -fopenmp -o exe myRead.o reverse.o

myRead.o : myRead.c myRead.h
	gcc -g -fopenmp -c myRead.c

reverse.o : reverse.c myRead.h
	gcc -g -fopenmp -c reverse.c

clean :
	rm *.o exe