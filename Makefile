FLAGS = -Wall  -L./ -m32

#all: clean my_format

#my_format: my_format.c 
#	gcc ${FLAGS} my_format.c -o my_format -lm
	

#clean:
#	rm -f my_format

all: clean my_format

my_format: my_format.o fat12.h
	gcc -g my_format.o -o my_format -lm

my_format.o: my_format.c
	gcc -c my_format.c -Wall -o my_format.o

clean:
	rm -f my_format

