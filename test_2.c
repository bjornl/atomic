#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *bar;

int main(int argc, char *argv[])
{
	char *foo;

	foo = malloc(4);
	sprintf(foo, "hello");
	printf("foo: \"%s\"\n", foo);

	/* lock-less atomic copy of the pointer address
	   of foo to the global pointer bar */
	__atomic_store(&bar, &foo, __ATOMIC_SEQ_CST);

	printf("bar: \"%s\"\n", bar);

	printf("Address of foo: %p, bar: %p\n", foo, bar); 

	return 0;
}
