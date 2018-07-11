
CFLAGS += -I. -L. -g -O0 -fPIC
LDFLAGS += -ldl

SRC = hack.c
OBJ = $(SRC:.c=.o)
TARGET = hack.so

all: $(OBJ)
	$(CC) $(CFLAGS) -shared -fPIC -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

