#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "mandelbrot.h"
#include "ctpl_stl.h"
#include "gif.h"
#include "colours.h"

const int Mandelbrot::NORMAL_COLOURING = 0;
const int Mandelbrot::HISTOGRAM_COLOURING = 1;
const int Mandelbrot::LOOP_COLOURING = 2;
const int Mandelbrot::SMOOTH_HISTOGRAM_COLOURING = 3;
const int Mandelbrot::SMOOTH_LOOP_COLOURING = 4;
const int Mandelbrot::CIRCULAR_COLOURING = 5;
const int Mandelbrot::HISTOGRAM_CIRCULAR_COLOURING = 6;
const int Mandelbrot::SMOOTH_CIRCULAR_COLOURING = 7;

Mandelbrot::Mandelbrot(int pw = 1280, int ph =720, double cw = 1.0, double x = 0.0, double y = 0.0) {
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
    colour_mode = NORMAL_COLOURING;
}

void Mandelbrot::iterate(int maxIter = 1000, double escapeRadius = 2.0, int NUM_THREADS = 1) {
    // Might change this to work with OpenMP using a parallel for loop
    lastFrameMaxIter = maxIter;
    ctpl::thread_pool p(NUM_THREADS);
    double x0 = startX, y0 = startY;
    for(int i = 0; i < pixelHeight; i++) {
        y0 -= step;
        x0 = startX;
        for(int j = 0; j< pixelWidth; j++) {
            x0 += step;
            if(colour_mode == SMOOTH_LOOP_COLOURING or colour_mode == SMOOTH_HISTOGRAM_COLOURING or colour_mode == SMOOTH_CIRCULAR_COLOURING) {
                p.push(Mandelbrot::logIteratePixel, iterationSpace, fracSpace, i * pixelWidth + j, x0, y0, maxIter, escapeRadius);
            } else {
                p.push(Mandelbrot::iteratePixel, iterationSpace, i * pixelWidth + j, x0, y0, maxIter, escapeRadius);
            }
        }
    }
    p.stop(true);
}

void Mandelbrot::print() {
    for(int i= 0; i< pixelHeight; i++) {
        for(int j = 0; j< pixelWidth; j++) {
            if( iterationSpace[i*pixelWidth + j] == lastFrameMaxIter ) {
                std::cout << "*";
            } else {
                std::cout << " ";
            }
        }
        std::cout<<std::endl;
    }
}

void Mandelbrot::setCentre(double cx, double cy) {
    centerX = cx;
    centerY = cy;
    startX = centerX - cWidth / 2.0;
    startY = centerY + cHeight / 2.0;
}

void Mandelbrot::setWidth(double cw) {
    cWidth = cw;
    cHeight = (double) pixelHeight / (double) pixelWidth * cw;
    startX = centerX - cWidth / 2.0;
    startY = centerY + cHeight / 2.0;
    step = cWidth / (double) pixelWidth;
}

