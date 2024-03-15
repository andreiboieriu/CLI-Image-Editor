CFLAGS=-Wall -Wextra

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

build: $(OBJS) -lm
	gcc $(CFLAGS) -o image_editor $(OBJS) -lm

pack:
	zip -FSr 312CA_Boieriu_Andrei_Teodor_Tema3.zip README Makefile *.c *.h

clean:
	rm -f image_editor *.gch $(OBJS)

.PHONY: pack clean
