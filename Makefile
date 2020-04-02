PROGRAM = tpp
ARGUMENTS =

CC = g++ # use C++ compiler
CPPFLAGS = -pedantic -Wall -Wextra -std=c++17 -ggdb # use C++ standard

LD = g++ # use C++ linker
LDFLAGS = -lm

CFILES = $(wildcard *.cpp) # collect cpp files
OFILES = $(CFILES:.cpp=.o) # rename

all: clean depend $(PROGRAM)

$(PROGRAM): $(OFILES)
	$(LD) -o $@ $(OFILES) $(LDFLAGS)

.PHONY: all clean

clean:
	rm -f $(OFILES) $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM) $(ARGUMENTS)

debug: $(PROGRAM)
	gdb $(PROGRAM) $(ARGUMENTS)

DEPENDFILE = .depend

depend:
	$(CC) $(CCFLAGS) -MM $(CFILES) > $(DEPENDFILE)

-include $(DEPENDFILE)
