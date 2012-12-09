CXX = c++
LDFLAGS = -lboost_serialization

mymap: mymap.o
	$(CXX) -o mymap mymap.o $(LDFLAGS)

mymap.o: mymap.cpp
	$(CXX) -c -I../cpplib mymap.cpp
