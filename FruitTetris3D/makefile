CC= g++
SRC= FruitTetris3D.cpp InitShader.cpp
EXE= FruitTetris3D

OBJECT= $(SRC:.cpp=.o)

all: $(OBJECT)
	$(CC) $(OBJECT) -o $(EXE) -lGL -lglut -lGLEW -lXext -lX11 -lm

$(OBJECT):
	$(CC) -c -o $@ $(@:.o=.cpp)

clean:
	rm -f $(OBJECT) $(EXE)
