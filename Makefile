CXX = g++-mp-4.7 -std=c++11
CXXFLAGS = -I../cpplib -I/opt/local/includes
LDLIBS = -L/opt/local/lib -lboost_serialization

map: map.o
	$(CXX) -o map map.o $(LDLIBS)
