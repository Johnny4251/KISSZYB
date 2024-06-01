# Compiler and linker variables
CC=gcc
AR=ar
CFLAGS=-Wall -c
LIBS=

# Source, object, and library files
SRC=kisszyb.c
OBJ=$(SRC:.c=.o)
LIB=libkisszyb.a

# Installation directories
LIBDIR=/usr/local/lib
INCDIR=/usr/local/include

# Default target
all: $(LIB)

# Rule to make the object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

# Rule to make the static library
$(LIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

# Install the library and header file
install: $(LIB)
	sudo cp $(LIB) $(LIBDIR)
	sudo cp kisszyb.h $(INCDIR)
	sudo ldconfig

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(LIB)

# Phony targets
.PHONY: all install clean

