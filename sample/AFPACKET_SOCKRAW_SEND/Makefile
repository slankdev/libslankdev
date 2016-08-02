

CPP = g++
RM  = rm -rf
SRC = main.cc
TARGET = sendarp

all: $(TARGET)


$(TARGET): $(SRC)
	$(CPP) $(SRC) -o $@ -lslankdev

clean:
	$(RM) $(TARGET) *.o


