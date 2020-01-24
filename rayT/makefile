CC= g++
SRC= rayT.cpp
EXE= rayT

OBJECT= $(SRC:.cpp=.o)

all: $(OBJECT)
	$(CC) $(OBJECT) -o $(EXE) -lopengl32 -lglew32 -lfreeglut -lglu32

$(OBJECT):
	$(CC) -c -o $@ $(@:.o=.cpp)

clean:
	rm -f $(OBJECT) $(EXE)
