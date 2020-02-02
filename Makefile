GCC = g++
CFLAGS = 
LDFLAGS = 

ODIR = obj
OUT = main

all: $(ODIR) $(OUT)

run: all
		./$(OUT)
$(ODIR):
		mkdir $(ODIR)

C_SOURCES = $(shell find . -type f -name "*.cpp" -printf "%f\n")
OBJECTS = $(patsubst %.cpp, $(ODIR)/%.o,$(C_SOURCES))

$(ODIR)/%.o : %.cpp
		$(GCC) $(CFLAGS) -c $^ -o $@

$(OUT): $(OBJECTS)
		$(GCC) $(LDFLAGS) -o $(OUT) $(OBJECTS)

clean: $(OUT)
	rm -rf $(ODIR)
