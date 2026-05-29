CC = gcc
CFLAGS = -Wall -g -I include
LIBS = -lncurses

TARGET = editor

SRCS = 	src/main.c \
		src/editor.c \
		src/operations.c \
		src/ui.c \
		src/garbageCollection.c \
		
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o editor
	
run: all
	./editor
	
.PHONY: all clean run
