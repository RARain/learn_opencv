include ../makefile.in

BIN = roi

.PHONY: all

all: $(BIN)

roi: roi.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $<

clean:
	rm -rf *.o $(BIN)