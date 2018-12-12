# Multithreaded Mandelbrot

This is the repository for a personal project of mine. For more information, please see [the post on my porfolio](https://tomchaplin.github.io/portfolio/Exploring-the-Mandelbrot-Set/).

A C++ class for drawing pictures of and animating zooms into the Mandelbrot set using multiple threads. For this to work you will also need the [C++ Thread Pool Library](https://github.com/vit-vit/CTPL) (ctp_stl.h) and [this simple GIF library](https://github.com/ginsweater/gif-h). When you compile you will need to use the C++11 standard and ensure that you have multithreading enabled. For example, if you are using the <code>g++</code> compiler :

```
g++ -c example.cpp mandelbrot.cpp -std=c++11 -pthread
g++ -o example example.o -std=c++11 -pthread
./example
```

I have also included an example file which demonstrates how you might use the class to create your own zooms into the fractal. For information on what each function of the class does, please consult the Mandelbrot class header file.

There is also a Python file which implements the histogram colouring algorithm in a serial fashion. Obviously, this is much slower but is useful for understanding the main algorithm for computing the Mandelbrot set.
