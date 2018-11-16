include ../makefile.in

BIN = rectangle

.PHONY: all

all: $(BIN)

rectangle: rectangle.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $<

clean:
	rm -rf *.o $(BIN)