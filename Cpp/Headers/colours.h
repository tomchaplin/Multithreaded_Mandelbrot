#ifndef __COLOURS_FUNCS
#define __COLOURS_FUNCS

#include <stdint.h>

double mod1(double x);
double hsl_tests(double x, double tmp_1, double tmp_2);
void HSL_2_RGB(double *hsl, uint8_t *rgb);
void RGB_2_HSL(double *hsl, uint8_t *rgb);

#endif
