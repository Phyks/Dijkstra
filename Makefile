CC=gcc -Wall -Wextra -O2
CFLAGS=-std=c99 -Wall `xml2-config --cflags --libs` -lm -fPIC
SHELL=/bin/zsh

SOURCES=graph.c main.c states.c utils.c
OBJECTS=$(SOURCES:%.c=%.o)
EXECUTABLE_QUEUE=dijkstra
EXECUTABLE_FIB=dijkstra_opt

SOURCES_QUEUE=$(SOURCES) queue.c dijkstra.c
SOURCES_FIB=$(SOURCES) fibonacci_heap.c dijkstra_opt.c

OBJECTS_QUEUE=$(SOURCES_QUEUE:%.c=%.o)
OBJECTS_FIB=$(SOURCES_FIB:%.c=%.o)

PYFLAGS=-pthread -fPIC -fwrapv -O2 -Wall -fno-strict-aliasing -I/usr/include/python3.4m
PYDEPS=graph.c utils.c states.c


$(EXECUTABLE_QUEUE): $(OBJECTS_QUEUE)
	$(CC) $(CFLAGS) $(OBJECTS_QUEUE) -o $@

$(EXECUTABLE_FIB): $(OBJECTS_FIB)
	$(CC) $(CFLAGS) $(OBJECTS_FIB) -o $@

.o:
	$(CC) $(INCLUDES) -c $< $(CFLAGS) -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm $(SOURCES:%.c=%.o)
	rm pyjkstra{,_opt}.{so,o,c}

test_queue: $(EXECUTABLE_QUEUE)
	@for i in tests/* ; do \
		echo "$$i :" ; \
		echo "Got :" ; \
		./$(EXECUTABLE_QUEUE) $$i ; \
		echo "Expected :" ; \
		cat tests_outputs/$${i#tests/} ; \
		if (diff <(./$(EXECUTABLE_QUEUE) $$i 2>&1) <(cat tests_outputs/$${i#tests/}) > /dev/null 2>&1) \
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

test_fib: $(EXECUTABLE_FIB)
	@for i in tests/* ; do \
		echo "$$i :" ; \
		echo "Got :" ; \
		./$(EXECUTABLE_FIB) $$i ; \
		echo "Expected :" ; \
		cat tests_outputs/$${i#tests/} ; \
		if (diff <(./$(EXECUTABLE_FIB) $$i 2>&1) <(cat tests_outputs/$${i#tests/}) > /dev/null 2>&1) \
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
	cython $^
	gcc  $(PYFLAGS) -c pyjkstra.c

pyqueue: $(OBJECTS_QUEUE:%.c=%.o) pyjkstra.o
	gcc -shared $(PYFLAGS) -o pyjkstra.so $^

pyfib: $(OBJECTS_FIB:%.c=%.o) pyjkstra.o
	gcc -shared $(PYFLAGS) -o pyjkstra.so $^
