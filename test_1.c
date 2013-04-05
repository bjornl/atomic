#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int a, b;

	a = 5;

	/* increments and then read the value */
	b = __atomic_add_fetch(&a, 2, __ATOMIC_SEQ_CST);

	printf("a=7: \"%d\", b=7: \"%d\"\n", a, b);

	a = 5;

	/* reads the old value and then increments with 2 */
	b = __atomic_fetch_add(&a, 2, __ATOMIC_SEQ_CST);

	printf("a=7: \"%d\" b=5: \"%d\"\n", a, b);

	return 0;
}
