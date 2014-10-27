CFLAGS = -Wall -g
OBJECTS = test_1 test_2 atomic_fifo_test working_ll_attach working_ll_attach_detach

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
