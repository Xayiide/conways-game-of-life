CC      = gcc
CFLAGS  = -Wall -O0 -g
TARGET  = conway

SOURCES = conway.c
OBJECTS = $(SOURCES:.c=.o)


.PHONY: all clean re


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@  $^
	@rm -f $(OBJECTS)


clean:
	@rm -f $(TARGET)
	@echo "clean"

re: clean all
