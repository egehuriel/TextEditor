CC = gcc
CFLAGS = -Wall -g
LIBS = -lncurses

TARGET = editor

SRCS = 	main.c \
		editor.c \
		operations.c \
		ui.c \
		garbageCollection.c \
		
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

editor : main.o
	$(CC) $(CFLAGS) -o editor main.o $(LIBS)
	
clean:
	rm -f *.o editor
	
run: all
	./editor
	
.PHONY: all clean run
