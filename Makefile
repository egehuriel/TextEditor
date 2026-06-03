CC = gcc
CFLAGS = -Wall -g -I include
LIBS = -lncurses

TARGET = editor
BUILD = build

SRCS = 	src/main.c \
		src/editor.c \
		src/operations.c \
		src/ui.c \
		src/garbageCollection.c \
		
OBJS = $(patsubst src/%.c, $(BUILD)/%.o, $(SRCS))

all: $(BUILD) $(TARGET)

$(BUILD):
	@echo "\n-------Building-------\n"
	mkdir -p $(BUILD)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	@echo "\n-------Built-------\n"
	
$(BUILD)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\n-------Cleaning-------\n"
	rm -rf $(BUILD) $(TARGET)

clear:
	@echo "\n-------Clearing-------\n"
	rm -rf $(BUILD) $(TARGET)

run: all
	@echo "\n-------Running-------\n"
	./editor
	
.PHONY: all clean run