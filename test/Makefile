
include ../common.mk
export INCLUDE_DIR := ../include
export LIB_DIR     := ../src

.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)


SRC = main.cc
OBJ = $(SRC:.cc=.o)
TARGET = a.out

test: $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(TARGET)  -L$(LIB_DIR) -I$(INCLUDE_DIR) -lslankdev

clean: 
	$(RM) $(TARGET)
	$(RM) $(OBJ)
