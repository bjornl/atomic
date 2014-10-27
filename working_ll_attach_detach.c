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
	struct atomic_fifo *q = NULL, *t = NULL, *h = NULL, *r = NULL;
	struct atomic_fifo **tp;
	char *d;
	// int i;

	printf("q before alloc: %p %p\n", q, &q);
	printf("t before alloc: %p %p\n\n", t, &t);

	t = malloc(sizeof(struct atomic_fifo));
	printf("t after alloc: %p %p\n", t, &t);
	d = strdup("foobar");
	t->data = (void *) d;
	t->next = NULL;
	printf("t->next: %p %p\n", t->next, &t->next);

	printf("1 tp: %p %p\n\n", tp, &tp);
	tp = &t;
	printf("2 tp: %p %p\n\n", tp, &tp);

	__atomic_store((struct atomic_fifo **) &q, tp, __ATOMIC_SEQ_CST);

	h = q;

	printf("Data: %s\n", (char *) q->data);
	printf("q->next: %p %p\n\n", q->next, &q->next);

	/* second */

	t = malloc(sizeof(struct atomic_fifo));
	printf("t after alloc: %p %p\n", t, &t);
	d = strdup("barfoo");
	t->data = (void *) d;
	t->next = NULL;
	printf("t->next: %p %p\n", t->next, &t->next);

	tp = &t;

	__atomic_store((struct atomic_fifo **) &q->next, tp, __ATOMIC_SEQ_CST);

	q = q->next;
	printf("Data: %s\n", (char *) q->data);

	/* third */

	t = malloc(sizeof(struct atomic_fifo));
	printf("t after alloc: %p %p\n", t, &t);
	d = strdup("fuubar");
	t->data = (void *) d;
	t->next = NULL;
	printf("t->next: %p %p\n", t->next, &t->next);

	tp = &t;

	__atomic_store((struct atomic_fifo **) &q->next, tp, __ATOMIC_SEQ_CST);

	q = q->next;
	printf("Data: %s\n", (char *) q->data);

	/*
	printf("Data dump:\n");
	for (i = 1 ; i < 4 ; i++) {
		printf("%d: %s\n", i, (char *) h->data);
		h = h->next;
	}
	*/

	printf("\nPerform detach..\n\n");

	printf("Take the first item off the list\n");
	printf("h before read: %p %p\n", h, &h);
	printf("r before read: %p %p\n", r, &r);
	__atomic_exchange(&h, &h->next, &r, __ATOMIC_SEQ_CST);
	printf("r after read: %p %p\n", r, &r);
	printf("h after read: %p %p\n", h, &h);
	printf("Data: %s\n", (char *) r->data);
	free(r);

	printf("Take the second item off the list\n");
	printf("h before read: %p %p\n", h, &h);
	printf("r before read: %p %p\n", r, &r);
	__atomic_exchange(&h, &h->next, &r, __ATOMIC_SEQ_CST);
	printf("r after read: %p %p\n", r, &r);
	printf("h after read: %p %p\n", h, &h);
	printf("Data: %s\n", (char *) r->data);
	free(r);

	printf("Take the third item off the list\n");
	printf("h before read: %p %p\n", h, &h);
	printf("r before read: %p %p\n", r, &r);
	__atomic_exchange(&h, &h->next, &r, __ATOMIC_SEQ_CST);
	printf("r after read: %p %p\n", r, &r);
	printf("h after read: %p %p\n", h, &h);
	printf("Data: %s\n", (char *) r->data);
	free(r);

	return 0;
}
