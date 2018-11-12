include ../makefile.in

BIN = segmentation

.PHONY: all

all: $(BIN)

segmentation: segmentation.o
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $<

clean:
	rm -rf *.o $(BIN)