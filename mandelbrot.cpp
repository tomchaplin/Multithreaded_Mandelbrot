#include <cstddef>
#include <iostream>
#include <cstdlib>

#include "ctpl_stl.h"
#include "gif.h"

class Mandelbrot{
public:
    const static int NORMAL_COLOURING = 0;
    const static int HISTOGRAM_COLOURING = 1;
    const static int LOG_COLOURING = 2;
    const static int LOOP_COLOURING = 3;
    int pixelWidth, pixelHeight;
    double cWidth, cHeight;
    double centerX, centerY;

    Mandelbrot(int pw = 1280, int ph =720, double cw = 1.0, double x = 0.0, double y = 0.0) {
        pixelWidth = pw;
        pixelHeight = ph;
        cWidth = cw;
        cHeight = (double) ph / (double) pw * cw;
        centerX = x;
        centerY = y;
        startX = centerX - cWidth / 2.0;
        startY = centerY + cHeight / 2.0;
        step = cWidth / (double) pixelWidth;
        iterationSpace = new int[pixelWidth * pixelHeight];
    }

    void iterate(int maxIter = 1000, double escapeRadius = 2.0, int NUM_THREADS = 1) {
        // Might change this to work with OpenMP using a parallel for loop
        lastFrameMaxIter = maxIter;
        ctpl::thread_pool p(NUM_THREADS);
        double x0 = startX, y0 = startY;
        for(int i = 0; i < pixelHeight; i++) {
            y0 -= step;
            x0 = startX;
            for(int j = 0; j< pixelWidth; j++) {
                x0 += step;
                p.push(Mandelbrot::iteratePixel, iterationSpace, i * pixelWidth + j, x0, y0, maxIter, escapeRadius);
            }
        }
        p.stop(true);
    }

    void print(int maxIter = 1000) {
        for(int i= 0; i< pixelHeight; i++) {
            for(int j = 0; j< pixelWidth; j++) {
                if( iterationSpace[i*pixelWidth + j] == maxIter ) {
                    std::cout << "*";
                } else {
                    std::cout << " ";
                }
            }
            std::cout<<std::endl;
        }
    }

    void setCentre(double cx, double cy) {
        centerX = cx;
        centerY = cy;
        startX = centerX - cWidth / 2.0;
        startY = centerY + cHeight / 2.0;
    }

    void setWidth(double cw) {
        cWidth = cw;
        cHeight = (double) pixelHeight / (double) pixelWidth * cw;
        startX = centerX - cWidth / 2.0;
        startY = centerY + cHeight / 2.0;
        step = cWidth / (double) pixelWidth;
    }

    void colourFrame(uint8_t *rgb_palette, int palette_size, int method = 0) {
        // Initialise image with black background
        image = new uint8_t[pixelWidth * pixelHeight * 4 + 3];
        image[pixelWidth*pixelHeight*4] = 255;
        image[pixelWidth*pixelHeight*4 + 1] = 255;
        image[pixelWidth*pixelHeight*4 + 2] = 255;
        // Colour in the image
        switch(method) {
            case HISTOGRAM_COLOURING:
            {
                // Histogram colouring
                int total = pixelWidth * pixelHeight;
                // Build the historgram
                int histogram[lastFrameMaxIter + 1];
                for(int i = 0; i<= lastFrameMaxIter; ++i) {
                    histogram[i] = 0;
                }
                for(int i = 0; i < total; i++) {
                    histogram[iterationSpace[i]]++;
                }
                // Figure out the colour for each number of iterations
                int colour[lastFrameMaxIter + 1];
                colour[0] = 0;
                double previousProp = (double) histogram[0] / (double) total;
                for(int i = 1; i <= lastFrameMaxIter; ++i) {
                    colour[i] = (int) (previousProp * ( (double) (palette_size - 1) )   );
                    previousProp = previousProp + (double) histogram[i] / (double) total;
                }
                // Go through each pixel and colour it in
                for(int i = 0; i < total; ++i) {
                    // Red, green, blue channels
                    image[i*4] = rgb_palette[colour[iterationSpace[i]]*3];
                    image[i*4 + 1] = rgb_palette[colour[iterationSpace[i]]*3 + 1];
                    image[i*4 + 2] = rgb_palette[colour[iterationSpace[i]]*3 + 2];
                    // Full Alpha
                    image[i*4 + 3] = 255;
                }
                break;
            }
            case LOG_COLOURING:
            {
                // Logarithm colouring
                break;
            }
            case LOOP_COLOURING:
            {
                // We do standard colouring but just keep looping round the palette rather than interpolating
                break;
            }
            default:
            {
                // Standard coloruing
                int colour_start;
                for(int i = 0; i < pixelWidth * pixelHeight; ++i) {
                    colour_start = (int) (iterationSpace[i] * (palette_size - 1) / lastFrameMaxIter);
                    // Red, green, blue channels
                    image[i*4] = rgb_palette[colour_start*3];
                    image[i*4 + 1] = rgb_palette[colour_start*3 + 1];
                    image[i*4 + 2] = rgb_palette[colour_start*3 + 2];
                    // Full alpha
                    image[i*4 + 3] = 255;
                }
            }
        }
    }

