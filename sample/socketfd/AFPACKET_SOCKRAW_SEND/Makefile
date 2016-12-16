

CPP = g++ -std=c++11
RM  = rm -rf
SRC = main.cc
TARGET = sendarp

all: $(TARGET)


$(TARGET): $(SRC)
	$(CPP) $(SRC) -o $@ -lslankdev

clean:
	$(RM) $(TARGET) *.o


