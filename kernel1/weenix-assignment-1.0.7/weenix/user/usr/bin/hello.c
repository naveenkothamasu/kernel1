/*
 * The classic hello world program.
 * If you can run this successfully, you should high-five
 * everyone nearby. You can also shout loudly. People
 * will understand.
 */

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
				
        open("/dev/tty0", O_RDONLY, 0);
	open("/dev/tty0", O_WRONLY, 0);
	
        write(2, "aaswerftgh\n", 10);
	
        return 0;
}

