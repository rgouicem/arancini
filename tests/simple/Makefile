CFLAGS= -Wall -O0 -g

all: run

simple: simple.c
	$(CC) $(CFLAGS) -I../../include -L../../libtcg -ltcg-x86_64 -o $@ $<

hello: hello.c
	$(CC) $(CFLAGS) -o $@ $<

run: simple hello
	LD_LIBRARY_PATH=../../libtcg ./simple ./hello

clean:
	rm -rf *~ *.o

mrproper: clean
	rm -rf simple hello

.PHONY: clean mrproper run all
