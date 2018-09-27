appname := trains

CC := gcc -g

CXX := gcc -Wall -g
# CXXFLAGS :=
LDFLAGS :=  -fopenmp
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
