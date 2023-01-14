DEPS = $(wildcard include/*.h)
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))

../liblockers.a: $(OBJS)
	ar -rcs $@ $^

obj/%.o: src/%.c $(DEPS)
	gcc -c -o $@ $<

.PHONY: clean

clean:
	rm -f obj/*.o
	rm -f ../liblockers.a