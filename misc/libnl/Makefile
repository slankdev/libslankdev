
CFLAGS += -Wall -Werror
CFLAGS += $(shell pkg-config --cflags libnl-3.0)
LDFLAGS += $(shell pkg-config --libs libnl-3.0)

SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = a.out

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

