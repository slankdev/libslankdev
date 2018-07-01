
bin_PROGRAMS = sample
sample_SOURCES = main.c hello.c

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(TARGET)
