#ifndef __MANDELBROT_C
#define __MANDELBROT_C

struct GifWriter;

class Mandelbrot{
public:
    const static int NORMAL_COLOURING;
    const static int HISTOGRAM_COLOURING;
    const static int LOOP_COLOURING;
    const static int SMOOTH_HISTOGRAM_COLOURING;
    const static int SMOOTH_LOOP_COLOURING;
    int pixelWidth, pixelHeight;
    double cWidth, cHeight;
    double centerX, centerY;
    Mandelbrot(int pw, int ph, double cw, double x, double y);
    void iterate(int maxIter, double escapeRadius, int NUM_THREADS);
    void print();
    void setCentre(double cx, double cy);
    void setWidth(double cw);
    void colourFrame(uint8_t *rgb_palette, int palette_size);
    void writeFrame(GifWriter *writer, uint32_t delay);
    void setColourMode(int mode);
    static bool inCardioidOrBulb(double x,double y);

private:
    int *iterationSpace;
    double *fracSpace;
    uint8_t *image;
    double startX, startY;
    double step;
    int lastFrameMaxIter;
    int colour_mode;
    static double mod1(double x);
    static uint8_t li_inerpolate(uint8_t lo, uint8_t hi, double frac);
    static void iteratePixel(int threadID, int *iterSpace, int pos, double x0, double y0, int maxIter, double escapeRadius);
    static void logIteratePixel(int threadID, int *iterSpace, double *frSpace, int pos, double x0, double y0, int maxIter, double escapeRadius);
};

#endif
