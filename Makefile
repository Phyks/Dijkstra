CC?=gcc -g -Wall -Wextra
CFLAGS?=-std=c99 -Wall

SOURCES=graph.c main.c dijkstra.c queue.c states.c fibonacci_heap.c
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm -rf $(EXECUTABLE)

test: $(EXECUTABLE)
	@for i in tests/* ; do \
		echo "$$i :" ; \
		./$(EXECUTABLE) $$i ; \
		echo "" ; \
	done
