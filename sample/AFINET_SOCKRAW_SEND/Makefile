

CPP = g++
RM  = rm -rf
SRC = main.cc
TARGET = app.out

all: $(TARGET)


$(TARGET): $(SRC)
	$(CPP) -g $(SRC) -o $@ -lslankdev

clean:
	$(RM) $(TARGET) *.o


run: 
	sudo ./$(TARGET) wlxcce1d51517b5 10.210.125.33
