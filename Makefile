PLATFORM := $(shell uname)
$(info Platform="$(PLATFORM)")

GL_LIBS = `pkg-config --static --libs glfw3 glew` 
EXT = 
CPPFLAGS = `pkg-config --cflags glfw3`


# Any other platform specific libraries here...
ifneq (, $(findstring MINGW, $(PLATFORM)))
    GL_LIBS = `pkg-config --static --libs glfw3 glew freeglut`
	EXT = .exe
endif


CC = g++
EXE = assign2
OBJS = main.o Maze.o Shader.o Viewer.o

.PHONY: all clean

# If you haven't seen this before, notice how the rules
# of the Makefile build .o files from c++ source, and then
# build the executable from the .o files. Files are only
# re-made if they've been modified, or something they depend
# on has been modified.

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(GL_LIBS)

main.o: main.cpp InputState.h 	
	$(CC) $(CPPFLAGS) -c main.cpp

Shader.o : Shader.cpp Shader.hpp
	$(CC) $(CPPFLAGS) -c Shader.cpp

Viewer.o: Viewer.h Viewer.cpp InputState.h
	$(CC) $(CPPFLAGS) -c Viewer.cpp

Maze.o: Maze.h Maze.cpp	
	$(CC) $(CPPFLAGS) -c Maze.cpp

clean:
	rm -f *.o $(EXE)$(EXT)
