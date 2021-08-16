/*
 * EasyPWM.c
 *
 * Created: 8/15/2021 9:41:43 PM
 * Author : user
 */ 

#include "rgbPwm.h"

#include <avr/io.h>
#include <stdlib.h>

int previousSeed = 1;

/*
	Current status: works for rainbow switching; needed to implement measure of PWM and coloring for PWM > 60% (???)
*/

typedef struct Color{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	} Color;

const Color rainbowArr[7] = {{.r = 255, .g = 0, .b = 2}, {.r = 255, .g = 165, .b = 2}, {.r = 255, .g = 255, .b = 2}, {.r = 0, .g = 241, .b = 20}, {.r = 0, .g = 0, .b = 255}, {.r = 75, .g = 0, .b = 130}, {.r = 168, .g = 0, .b = 255}};

int main(void)
{
	initPwm();
	int arrInd = 0;
	
    while (1) 
    {
		if (transitionOver) {
			setTargetRgb(rainbowArr[arrInd].r, rainbowArr[arrInd].g, rainbowArr[arrInd].b);
			arrInd++;
			if (arrInd == 7) arrInd = 0;
		}
    }
}

