#include <cstdlib>
#include <cstddef>
#include <iostream>

#include "colours.h"
#include "palette.h"
#include "gif.h"

int main(void) {
	int palette_size = 135;
	uint8_t palette[135*3];
	getPaletteOne(palette);

	const int delay = 10;
	const char *filename = "outputs/colour_test2";
	const int gifWidth = 100;
	const int gifHeight = 135;

	GifWriter writer;
	GifBegin(&writer, filename, gifWidth, gifHeight, delay);
	uint8_t image[gifWidth*gifHeight*4 + 3];

	image[gifWidth*gifHeight*4] = 255;
	image[gifWidth*gifHeight*4+1] = 255;
	image[gifWidth*gifHeight*4+2] = 255;

	uint8_t rgb[3], rgb_save[3];
	double hsl[3];
	int dist, pixel;;
	for(int colour = 0; colour < palette_size; ++colour){
		rgb[0] = palette[colour*3];
		rgb[1] = palette[colour*3+1];
		rgb[2] = palette[colour*3+2];
		rgb_save[0] = rgb[0];
		rgb_save[1] = rgb[1];
		rgb_save[2] = rgb[2];
		RGB_2_HSL(hsl,rgb);
		HSL_2_RGB(hsl,rgb);
		dist = abs( rgb_save[0] - rgb[0] ) + abs( rgb_save[1] - rgb[1] ) + abs( rgb_save[2] - rgb[2]); 
		for(int i = 0; i < gifWidth; ++i) {
			pixel = colour*gifWidth + i;
			image[pixel*4] = rgb[0];
			image[pixel*4 + 1] = rgb[1];
			image[pixel*4 + 2]= rgb[2];
		}
		if (dist > 5) {
			std::cout << (int) rgb[0] << ":" << (int) rgb[1] << ":" << (int) rgb[2] << "---" << (int) rgb_save[0] << ":" << (int) rgb_save[1] << ":" << (int) rgb_save[2] << std::endl;
			std::cout.flush();
		}
	}
	GifWriteFrame(&writer, image, gifWidth, gifHeight, delay, 8, false);
	GifEnd(&writer);
}
