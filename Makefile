EXE = cosmic
SOURCES = envmain.cpp cosproc.cpp
SOURCES += lib/imgui/imgui_impl_sdl.cpp lib/imgui/imgui_impl_opengl3.cpp
SOURCES += lib/imgui/imgui.cpp lib/imgui/imgui_demo.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_widgets.cpp

VPATH = src:bin

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
BINS = $(addprefix bin/, $(OBJS))
UNAME_S := $(shell uname -s)

CXXFLAGS = -Ilib/imgui
CXXFLAGS += -g -Wall -Wformat -Wno-unknown-pragmas
LIBS =


## Using OpenGL loader: gl3w [default]
SOURCES += lib/gl3w/GL/gl3w.c
CXXFLAGS += -Ilib/gl3w

ifeq ($(UNAME_S), Linux) #LINUX
	echo "Starting Build for Linux"
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += -Ilibs/gl3w `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	echo "Starting Build for macOS"
	ECHO_MESSAGE = "macOS"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += -Ilibs/gl3w `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
	echo "Starting Build for Windows (MinGW)"
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`
	CXXFLAGS += -Ilibs/gl3w `pkg-config --cflags sdl2` -lmingw32 -lSDL2main -lSDL2 -mwindows
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------


%.o:%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/$@ $<

%.o:%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/$@ $<

%.o:lib/imgui/%.cpp
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
	rm -f $(EXE) $(OBJS)
