#include <algorithm>
#include "colours.h"
#include <stdint.h>

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

// Accepts HSL
// Hue in (0,360), saturation in (0,1), lightness in (0,1)
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

// Outputs HSL
// Hue in (0,360), saturation in (0,1), luminance in (0,1)
void RGB_2_HSL(double *hsl, uint8_t *rgb){
	double red = (double) ( (int) rgb[0] )/ 255.0;
	double green = (double) ( (int) rgb[1] ) / 255.0;
	double blue = (double) ( (int) rgb[2] ) / 255.0;
	double min = std::min(red,std::min(green,blue));
	double max = std::max(red,std::max(green,blue));
	// We can figure out luminance straight away
	hsl[2] = (min + max) / 2.0;
	if( min == max) {
		// We have a shade of grey
		hsl[0]=0.0;
		hsl[1]=0.0;
	} else {
		// Calculate saturation
		if(hsl[2] <= 0.5) {
			hsl[1] = (max - min)/(max + min);
		} else {
			hsl[1] = (max - min)/(2.0 - max - min);
		}
		// Calculate hue
		if(red == max) {
			hsl[0] = (green - blue)/(max - min);
		} else if ( green == max ) {
			hsl[0] = 2.0 + (blue - red)/(max - min);
		} else {
			hsl[0] = 4.0 + (red - green)/(max-min);
		}
		hsl[0] = hsl[0] * 60.0;
		if( hsl[0] < 0.0 ) {
			hsl[0] = hsl[0] + 360.0;
		}
	}
}
