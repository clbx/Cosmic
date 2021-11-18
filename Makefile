EXE = cosmic
SOURCES = main.cpp cosproc.cpp pgu.cpp runGUI.cpp runCLI.cpp
SOURCES += lib/gl3w/GL/glew.c
SOURCES += lib/misc/imgui_impl_sdl.cpp lib/misc/imgui_impl_opengl3.cpp lib/misc/imgui_impl_opengl2.cpp
SOURCES += lib/imgui/imgui.cpp lib/imgui/imgui_demo.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_widgets.cpp
SOURCES += lib/misc/imguifilesystem.cpp
#SOURCES += lib/imtui/imtui-impl-text.cpp lib/timtui/imtui-impl-ncurses.h

VPATH = src:bin

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
BINS = $(addprefix bin/, $(OBJS))
UNAME_S := $(shell uname -s)
ARCH := $(shell gcc -dumpmachine)

CXXFLAGS = -Ilib/imgui -Ilib/misc -Ilib/imtui
CXXFLAGS += -g -Wformat -Wno-unknown-pragmas #-lncurses -fno-omit-frame-pointer -fsanitize=thread
LIBS =


## Using OpenGL loader: gl3w [default]
# SOURCES += lib/gl3w/GL/gl3w.c
CXXFLAGS += -Ilib/gl3w
ifeq ($(PLAT), Pi)
	CXXFLAGS += -lwiringPi
endif

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += -Ilibs/gl3w `sdl2-config --cflags` -Wall -std=c++11
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "macOS"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += -Ilibs/gl3w `sdl2-config --cflags` -Wall
	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include/
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(ARCH),x86_64-w64-mingw32)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32
	CXXFLAGS += -Ilibs/gl3w -I/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -static-libgcc -static-libstdc++ -lpthread
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------


%.o:%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/$@ $<

%.o:lib/imgui/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/$@ $<

%.o:lib/misc/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/$@ $<

%.o:lib/imtui/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/$@ $<

%.o:lib/gl3w/GL/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c -o bin/$@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $(BINS) $(CXXFLAGS) $(LIBS)
	rm -f imgui.ini

clean:
	rm -rf bin 
	rm cosmic
