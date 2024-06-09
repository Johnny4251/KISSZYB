CC = gcc
CFLAGS = -lkisszyb
TARGET1 = piczyb.exe
SRC1 = piczyb.c
TARGET2 = picrecv.exe
SRC2 = picrecv.c

# Compile both targets by default
all: $(TARGET1) $(TARGET2)

$(TARGET1): $(SRC1)
	$(CC) $(SRC1) -o $(TARGET1) $(CFLAGS)

$(TARGET2): $(SRC2)
	$(CC) $(SRC2) -o $(TARGET2) $(CFLAGS)

clean:
	rm -f $(TARGET1) $(TARGET2)

