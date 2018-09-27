appname := sim

CFLAGS := -g -pthread

CXX := gcc
CXXFLAGS := -Wall -g
LDFLAGS :=  -fopenmp -pthread
# LDLIBS :=


srcfiles := $(shell find . -name "*.c")
objects  := $(patsubst %.c, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(LDFLAGS) -o $(appname) $(objects)

clean:
	rm -f $(objects)
	rm -f $(appname)

dist-clean: clean
	rm -f *~ .depend
