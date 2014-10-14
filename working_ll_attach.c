#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct atomic_fifo {
        void *data;
        struct atomic_fifo *next;
};

int
main()
{
	struct atomic_fifo *q = NULL, *t = NULL;
	char *d;

	printf("q before alloc: %p %p\n", q, &q);
	printf("t before alloc: %p %p\n\n", t, &t);

	t = malloc(sizeof(struct atomic_fifo));

	printf("t after alloc: %p %p\n", t, &t);

	d = strdup("foobar");

	t->data = (void *) d;
	t->next = NULL;

	printf("t after data add: %p %p\n", t, &t);
	printf("t data: %s\n", (char *) t->data);

	if (!q) {
		printf("q is null\n");
		__atomic_store(&q, &t, __ATOMIC_SEQ_CST);
	} else {
		printf("q is not null\n");
		__atomic_store(&q->next, &t, __ATOMIC_SEQ_CST);
		q = q->next;
	}

	printf("q after attach: %p %p\n", q, &q);
	printf("q data: %s\n\n", (char *) q->data);

	/* second */

	free(d);
	d = strdup("barfoo");

	t = malloc(sizeof(struct atomic_fifo));
	printf("t after alloc: %p %p\n", t, &t);
	t->data = (void *) d;
	t->next = NULL;
	printf("t after data add: %p %p\n", t, &t);
	printf("t data: %s\n", (char *) t->data);

	if (!q) {
		printf("q is null\n");
		__atomic_store(&q, &t, __ATOMIC_SEQ_CST);
	} else {
		printf("q is not null\n");
		__atomic_store(&q->next, &t, __ATOMIC_SEQ_CST);
		q = q->next;
	}

	printf("q after attach: %p %p\n", q, &q);
	printf("q data: %s\n\n", (char *) q->data);

	/* third */

	free(d);
	d = strdup("fuubar");

	t = malloc(sizeof(struct atomic_fifo));
	printf("t after alloc: %p %p\n", t, &t);
	t->data = (void *) d;
	t->next = NULL;
	printf("t after data add: %p %p\n", t, &t);
	printf("t data: %s\n", (char *) t->data);

	if (!q) {
		printf("q is null\n");
		__atomic_store(&q, &t, __ATOMIC_SEQ_CST);
	} else {
		printf("q is not null\n");
		__atomic_store(&q->next, &t, __ATOMIC_SEQ_CST);
		q = q->next;
	}

	printf("q after attach: %p %p\n", q, &q);
	printf("q data: %s\n\n", (char *) q->data);

	return 0;
}
