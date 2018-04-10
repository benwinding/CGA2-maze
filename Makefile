PLATFORM := $(shell uname)
$(info Platform="$(PLATFORM)")

# LINUX
# sudo apt-get install libglfw3-dev libglew-dev libglm-dev
GL_LIBS = `pkg-config --static --libs glfw3 glew` 
EXT = 
CPPFLAGS = `pkg-config --cflags glfw3` -Iinclude

# Any other platform specific libraries here...
ifneq (, $(findstring MINGW, $(PLATFORM)))
    GL_LIBS = `pkg-config --static --libs glfw3 glew freeglut`
    EXT = .exe
endif

CC = g++
EXE = assign2
OUT_DIR = out/
SRC_DIR = src/

CPP_FILES = $(wildcard $(SRC_DIR)*.cpp)
O_FILES = $(addprefix $(OUT_DIR), $(notdir $(CPP_FILES:%.cpp=%.o)))
D_FILES = $(addprefix $(OUT_DIR), $(notdir $(CPP_FILES:%.cpp=%.d)))

all: $(EXE)

$(OUT_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p out
	$(CC) $(CPPFLAGS) -c -MMD -o $@ $<
	
-include $(D_FILES)

$(EXE): $(O_FILES)
	$(CC) -o $(EXE) $(O_FILES) $(GL_LIBS)

clean:
	rm -f $(O_FILES) $(D_FILES) $(EXE)$(EXT)

