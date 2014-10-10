#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct atomic_fifo {
	void *data;
	struct atomic_fifo *next;
};

struct atomic_fifo *
atomic_fifo_init()
{
	struct atomic_fifo *af;

	af = malloc(sizeof(struct atomic_fifo));

	af->data = NULL;
	af->next = NULL;

	printf("af alloc and init: %p %p\n", af, &af);

	return af;
}

void
atomic_fifo_put(struct atomic_fifo *af, void *item)
{
	struct atomic_fifo *af_item;

	af_item = malloc(sizeof(struct atomic_fifo));
	af_item->data = item;
	af_item->next = NULL;

	__atomic_store(&af->next, &af_item, __ATOMIC_SEQ_CST);
}

int
main()
{
	struct atomic_fifo *myqueue = NULL, *h = NULL;
	void *mydata1, *mydata2, *mydata3;

	printf("myqueue uninitilized: %p %p\n", myqueue, &myqueue);
	printf("myqueue handle uninitilized: %p %p\n", h, &h);

	myqueue = atomic_fifo_init();
	h = myqueue;

	printf("myqueue: %p %p\n", myqueue, &myqueue);
	printf("myqueue handle: %p %p\n", h, &h);

	/* add first message */
	mydata1 = strdup("foobar");
	atomic_fifo_put(myqueue, mydata1);
	printf("\"%s\"\n", (char *) myqueue->next->data);

	/* alloc space for next element */
	myqueue = realloc(myqueue, sizeof(struct atomic_fifo) * 2);
	myqueue = myqueue->next;
	printf("myqueue: %p %p\n", myqueue, &myqueue);

	/* add second message */
	mydata2 = strdup("barfoo");
	atomic_fifo_put(myqueue, mydata2);
	printf("\"%s\"\n", (char *) myqueue->next->data);

	/* alloc space for next element */
	myqueue = realloc(myqueue, sizeof(struct atomic_fifo) * 3);
	myqueue = myqueue->next;
	printf("myqueue: %p %p\n", myqueue, &myqueue);

	/* add third message */
	mydata3 = strdup("fuubar");
	atomic_fifo_put(myqueue, mydata3);
	printf("\"%s\"\n", (char *) myqueue->next->data);

	return 0;
}
