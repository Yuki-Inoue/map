CXX = c++ -std=c++11
LDFLAGS = -lboost_serialization

map: map.o
	$(CXX) -o map map.o $(LDFLAGS)

map.o: map.cpp
	$(CXX) -c -I../cpplib map.cpp
