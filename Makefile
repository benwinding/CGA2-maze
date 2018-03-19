
PLATFORM := $(shell uname)

# Use pkg-config to get the right libraries for your platform
GL_LIBS = `pkg-config --static --libs glfw3` -lGLEW -lGL
EXT = 
DEFS = `pkg-config --cflags glfw3` -std=c++11

# Any other platform specific libraries here...
ifneq (, $(findstring CYGWIN, $(PLATFORM)))
    GL_LIBS = -lopengl32 -lglfw3 -lglew32
	EXT = .exe
    DEFS =-DWIN32
endif

LINK += shader.o engine.o

.PHONY:  clean

all : assign2$(EXT)

assign2$(EXT) : main.o $(LINK)
	g++ $(DEFS) -o assign2 main.o $(LINK) $(GL_LIBS)

main.o : main.cpp $(LINK)
	g++ $(DEFS) -c main.cpp

engine.o : engine.cpp engine.hpp
	g++ $(DEFS) -c engine.cpp

shader.o : shader.cpp shader.hpp
	g++ $(DEFS) -c shader.cpp

clean:
	rm -f *.o assign2$(EXT)
