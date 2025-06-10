STATIC = libringbuffer.a
SHARED = libringbuffer.so
SRC = src/ringbuffer.c
INC_DIR = include/

PREFIX ?= /usr
INCLUDEDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib

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

install:
	mkdir -p $(DESTDIR)/usr/include
	cp -r include/* $(DESTDIR)/usr/include/
	mkdir -p $(DESTDIR)/usr/lib64
	cp libringbuffer.a $(DESTDIR)/usr/lib64/
	cp libringbuffer.so $(DESTDIR)/usr/lib64/

clean:
	rm -f $(STATIC) $(SHARED) $(OBJ)
