#ifndef __MANDELBROT_C
#define __MANDELBROT_C

struct GifWriter;

class Mandelbrot{
public:
    // Constants for setting colouring algorithm
    const static int NORMAL_COLOURING;
    const static int HISTOGRAM_COLOURING;
    const static int LOOP_COLOURING;
    const static int SMOOTH_HISTOGRAM_COLOURING;
    const static int SMOOTH_LOOP_COLOURING;
	const static int CIRCULAR_COLOURING;
	const static int HISTOGRAM_CIRCULAR_COLOURING;
	const static int SMOOTH_CIRCULAR_COLOURING;
    // Width and height of image in pixels
    int pixelWidth, pixelHeight;
    // Width and height of the image in the complex plane
    double cWidth, cHeight;
    // The coordinates of the centre of the image in the complex plane
    double centerX, centerY;
    // Initialiser requiring, pixelWidth, pixelHeight, cWidth, centerX, centerY
    Mandelbrot(int pw, int ph, double cw, double x, double y);
    // Call this to compute the actual mandelbrot set
    // Provide the maximum number of iterations, espcape radius and desired number of threads
    void iterate(int maxIter, double escapeRadius, int NUM_THREADS);
    // Print mandelbrot to the terminal with '*'
    void print();
    // Set the centre of the image in the complex plane
    void setCentre(double cx, double cy);
    // Set the width of the image in the complex plane
    void setWidth(double cw);
    // After iterating, colour the frame in according to the colouring algorithm
    void colourFrame(uint8_t *rgb_palette, int palette_size);
    // Write the coloured image to file
    void writeFrame(GifWriter *writer, uint32_t delay);
    // Set the colouring algorithm
    void setColourMode(int mode);
    // Useful test for checking if the point x+iy is in the cardiod or bulb of the Mandelbrot set.
    static bool inCardioidOrBulb(double x,double y);

private:
    // Reference to array storing the number of iterations each point took
    int *iterationSpace;
    // Reference to array storing a fractional part for each point (for smooth colouring algorithms)
    double *fracSpace;
    // The image stored in RGBA format
    // Size of array is pixelWidth*pixelHeight*4 + 3
    // Arrays starts with RGBA channels for each pixel in return
    // Followed by RGB for background of image
    uint8_t *image;
    // The coordiantes in the complex plane of the top left corner of the image
    double startX, startY;
    // The gap in the complex plane between each pixel
    double step;
    // The maximum number of iterations used to compute the last frame
    int lastFrameMaxIter;
    // The constant referring to the colouring algorithm
    int colour_mode;
    // Helper function to calculate a real number modulo 1
    static double mod1(double x);
    // Helper function to linearly interpolate between lo and hi by frac
    static uint8_t li_inerpolate(uint8_t lo, uint8_t hi, double frac);
    // Actual function for computing the escape time for a given point in the plane
    // Accepts threadID, the iterations output array (normally iterationSpace above), the position to output the result in the array,
    // Coords of point in mandelbrot set, maximum number of iterations and escape radius
    static void iteratePixel(int threadID, int *iterSpace, int pos, double x0, double y0, int maxIter, double escapeRadius);
    // Same as above but also computes a fractial iteration part for smooth colouring algorithms
    // Extra argument is for specififying the output array for this fractional part
    static void logIteratePixel(int threadID, int *iterSpace, double *frSpace, int pos, double x0, double y0, int maxIter, double escapeRadius);
};

#endif
