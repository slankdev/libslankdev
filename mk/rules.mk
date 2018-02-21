
.SUFFIXES: .out .c .cc .o .hS

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.c.o:
	@echo "  [CC]  $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@echo "  [LD]  $@"
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)




