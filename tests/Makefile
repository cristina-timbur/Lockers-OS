SRCS = $(wildcard *.c)
EXECS = $(patsubst %.c, bin/%, $(wildcard *.c)) 

.PHONY: all
all: $(EXECS)
	
bin/%: %.c
	gcc $< -L.. -llockers -o $@

.PHONY: clean

clean:
	rm -f bin/*

