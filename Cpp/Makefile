INC = -I./Headers/

mandelbrot_example: example.o mandelbrot.o palette.o gif.o colours.o
	g++ -o mandelbrot_example example.o mandelbrot.o palette.o  gif.o colours.o -std=c++11 -pthread

colour_test: colour_tester.o colours.o palette.o gif.o
	g++ -o colour_test colour_tester.o colours.o palette.o gif.o -std=c++11 -pthread

palette.o: palette.cpp Headers/palette.h Headers/colours.h
	g++ $(INC) -c palette.cpp -std=c++11 -pthread

example.o: example.cpp Headers/mandelbrot.h Headers/gif.h Headers/palette.h
	g++ $(INC) -c example.cpp -std=c++11 -pthread

mandelbrot.o: mandelbrot.cpp Headers/mandelbrot.h Headers/ctpl_stl.h Headers/gif.h Headers/colours.h
	g++ $(INC) -c mandelbrot.cpp -std=c++11 -pthread

gif.o: gif.cpp Headers/gif.h
	g++ $(INC) -c gif.cpp -std=c++11 -pthread

colours.o: colours.cpp Headers/colours.h
	g++ $(INC) -c colours.cpp -std=c++11 -pthread

colour_tester.o: colour_tester.cpp Headers/colours.h Headers/palette.h Headers/gif.h
	g++ $(INC) -c colour_tester.cpp -std=c++11 -pthread

clean:
	rm *.o

