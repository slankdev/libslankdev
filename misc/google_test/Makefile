SRC = tmp.c
OBJ = $(SRC:.c=.o)
TARGET = libtmp.a

all: $(OBJ)
	ar rcs $(TARGET) $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)

test:
	g++ test1.cc \
		-I. -L. -ltmp -lgtest -lgtest_main -lpthread

mytest:
	g++ main.cc tmp.o -I. -L.

tmd:
		# -I$(GTEST_INCLUDEDIR) -L$(GTEST_LIBDIR) \
