include ../makefile.in

BIN = blur

.PHONY: all

all: $(BIN)

blur: blur.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $<

clean:
	rm -rf *.o $(BIN)