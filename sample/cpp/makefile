CC=g++
CXXFLAGS = `pkg-config --cflags --libs opencv`
CPPFLAGS = -L/home/xul4/opencv/lib\
	   -I/home/xul4/opencv/include


todo:  
	$(CC) $(CPPFLAGS) houghlines.cpp  -o exe $(CXXFLAGS)


clean:
	rm -rf exe 