void Mandelbrot::colourFrame(uint8_t *rgb_palette, int palette_size) {
    // Initialise image with black background
    image = new uint8_t[pixelWidth * pixelHeight * 4 + 3];
    image[pixelWidth*pixelHeight*4] = 255;
    image[pixelWidth*pixelHeight*4 + 1] = 255;
    image[pixelWidth*pixelHeight*4 + 2] = 255;
    // Colour in the image
    switch(colour_mode) {
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
        case SMOOTH_HISTOGRAM_COLOURING:
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
            int colour_start,colour_end;
            // Go through each pixel and colour it in
            for(int i = 0; i < total; ++i) {
                colour_start = colour[iterationSpace[i]];
                // If we've reached the end of the palette then we'll just use the last colour
                if(iterationSpace[i] == lastFrameMaxIter) {
                    colour_end = colour_start;
                } else {
                    colour_end = colour[iterationSpace[i] + 1];
                }
                // Red, green, blue channels
                image[i*4] = li_inerpolate(rgb_palette[colour_start*3], rgb_palette[colour_end*3], fracSpace[i]);;
                image[i*4 + 1] = li_inerpolate(rgb_palette[colour_start*3 + 1], rgb_palette[colour_end*3 + 1], fracSpace[i]);;
                image[i*4 + 2] = li_inerpolate(rgb_palette[colour_start*3 + 2], rgb_palette[colour_end*3 + 2], fracSpace[i]);;
                // Full alpha
                image[i*4 + 3] = 255;
            }
            break;
        }
        case SMOOTH_LOOP_COLOURING:
        {
            // Logarithm colouring
            int colour_start,colour_end;
            for(int i = 0; i < pixelWidth * pixelHeight; ++i) {
                colour_start = iterationSpace[i] % palette_size;
                colour_end = (iterationSpace[i] + 1) % palette_size;
                // Red, green, blue channels
                image[i*4] = li_inerpolate(rgb_palette[colour_start*3], rgb_palette[colour_end*3], fracSpace[i]);;
                image[i*4 + 1] = li_inerpolate(rgb_palette[colour_start*3 + 1], rgb_palette[colour_end*3 + 1], fracSpace[i]);;
                image[i*4 + 2] = li_inerpolate(rgb_palette[colour_start*3 + 2], rgb_palette[colour_end*3 + 2], fracSpace[i]);;
                // Full alpha
                image[i*4 + 3] = 255;
            }
            break;
        }
        case LOOP_COLOURING:
        {
            // We do standard colouring but just keep looping round the palette rather than interpolating
            int colour_start;
            for(int i = 0; i < pixelWidth * pixelHeight; ++i) {
                colour_start = iterationSpace[i] % palette_size;
                // Red, green, blue channels
                image[i*4] = rgb_palette[colour_start*3];
                image[i*4 + 1] = rgb_palette[colour_start*3 + 1];
                image[i*4 + 2] = rgb_palette[colour_start*3 + 2];
                // Full alpha
                image[i*4 + 3] = 255;
            }
            break;
        }
		case CIRCULAR_COLOURING:
		{
			// We select hue and saturation from the palette according to angle counter-clockwise from positive x-axis
			// We select luminance according to iteration count
			// First we convert our palette to HSL
			double hsl_palette[palette_size * 3];
			double hsl[3];
			uint8_t rgb[3];
			for(int i =0; i < palette_size; ++i){
				rgb[0]=rgb_palette[i*3];
				rgb[1]=rgb_palette[i*3+1];
				rgb[2]=rgb_palette[i*3+2];
				RGB_2_HSL(hsl,rgb);
				hsl_palette[i*3]=hsl[0];
				hsl_palette[i*3+1]=hsl[1];
				hsl_palette[i*3+2]=hsl[2];
			}
			// Now we loop through our pixels
			int colour_start;
			double x, y, luminance;
			for(int i=0; i < pixelWidth * pixelHeight; ++i){
				// First we pull out colour from palette according to angle
				y = startY - floor( i / pixelWidth) * step;
				x = startX + i % pixelWidth * step;
				colour_start = (int) floor(( (atan2(y,x) + M_PI) / (2 * M_PI) )* (palette_size-1)); 
				// We figure out luminance based on iteration count
				luminance = (double) iterationSpace[i] / (double) lastFrameMaxIter;
				// Now we make our new colour
				hsl[0]=hsl_palette[colour_start*3];
				hsl[1]=hsl_palette[colour_start*3+1];
				hsl[2]=luminance;
				HSL_2_RGB(hsl,rgb);
				//RGB_2_HSL(hsl,rgb);
				//HSL_2_RGB(hsl,rgb);
				// And finally colour in the pixel
				// Red, green, blue channels
				image[i*4] = rgb[0];
				image[i*4+1] = rgb[1];
				image[i*4+2] = rgb[2];
				// Full alpha
				image[i*4+3] = 255;
			}
			break;
		}
		case HISTOGRAM_CIRCULAR_COLOURING:
		{
			// First we convert our palette to HSL
			double hsl_palette[palette_size * 3];
			double hsl[3];
			uint8_t rgb[3];
			for(int i =0; i < palette_size; ++i){
				rgb[0]=rgb_palette[i*3];
				rgb[1]=rgb_palette[i*3+1];
				rgb[2]=rgb_palette[i*3+2];
				RGB_2_HSL(hsl,rgb);
				hsl_palette[i*3]=hsl[0];
				hsl_palette[i*3+1]=hsl[1];
				hsl_palette[i*3+2]=hsl[2];
			}
            int total = pixelWidth * pixelHeight;
            // Build the historgram
            int histogram[lastFrameMaxIter + 1];
            for(int i = 0; i<= lastFrameMaxIter; ++i) {
                histogram[i] = 0;
            }
            for(int i = 0; i < total; i++) {
                histogram[iterationSpace[i]]++;
            }
            // Figure out the luminance for each number of iterations
        	double luminance[lastFrameMaxIter + 1];
            luminance[0] = 0.0;
            double previousProp = (double) histogram[0] / (double) total;
            for(int i = 1; i <= lastFrameMaxIter; ++i) {
                luminance[i] = previousProp;
                previousProp = previousProp + (double) histogram[i] / (double) total;
            }
            // Go through each pixel and colour it in
			int colour_angle;
			double x,y;
            for(int i = 0; i < total; ++i) {
				y = startY - floor( i / pixelWidth) * step;
				x = startX + i % pixelWidth * step;
				colour_angle = (int) floor(( (atan2(y,x) + M_PI) / (2 * M_PI) )* (palette_size-1)); 
				// We construct the colour
				hsl[0]=hsl_palette[colour_angle*3];
				hsl[1]=hsl_palette[colour_angle*3+1];
				hsl[2]=luminance[iterationSpace[i]];
				HSL_2_RGB(hsl,rgb);
				//RGB_2_HSL(hsl,rgb);
				//HSL_2_RGB(hsl,rgb);
				// And finally colour in the pixel
				// Red, green, blue channels
				image[i*4] = rgb[0];
				image[i*4+1] = rgb[1];
				image[i*4+2] = rgb[2];
                // Full Alpha
                image[i*4 + 3] = 255;
            }
			break;
		}
		case SMOOTH_CIRCULAR_COLOURING:
		{
			// We select hue and saturation from the palette according to angle counter-clockwise from positive x-axis
			// We select luminance according to iteration count
			// First we convert our palette to HSL
			double hsl_palette[palette_size * 3];
			double hsl[3];
			uint8_t rgb[3];
			for(int i =0; i < palette_size; ++i){
				rgb[0]=rgb_palette[i*3];
				rgb[1]=rgb_palette[i*3+1];
				rgb[2]=rgb_palette[i*3+2];
				RGB_2_HSL(hsl,rgb);
				hsl_palette[i*3]=hsl[0];
				hsl_palette[i*3+1]=hsl[1];
				hsl_palette[i*3+2]=hsl[2];
			}
			// Now we loop through our pixels
			int colour_start;
			double x, y, luminance, luminance_2, luminance_interpolated;
			for(int i=0; i < pixelWidth * pixelHeight; ++i){
				// First we pull out colour from palette according to angle
				y = startY - floor( i / pixelWidth) * step;
				x = startX + i % pixelWidth * step;
				colour_start = (int) floor(( (atan2(y,x) + M_PI) / (2 * M_PI) )* (palette_size-1)); 
				// We figure out luminance based on iteration count
				luminance = (double) (iterationSpace[i]) / (double) lastFrameMaxIter;
				luminance_2 = luminance + 1.0 / (double) lastFrameMaxIter ;
				if (luminance_2 > 1.0) {
					luminance_2 = 1.0;
				}
				luminance_interpolated = luminance + (luminance_2 - luminance) * fracSpace[i];
				// Now we make our new colour
				hsl[0]=hsl_palette[colour_start*3];
				hsl[1]=hsl_palette[colour_start*3+1];
				hsl[2]=luminance_interpolated;
				HSL_2_RGB(hsl,rgb);
				//RGB_2_HSL(hsl,rgb);
				//HSL_2_RGB(hsl,rgb);
				// And finally colour in the pixel
				// Red, green, blue channels
				image[i*4] = rgb[0];
				image[i*4+1] = rgb[1];
				image[i*4+2] = rgb[2];
				// Full alpha
				image[i*4+3] = 255;
			}
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

void Mandelbrot::writeFrame(GifWriter *writer, uint32_t delay = 0) {
    GifWriteFrame(writer, image, pixelWidth, pixelHeight, delay, 8 , false);
}

void Mandelbrot::setColourMode(int mode){
    colour_mode = mode;
    if(mode == SMOOTH_LOOP_COLOURING or mode == SMOOTH_HISTOGRAM_COLOURING or mode == SMOOTH_CIRCULAR_COLOURING) {
        fracSpace = new double[pixelWidth*pixelHeight];
    }
}

bool Mandelbrot::inCardioidOrBulb(double x,double y) {
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

double Mandelbrot::mod1(double x) {
    while (x >= 1.0) {
        x--;
    }
    while (x < 0.0) {
        x++;
    }
    return x;
}

uint8_t Mandelbrot::li_inerpolate(uint8_t lo, uint8_t hi, double frac) {
    // We linearly interpolate in each of the red green and blue channels
    return lo + (uint8_t) floor( (double)(hi-lo) * frac );
}

void Mandelbrot::iteratePixel(int threadID, int *iterSpace, int pos, double x0, double y0, int maxIter, double escapeRadius) {
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

void Mandelbrot::logIteratePixel(int threadID, int *iterSpace, double *frSpace, int pos, double x0, double y0, int maxIter, double escapeRadius) {

    // We are assuming we are using LOG_COLOURING
    // Could add this check if your image ha>s lots of the cardioid in it
    // Otherwise it might slow down your iterations
    //if(inCardioidOrBulb(x0,y0)) {
    //    iterSpace[pos] = maxIter;
    //    return;
    //}

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
        iterSpace[pos] = 0;
        frSpace[pos] = 0.0;
    } else {
        // This is all the fancy normalising maths
        double log_zn = log(x*x + y*y) / 2.0;
        double nu = log( log_zn / log(2.0) ) / log(2.0);
        double iteration_double = iterations + 1.0 - nu;
        iterSpace[pos] = (int) floor(iteration_double);
        frSpace[pos] = mod1(iteration_double);
    }
}
