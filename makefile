todo: all
all: src.cpp
	g++ src.cpp -o exe `pkg-config --cflags --libs opencv`

clean:
	rm -rf exe tags
	#rm -f source/*
	#rm -f target/*
