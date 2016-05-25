
include ../common.mk
INCLUDE_DIR = ../include

.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)

SRC = utils.cc system.cc intfd.cc
OBJ = $(SRC:.cc=.o)


all: libslankdev.a

libslankdev.a: $(OBJ)
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

clean:
	$(RM) libslankdev.a
	$(RM) $(OBJ)