    void writeFrame(GifWriter *writer, uint32_t delay = 0) {
        GifWriteFrame(writer, image, pixelWidth, pixelHeight, delay, 8 , false);
    }

private:
    int *iterationSpace;
    uint8_t *image;
    double startX, startY;
    double step;
    int lastFrameMaxIter;

    static void iteratePixel(int threadID, int *iterSpace, int pos, double x0, double y0, int maxIter, double escapeRadius) {
        /*
        // Could add this check if your image has lots of the cardioid in it
        // Otherwise it might slow down your iterations
        if(inCardioidOrBulb(x0,y0)) {
            iterSpace[pos] = maxIter;
            return;
        }
        */
        double x = 0.0, y = 0.0;
        int iterations = 0;
        double escapeSqr = escapeRadius * escapeRadius;
        double xTemp;
        while(x*x + y*y < escapeSqr && iterations < maxIter) {
            xTemp = x*x - y*y + x0;
            y = 2.0*x*y + y0;
            x = xTemp;
            iterations++;
        }
        // We guess the point belongs to the mandelbrot set
        // Really this should be maxIter but because we are interested in the boundary of the Mandelbrot set
        // We use the first colour in the palette to create contrast at the boundary
        if(iterations == maxIter) {
            iterations = 0;
        }
        iterSpace[pos] = iterations;

    }

    static bool inCardioidOrBulb(double x,double y) {
        double q = (x-0.25)*(x-0.25) + y*y;
        if(   q * (q + x - 0.25) < 0.25 * y * y ) {
            // (x,y) is in the cardioid
            return true;
        } else if (  16.0*(x+1)*(x+1) + 16.0*y*y < 1.0  ) {
            // (x,y) is in the bulb
            return true;
        } else {
            return false;
        }
    }

};

double mod1(double x) {
    while (x > 1.0) {
        x--;
    }
    while (x < 0.0) {
        x++;
    }
    return x;
}

double hsl_tests(double x, double tmp_1, double tmp_2) {
    if(6.0*x < 1.0) {
        return tmp_2 + (tmp_1 - tmp_2) * 6.0 * x;
    } else if(2.0*x < 1.0) {
        return tmp_1;
    } else if(3.0*x < 2.0) {
        return tmp_2 + (tmp_1 - tmp_2) * (2.0/3.0 - x) * 6.0;
    } else {
        return tmp_2;
    }
}

void HSL_2_RGB(double *hsl, uint8_t *rgb) {
    double hue = hsl[0];
    double saturation = hsl[1];
    double luminance = hsl[2];

    if(saturation == 0) {
        // The colour is grey
        rgb[0] = (uint8_t) luminance * 255.0;
        rgb[1] = (uint8_t) luminance * 255.0;
        rgb[2] = (uint8_t) luminance * 255.0;
    } else {
        // We need to calculate some temporary variables
        double tmp_1, tmp_2;
        if(luminance < 0.5) {
            tmp_1 = luminance * (1.0 + saturation);
        } else {
            tmp_1 = luminance + saturation - luminance*saturation;
        }
        tmp_2 = 2.0*luminance - tmp_1;
        hue = hue/360.0;
        rgb[0] = (uint8_t) ( hsl_tests ( mod1(hue + (1.0/3.0)), tmp_1, tmp_2 ) * 255 );
        rgb[1] = (uint8_t) ( hsl_tests ( mod1(hue), tmp_1, tmp_2 ) * 255 );
        rgb[2] = (uint8_t) ( hsl_tests ( mod1(hue - (1.0/3.0)), tmp_1, tmp_2 ) * 255 );
    }
}

int main(void) {
    // Mandelbrot iteraions paramaters
    const int max_iterations = 7500;
    const double escape_radius = 2.0;
    // GIF creation parameters
    const int delay = 10;
    // Multithreading parameters
    const int num_threads = 4;
    // Path taken
    const int timeSteps = 500;
    const double zoomFactor = 1.04;
    const double xShift = 0.0;
    const double yShift = 0.0;
    double currentX = -1.631338551976979;
    double currentY = 0.0000013531132100806823;
    double currentWidth = 1.0;

    /*
    test_zoom04 -0.743643,0.1318265, 1.118,100

    */

    Mandelbrot *mandel = new Mandelbrot(1920,1080,currentWidth,currentX,1.0);

    int palette_size = 300;

    uint8_t palette[palette_size*3];
    uint8_t rgb[3];
    double hsl[3];
    hsl[0] = 235.0;
    hsl[1] = 1.0;
    for(int i=0; i < palette_size; ++i) {
        hsl[2] = ( (double) i ) / 300.0;
        HSL_2_RGB(hsl, rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
        //std::cout << (int) rgb[0] << "," << (int) rgb[1] << "," << (int) rgb[2] << " | " << std::flush;
    }

    GifWriter writer;
    const char *filename = "outputs/hsl_palette_big_zoom02.gif";
    GifBegin(&writer, filename,1920,1080,delay);

    for(int i = 0; i < timeSteps; i++) {
        mandel->setCentre(currentX, currentY);
        mandel->setWidth(currentWidth);
        mandel->iterate(max_iterations,escape_radius,num_threads);
        mandel->colourFrame(palette,palette_size,Mandelbrot::HISTOGRAM_COLOURING);
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

    GifEnd(&writer);
}
