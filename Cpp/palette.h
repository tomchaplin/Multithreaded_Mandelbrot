// These are some functions which create palettes

#ifndef __PALETTE_FUNCS
#define __PALETTE_FUNCS

#include <cstddef>
#include <iostream>
#include <cstdlib>

#include "colours.h"

// palette should be a uint8_t array of size 135*3
const void getPaletteOne(uint8_t *palette) {
    uint8_t rgb[3];
    double hsl[3];

    hsl[0] = 287.0;
    hsl[1] = 1.0;
    for(int i = 0; i <= 17; i++) {
        hsl[2] = (double) i / 100.0;
        HSL_2_RGB(hsl, rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }

    for(int i=18; i<=70; i++) {
        hsl[0]++;
        hsl[2] = hsl[2] + (0.19 / 53.0);
        HSL_2_RGB(hsl, rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }

    for(int i=71; i<=90; i++) {
        hsl[0]++;
        hsl[2] = hsl[2] + (0.19 / 64.0);
        HSL_2_RGB(hsl, rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }

    hsl[0] = 0.0;

    for(int i=91; i<=134; i++) {
        hsl[0]++;
        hsl[2] = hsl[2] + (0.19 / 64.0);
        HSL_2_RGB(hsl, rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }
}

// palette should be a uint8_t array of size 460*3
const void getPaletteTwo(uint8_t *palette) {
    uint8_t rgb[3];
    double hsl[3];
    hsl[0] = 0.0;
    hsl[1] = 0.5;

    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)i / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }

    hsl[2] = 0.50;

    for(int i=0; i<360; i++) {
        hsl[0] = (double)i;
        HSL_2_RGB(hsl,rgb);
        palette[(i+50)*3] = rgb[0];
        palette[(i+50)*3 + 1] = rgb[1];
        palette[(i+50)*3 + 2] = rgb[2];
    }

    hsl[0] = 0.0;

    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)(50-i) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[(i+410)*3] = rgb[0];
        palette[(i+410)*3 + 1] = rgb[1];
        palette[(i+410)*3 + 2] = rgb[2];
    }
}

// palette should be a uint8_t array of size 510*3
const void getPaletteThree(uint8_t *palette) {
    uint8_t rgb[3];
    double hsl[3];
    hsl[0] = 360.0;
    hsl[1] = 0.5;

    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)i / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }

    hsl[2] = 0.50;

    for(int i=0; i<180; i++) {
        hsl[0] = (double)(360-i);
        HSL_2_RGB(hsl,rgb);
        palette[(i+50)*3] = rgb[0];
        palette[(i+50)*3 + 1] = rgb[1];
        palette[(i+50)*3 + 2] = rgb[2];
    }

    for(int i = 0; i< 50; i++) {
        hsl[1] = (double)(i+50) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[(i+230)*3] = rgb[0];
        palette[(i+230)*3 + 2] = rgb[2];
        palette[(i+230)*3 + 1] = rgb[1];
    }

    hsl[1]=1.0;

    for(int i=0; i<180; i++) {
        hsl[0] = (double)(i+180);
        HSL_2_RGB(hsl,rgb);
        palette[(i+280)*3] = rgb[0];
        palette[(i+280)*3 + 1] = rgb[1];
        palette[(i+280)*3 + 2] = rgb[2];
    }

    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)(50-i) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[(i+460)*3] = rgb[0];
        palette[(i+460)*3 + 1] = rgb[1];
        palette[(i+460)*3 + 2] = rgb[2];
    }
}

// palette should be a uint8_t array of size 231*3
const void getPaletteFour(uint8_t *palette) {
    uint8_t rgb[3];
    double hsl[3];

    // Add black
    hsl[0] = 360.0;
    hsl[1] = 0.5;
    hsl[2] = 0.0;
    HSL_2_RGB(hsl,rgb);
    palette[0] = rgb[0];
    palette[1] = rgb[1];
    palette[2] = rgb[2];

    // White down to middle saturation
    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)(100-i) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[(i+1)*3] = rgb[0];
        palette[(i+1)*3 + 1] = rgb[1];
        palette[(i+1)*3 + 2] = rgb[2];
    }

    hsl[2] = 0.50;

    // Red to blue
    for(int i=0; i<120; i++) {
        hsl[0] = (double)(360-i);
        HSL_2_RGB(hsl,rgb);
        palette[(i+51)*3] = rgb[0];
        palette[(i+51)*3 + 1] = rgb[1];
        palette[(i+51)*3 + 2] = rgb[2];
    }

    hsl[0] = 240.0;

    // Fade to black
    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)(50-i) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[(i+171)*3] = rgb[0];
        palette[(i+171)*3 + 2] = rgb[2];
        palette[(i+171)*3 + 1] = rgb[1];
    }
}

// palette should be a uint8_t array of size 231*3
const void getPaletteFive(uint8_t *palette) {
    uint8_t rgb[3];
    double hsl[3];

    // Add black
    hsl[0] = 360.0;
    hsl[1] = 0.5;

    // Black to red
    for(int i = 0; i< 50; i++) {
        hsl[2] = (double)(i) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[i*3] = rgb[0];
        palette[i*3 + 1] = rgb[1];
        palette[i*3 + 2] = rgb[2];
    }

    hsl[2] = 0.5;

    // Red to blue
    for(int i=0; i<120; i++) {
        hsl[0] = (double)(360-i);
        HSL_2_RGB(hsl,rgb);
        palette[(i+50)*3] = rgb[0];
        palette[(i+50)*3 + 1] = rgb[1];
        palette[(i+50)*3 + 2] = rgb[2];
    }

    hsl[0] = 240.0;

    // Blue to white
    for(int i = 0; i<= 50; i++) {
        hsl[2] = (double)(i+50) / 100.0;
        HSL_2_RGB(hsl,rgb);
        palette[(i+170)*3] = rgb[0];
        palette[(i+170)*3 + 1] = rgb[1];
        palette[(i+170)*3 + 2] = rgb[2];
    }
}

#endif
