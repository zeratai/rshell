DIR = bin
TARGET = $(addprefix $(DIR)/,rshell)
CC = g++
MAKE_DIR = mkdir -p $(DIR)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = main.cpp

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(MAKE_DIR)
	$(CC) $(OBJECTS) -o $(TARGET)

clean:
	-rm -f $(TARGET)
	-rmdir $(DIR)
