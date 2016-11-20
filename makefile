CC=g++
CXXFLAGS = `pkg-config --cflags --libs opencv`
CPPFLAGS = -L /home/xul4/opencv/lib\
	   -I /home/xul4/opencv/include

OBJECTS = ProjectHelper.o

todo:  $(OBJECTS)
	$(CC) $(CPPFLAGS) src.cpp  $(OBJECTS) -o exe $(CXXFLAGS)

ProjectHelper.o:
	$(CC) -c ProjectHelper.cpp

clean:
	rm -rf exe $(OBJECTS)
