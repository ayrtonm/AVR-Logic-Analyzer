CC = gcc
CFLAGS = #-g -O3
SRCS = host.c draw.c
OBJS = $(SRCS:.c=.o)
LIBS = -lSDL -lusb
MAIN = main

all: $(MAIN)
	@echo emulator has been compiled
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(MAIN)
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o $(MAIN)
