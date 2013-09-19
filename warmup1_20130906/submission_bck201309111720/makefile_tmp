#compile the modules seperately

# a separate rule to link all the modules together
# to receive full credit for separate compilation
#	to create an executable, at a minimum, you must run the 
# compiler at least twice and the linker once
# compile modules separately

	
clean:
	rm *.o
warmpup1: my402list.o my402output.o my402sortlist.o my402util.o
	gcc -o warmup1 -g my402list.o my402output.o my402sortlist.o my402util.o
main.o: my402list.h
	gcc -g -c -Wall my402list.c my402output.c my402sortlist.c my402util.c
my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c
my402output.o: my402output.c
	gcc -g -c -Wall my402output.c
my402sortlist.o: my402sortlist.c my402sortlist.h
	gcc -g -c -Wall my402sortoutput.c
my402util.o: my402util.c
	gcc -g -c -Wall my402util.c
