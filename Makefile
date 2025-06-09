STATIC = libringbuffer.a
SHARED = libringbuffer.so
SRC = src/ringbuffer.c
INC_DIR = include/
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -g -I$(INC_DIR)
PICFLAGS = -fPIC

all: $(STATIC) $(SHARED)

$(STATIC): $(OBJ)
	ar rcs $@ $^

$(SHARED): $(SRC)
	$(CC) $(CFLAGS) $(PICFLAGS) -shared -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(STATIC) $(SHARED) $(OBJ)
