cc=g++
VPATH = ../../src
FONTS= main.cpp
INCLUDE = ../../include
LIBFLAGS = -lcurl -ljsoncpp
CTAGS = -Wall -pedantic -O2 
OUT = moviesearch
LIBDIR = ../lib


all:$(OUT)

$(OUT): $(FONTS:.cpp=.o)
	$(cc) -o $@ $^ $(LIBFLAGS)

%.o: %.cpp 
	$(cc) -c $(CTAGS) -I$(INCLUDE) -I$(LIBDIR) $< -o $@

clean:
	rm -f $(OUT) *.o