EXE = cosmic
SOURCES = envmain.cpp cosproc.cpp
SOURCES += ../lib/imgui/imgui_impl_sdl.cpp ../lib/imgui/imgui_impl_opengl3.cpp
SOURCES += ../lib/imgui/imgui.cpp ../lib/imgui/imgui_demo.cpp ../lib/imgui/imgui_draw.cpp ../lib/imgui/imgui_widgets.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I../ -I../../ -I../lib/imgui
CXXFLAGS += -g -Wall -Wformat -Wno-unknown-pragmas
LIBS =

## Using OpenGL loader: gl3w [default]
SOURCES += ../lib/gl3w/GL/gl3w.c
CXXFLAGS += -I../lib/gl3w

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += -I../libs/gl3w `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "macOS"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += -I../libs/gl3w `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
   ECHO_MESSAGE = "MinGW"
   LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

   CXXFLAGS += -I../libs/gl3w `pkg-config --cflags sdl2`
   CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../../%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../lib/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../lib/gl3w/GL/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	rm -f imgui.ini


simple:
	g++ -o cosmic cosproc.cpp term.cpp

clean:
	rm -f $(EXE) $(OBJS)