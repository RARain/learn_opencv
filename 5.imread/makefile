include ../makefile.in

BIN = imread

.PHONY: all

all: $(BIN)

imread: imread.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $<

clean:
	rm -rf *.o $(BIN)