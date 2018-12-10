#include <cstddef>
#include <iostream>
#include <cstdlib>

#include "mandelbrot.cpp"
#include "gif.h"
#include "palette.h"

int main(void) {
    // Mandelbrot iteraions paramaters
    const int max_iterations = 2000;
    const double escape_radius = 2.0 * 1000000.0;
    // GIF creation parameters
    const int delay = 10;
    const char *filename = "outputs/histo_smooth_colouring26.gif";
    const int gifWidth = 1920;
    const int gifHeight = 1080;
    GifWriter writer;
    GifBegin(&writer, filename,gifWidth,gifHeight,delay);
    // Multithreading parameters
    const int num_threads = 8;
    // Path taken
    const int timeSteps = 240;
    const double zoomFactor = 1.06;
    const double xShift = 0.0;
    const double yShift = 0.0;
    double currentX = -0.13753002464788744;
    double currentY = 1.0148482188967138;
    double currentWidth = 1.0;
    // Fetch our palette
    uint8_t palette[135*3];
    getPaletteOne(palette);
    int palette_size = 135;
    // Create mandelbrot and set colour mode
    Mandelbrot *mandel = new Mandelbrot(gifWidth,gifHeight,currentWidth,currentX,currentY);
    mandel->setColourMode(Mandelbrot::SMOOTH_HISTOGRAM_COLOURING);

    // Main loop
    for(int i = 0; i < timeSteps; i++) {
        mandel->setCentre(currentX, currentY);
        mandel->setWidth(currentWidth);
        mandel->iterate(max_iterations,escape_radius,num_threads);
        mandel->colourFrame(palette,palette_size);
        mandel->writeFrame(&writer,delay);
        if(i== 0) {
            // Stop at the start for a second
            for(int j=0; j<10; j++) {
                mandel->writeFrame(&writer,delay);
            }
        }
        std::cout << timeSteps - i - 1 << " frames left" << std::endl;
        currentX += xShift;
        currentY += yShift;
        currentWidth = currentWidth / zoomFactor;
    }

    // Stop at the end for a second
    for(int i = 0; i < 10; i++) {
        mandel->writeFrame(&writer,delay);
    }

    // Relase the GIF
    GifEnd(&writer);
}
