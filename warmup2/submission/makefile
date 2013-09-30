warmup2: my402list.o my402threads.o my402util.o my402input.o my402stats.o
	gcc -o warmup2 -g -pthread my402list.o my402threads.o my402util.o my402input.o my402stats.o -lm

my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c
my402threads.o: my402threads.c my402threads.h
	gcc -g -c -Wall -pthread my402threads.c
my402util.o: my402util.c my402util.h
	gcc -g -c -Wall my402util.c
my402input.o: my402input.c
	gcc -g -c -Wall my402input.c
my402stats.o: my402stats.c
	gcc -g -c -Wall my402stats.c

clean:
	rm -f *.o warmup2 
