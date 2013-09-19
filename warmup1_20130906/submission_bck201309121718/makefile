warmup1: my402output.o my402sortlist.o my402util.o my402list.o
	gcc -o warmup1 -g my402output.o my402sortlist.o my402util.o my402list.o

my402output.o: my402output.c
	gcc -g -c -Wall my402output.c
my402sortlist.o: my402sortlist.c my402sortlist.h
	gcc -g -c -Wall my402sortlist.c
my402util.o: my402util.c
	gcc -g -c -Wall my402util.c
my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c

clean:
	rm -f *.o warmup1 
