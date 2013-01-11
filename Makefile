CXX = c++ -std=c++11
CXXFLAGS = -I../cpplib -I/opt/local/include
LDLIBS = -L/opt/local/lib -lboost_serialization

map: map.o
	$(CXX) -o map map.o $(LDLIBS)

.PHONY: clean
clean:
	rm map map.o
