DIR = bin
TARGET = $(addprefix $(DIR)/,rshell)
CC = g++
CCFLAGS = -Wall -Werror -ansi -pedantic
MAKE_DIR = mkdir -p $(DIR)

.PHONY: default all clean

default: $(TARGET)
all: default
rshell: default

OBJECTS = src/main.cpp

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(MAKE_DIR)
	$(CC) $(CCFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	-rm -f $(TARGET)
	-rmdir $(DIR)
