CC=gcc
BIN=./bin
CFLAGS=-g -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code

PROG=sigcatch forkprio sched-threads benchmark

.PHONY: all
all: $(PROG)

%: %.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(PROG)

zip:
	git archive --format zip --output ${USER}-lab03.zip HEAD

html:
	pandoc -o README.html -f gfm README.md
