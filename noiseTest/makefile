
QT_HOME=/opt/qt6
QT_INC=$(QT_HOME)/include
QT_BIN=$(QT_HOME)/libexec


CC=g++
CCFLAGS=-std=c++17 -pedantic -Wall -fPIC
CINCS=-I$(QT_INC) -I$(QT_INC)/QtGui -I$(QT_INC)/QtWidgets -I$(QT_INC)/QtCore -I$(QT_INC)/QtOpenGLWidgets -I$(QT_INC)/QtOpenGL 

LK=g++
LKFLAGS=-Wl,-rpath,/opt/qt6/lib 
LKLIBS= -L$(QT_HOME)/lib -lQt6Gui -lQt6Core -lQt6Widgets -lQt6Network  -lQt6OpenGL -lQt6OpenGLWidgets 

MOC=$(QT_BIN)/moc
UIC=$(QT_BIN)/uic
RCC=$(QT_BIN)/rcc

PROJ=noiseTest

OBJS=main.o noiseTest.o perlinNoise.o simplex2D.o utility.o
MOCS=noiseTest.moc.o

$(PROJ): $(OBJS) $(MOCS)
	$(LK) $(LKFLAGS) $(OBJS) $(MOCS) $(LKLIBS) -o $(PROJ)


%.o : %.cpp %.h
	$(CC) -c $(CCFLAGS) $(CINCS) $< -o $@
	
all : clean $(PROJ)

main.o : main.cpp
	$(CC) -c $(CCFLAGS) $(CINCS) main.cpp -o main.o

%.moc.o : %.h
	$(MOC) $< -o $(*F).moc.cpp
	$(CC) -c $(CCFLAGS) $(CINCS) $(*F).moc.cpp -o $(*F).moc.o
	

clean:
	rm -f *.o
#	rm -f moc_*.*
	rm -f *.moc.cpp
	rm -f *.*~
	rm -rf $(PROJ)

