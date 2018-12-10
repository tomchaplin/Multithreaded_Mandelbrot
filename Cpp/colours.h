// Theses are just some helper functions for creating palettes

#ifndef __COLOUR_HELPERS
#define __COLOUR_HELPERS

double mod1(double x) {
    while (x >= 1.0) {
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

#endif
