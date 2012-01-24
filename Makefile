CPP  = i686-pc-mingw32-g++.exe
CC   = gcc.exe

RES  =
OBJ  = Affiche3d.o Camera.o Ground.o InitGLmodes.o InitPlayfield.o ZEMain.o QuadTree.o QuadTreeFrustum.o $(RES)
LINKOBJ  = Affiche3d.o Camera.o Ground.o InitGLmodes.o InitPlayfield.o ZEMain.o QuadTree.o QuadTreeFrustum.o $(RES)
LIBS =  -L"lib" -L"." glaux.lib -lagl -lalleg -luser32 -lgdi32 -lopengl32 -lglu32 -lglu32 -glaux32 -lopengl32 -mwindows
INCS =
CXXINCS =  -I"C:\cygwin\usr\i686-pc-mingw32\sys-root\mingw\include" -I"include/allegro442_include" -I"include/allegrogl_include"

BIN  = trip-to-mars.exe
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before trip-to-mars.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "trip-to-mars.exe" $(LIBS)

Affiche3d.o: Affiche3d.cpp
	$(CPP) -c Affiche3d.cpp -o Affiche3d.o $(CXXFLAGS)

Camera.o: Camera.cpp
	$(CPP) -c Camera.cpp -o Camera.o $(CXXFLAGS)

Ground.o: Ground.cpp
	$(CPP) -c Ground.cpp -o Ground.o $(CXXFLAGS)

InitGLmodes.o: InitGLmodes.cpp
	$(CPP) -c InitGLmodes.cpp -o InitGLmodes.o $(CXXFLAGS)

InitPlayfield.o: InitPlayfield.cpp
	$(CPP) -c InitPlayfield.cpp -o InitPlayfield.o $(CXXFLAGS)

ZEMain.o: ZEMain.cpp
	$(CPP) -c ZEMain.cpp -o ZEMain.o $(CXXFLAGS)

QuadTree.o: QuadTree.cpp
	$(CPP) -c QuadTree.cpp -o QuadTree.o $(CXXFLAGS)

QuadTreeFrustum.o: QuadTreeFrustum.cpp
	$(CPP) -c QuadTreeFrustum.cpp -o QuadTreeFrustum.o $(CXXFLAGS)
