#compile the modules seperately

# a separate rule to link all the modules together
# to receive full credit for separate compilation
#	to create an executable, at a minimum, you must run the 
# compiler at least twice and the linker once
# compile modules separately

	
clean:
	rm *.o
warmpup1: main.o my402list.o
	gcc -o warmup1 -g main.o my402list.o 
main.o: main.c my402list.h
	gcc -g -c -Wall main.c
my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c 
