CC=g++
CXXFLAGS = `pkg-config --cflags --libs opencv`
OBJECTS = ProjectHelper.o

todo:  $(OBJECTS)
	$(CC) src.cpp  $(OBJECTS) -o exe $(CXXFLAGS)

ProjectHelper.o:
	$(CC) -c ProjectHelper.cpp

clean:
	rm -rf exe $(OBJECTS)
