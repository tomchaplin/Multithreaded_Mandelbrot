# Multithreaded Mandelbrot

A C++ class for drawing pictures of and animating zooms into the Mandelbrot set using multiple threads. For this to work you will also need the [C++ Thread Pool Library](https://github.com/vit-vit/CTPL) (ctp_stl.h) and [this simple GIF library](https://github.com/ginsweater/gif-h). When you compile you will also need the C++11 standard and make sure you have multithreading enabled. For example, if you are using the <code>g++</code> compiler :

```
g++ -o mandelbrot mandelbrot.cpp -std=c++11 -pthread
```

I have also included a Python file which does the same job but isn't quite so robust.
