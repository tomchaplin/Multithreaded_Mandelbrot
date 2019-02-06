#include <cstddef>
#include <iostream>
#include <cstdlib>

#include "mandelbrot.h"
#include "gif.h"
#include "palette.h"

int main(void) {
    // Mandelbrot iteraions paramaters
    const int max_iterations = 100;
    const double escape_radius = 2.0 * 1000000.0;
    // GIF creation parameters
    const int delay = 10;
    const char *filename = "outputs/circular_test18";
    const int gifWidth = 2560*2;
    const int gifHeight = 1440*2;
    GifWriter writer;
    GifBegin(&writer, filename,gifWidth,gifHeight,delay);
    // Multithreading parameters
    const int num_threads = 4;
    // Path taken
    const int timeSteps = 1;
    const double zoomFactor = 1.06;
    const double xShift = 0.0;
    const double yShift = 0.0;
    double currentX = -0.75;
    double currentY = 0.0;
    double currentWidth = 3.75;
    // Fetch our palette
	// One is nicest
    uint8_t palette[360*3];
    getPaletteSix(palette);
    int palette_size = 360;
    // Create mandelbrot and set colour mode
    Mandelbrot *mandel = new Mandelbrot(gifWidth,gifHeight,currentWidth,currentX,currentY);
    mandel->setColourMode(Mandelbrot::SMOOTH_CIRCULAR_COLOURING);

    // Main loop
    for(int i = 0; i < timeSteps; i++) {
        // Reset center and width of image
        mandel->setCentre(currentX, currentY);
        mandel->setWidth(currentWidth);
        // Compute escape times for each pixel
        mandel->iterate(max_iterations,escape_radius,num_threads);
        // Colour image in according to specified algorithm
        mandel->colourFrame(palette,palette_size);
        // Write image to file
        mandel->writeFrame(&writer,delay);
        // Stop at the start for a second
        if(i== 0) {
            for(int j=0; j<10; j++) {
                mandel->writeFrame(&writer,delay);
            }
        }
        std::cout << timeSteps - i - 1 << " frames left" << std::endl;
        // Adjust the point to follow straight line path
        currentX += xShift;
        currentY += yShift;
        // Adjust width of image to zoom in
        currentWidth = currentWidth / zoomFactor;
    }

    // Stop at the end for a second
    for(int i = 0; i < 10; i++) {
        mandel->writeFrame(&writer,delay);
    }

    // Relase the GIF
    GifEnd(&writer);
}
