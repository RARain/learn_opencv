include ../makefile.in

BIN = load_1 load_2

.PHONY: all

all: $(BIN)

load_1: load_1.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

load_2: load_2.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $<

clean:
	rm -rf *.o $(BIN)