SOURCE=libpqxx-mem-overrun.cpp
OBJECTS=libpqxx-mem-overrun.o

CXX=g++
CXXFLAGS=-std=c++17 -O2
LDFLAGS=

LIBPQXX_INCLUDE=/home/george/soft/libpqxx/include
LIBPQXX_LIBRARY=/home/george/soft/libpqxx/lib
LIBPQXX_LIBS=-lpqxx -lpq

.cpp.o:
	$(CXX) $(CXXFLAGS) -I $(LIBPQXX_INCLUDE) -o $@ -c $<

all: libpqxx-mem-overrun

libpqxx-mem-overrun: $(OBJECTS)
	$(CXX) $(LDFLAGS) -L $(LIBPQXX_LIBRARY) -o $@ $< $(LIBPQXX_LIBS)

clean:
	rm -f *.o
	rm -f libpqxx-mem-overrun

