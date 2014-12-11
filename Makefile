CC=gcc -g -Wall -Wextra
CFLAGS=-std=c99 -Wall `xml2-config --cflags --libs` -lm -fPIC
SHELL=/bin/zsh

SOURCES=graph.c main.c dijkstra.c queue.c states.c utils.c fibonacci_heap.c
OBJECTS=$(SOURCES:%.c=%.o)
EXECUTABLE=dijkstra

PYFLAGS=-pthread -fPIC -fwrapv -O2 -Wall -fno-strict-aliasing -I/usr/include/python3.4m
PYDEPS=graph.c utils.c states.c dijkstra.c queue.c

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.o:
	$(CC) $(INCLUDES) -c $< $(CFLAGS) -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm $(SOURCES:%.c=%.o)
	rm pyjkstra.{so,o,c}

test: $(EXECUTABLE)
	@for i in tests/* ; do \
		echo "$$i :" ; \
		echo "Got :" ; \
		./$(EXECUTABLE) $$i ; \
		echo "Expected :" ; \
		cat tests_outputs/$${i#tests/} ; \
		if (diff <(./$(EXECUTABLE) $$i 2>&1) <(cat tests_outputs/$${i#tests/}) > /dev/null 2>&1) \
		then \
			echo "" ; \
			printf '\033[1;32;40m' ; \
			echo "[Passed]" ; \
			printf '\033[0m' ; \
		else \
			echo "" ; \
			printf '\033[1;31;40m' ; \
			echo "[Failed]" ; \
			printf '\033[0m' ; \
		fi ; \
		echo "" ; \
	done

pyjkstra.o: pyjkstra.pyx
	cython pyjkstra.pyx
	gcc  $(PYFLAGS) -c pyjkstra.c

py: $(PYDEPS:%.c=%.o) pyjkstra.o
	gcc -shared $(PYFLAGS) -o pyjkstra.so $^
