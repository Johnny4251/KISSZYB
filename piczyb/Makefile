CC = gcc
CFLAGS = -lkisszyb
TARGET = piczyb.exe
SRC = piczyb.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
