#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct atomic_fifo_node {
        void *data;
        struct atomic_fifo_node *next;
};

struct atomic_fifo {
	struct atomic_fifo_node *tail, *head;
};

struct atomic_fifo *
atomic_fifo_init(void)
{
	struct atomic_fifo *queue = NULL;

	queue = malloc(sizeof(struct atomic_fifo));
	queue->tail = NULL;
	queue->head = NULL;

	return queue;
}

void
atomic_fifo_push(struct atomic_fifo *queue, void *data)
{
	struct atomic_fifo_node *node = NULL, **npp = NULL;

	node = malloc(sizeof(struct atomic_fifo_node));
	node->data = (void *) data;
	node->next = NULL;

	npp = &node;

	if (!queue->tail) {
		__atomic_store((struct atomic_fifo_node **) &queue->tail, npp, __ATOMIC_SEQ_CST);
		queue->head = queue->tail;
	} else {
		__atomic_store((struct atomic_fifo_node **) &queue->tail->next, npp, __ATOMIC_SEQ_CST);
		queue->tail = queue->tail->next;
	}

	return;
}

void *
atomic_fifo_pop(struct atomic_fifo *queue)
{
	struct atomic_fifo_node *node = NULL;
	void *data;

	if (queue->head) {
		__atomic_exchange(&queue->head, &queue->head->next, &node, __ATOMIC_SEQ_CST);
		data = node->data;
		free(node);
		return data;
	}

	return NULL;
}

int
main()
{
	struct atomic_fifo *myqueue = NULL;
	char buf[256], *str;
	int i;

	myqueue = atomic_fifo_init();

	for (i = 1 ; i < 101 ; i++) {
		sprintf(buf, "This is item no %d", i);
		str = strdup(buf);
		atomic_fifo_push(myqueue, str);
	}

	while ((str = atomic_fifo_pop(myqueue)) != NULL) {
		printf("Pop: %s\n", (char *) str);
	}

	return 0;
}
