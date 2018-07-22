
CFLAGS += -g -O0
LDFLAGS += -rdynamic -lunwind -ldl

SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = a.out

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

