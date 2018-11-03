# Multithreaded Mandelbrot

A C++ class for drawing pictures of and animating zooms into the Mandelbrot set using multiple threads. For this to work you will also need the [C++ Thread Pool Library](https://github.com/vit-vit/CTPL) (ctp_stl.h) and [this simple GIF library](https://github.com/ginsweater/gif-h). When you compile you will need to use the C++11 standard and ensure that you have multithreading enabled. For example, if you are using the <code>g++</code> compiler :

```
g++ -o mandelbrot mandelbrot.cpp -std=c++11 -pthread
```

I have also included an example file which demonstrates how you might use the class to create your own zooms into the fractal.

There is also a Python file which does the same job but in serial and significantly more slowly.