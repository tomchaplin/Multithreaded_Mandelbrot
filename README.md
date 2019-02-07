# Multithreaded Mandelbrot

This is the repository for a personal project of mine. For more information, please see [the post on my porfolio](https://tomchaplin.github.io/portfolio/Exploring-the-Mandelbrot-Set/).

This is a C++ class for drawing pictures of and animating zooms into the Mandelbrot set using multiple threads. For this to work we are using the [C++ Thread Pool Library](https://github.com/vit-vit/CTPL) (ctp_stl.h) and [this simple GIF library](https://github.com/ginsweater/gif-h). Their (un)licenses apply to their respective software. When you compile you will need to use the C++11 standard and ensure that you have multithreading enabled. For example, if you are using the <code>g++</code> compiler :

```
g++ -c example.cpp mandelbrot.cpp -std=c++11 -pthread
g++ -o example example.o -std=c++11 -pthread
./example
```

I have implemented a number of colouring algorithms which all take in a palette:
* Normal colouring (scaling iteration number to palette)
* Histogram colouring (ensures colours are evenly distributed - better for zooms)
* Loop colouring (one iteration -> one palette colour)
* Smooth histogram colouring
* Smooth loop colouring
* Circular colouring (palette is looped round from negative real axis clockwise, use hue and saturation for colour, luminance depends on iteration count)
* Histogram circular colouring
* Smooth circular colouring

I have include a number of functions that create some palettes for you to experiment with but this is by no means necessary. Palettes should be passed in a `uint8_t` array of RGB colours.

I have also included an example file which demonstrates how you might use the class to create your own zooms into the fractal. To use this just run 

```
mkdir outputs
make mandelbrot_example
./mandelbrot_example
```

and the output gif should turn up in the outpts folder.

For information on what each function of the class does, please consult the Mandelbrot class header file.

There is also a Python file which implements the histogram colouring algorithm in a serial fashion. Obviously, this is much slower but is useful for understanding the main algorithm for computing the Mandelbrot set.
