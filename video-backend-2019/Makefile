CC=cc
CFLAGS=-Wall -g -pthread

PROG=avcframes
SOURCES=avcframes.c

$(PROG): $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROG) $(SOURCES)

test: $(PROG)
	@python3 ./test.py
