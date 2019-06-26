CXX           = g++
CXXFLAGS      = -g -O2 -I../../../src/ -I/opt/local/include
LD            = $(CXX)
LDFLAGS       = 

all: randomness cf_boost

randomness.o: randomness.cc 
	$(CXX) $(CXXFLAGS) randomness.cc -c -o $@

randomness: randomness.o
	$(LD) -o $@ $(LDFLAGS) randomness.o

cf_boost.o: cf_boost.cc
	$(CXX) $(CXXFLAGS) cf_boost.cc -c -o $@ 

cf_boost: cf_boost.o
	$(LD) -o $@ $(LDFLAGS) cf_boost.o

clean:
	@-rm -rf randomness cf_boost *.o
