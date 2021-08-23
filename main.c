/*
* EasyPWM.c
*
* Created: 8/15/2021 9:41:43 PM
* Author : Sergey Kiselev
*/

#include "rgbPwm.h"
#include "adcIO.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define countsToAdcMeasure 255
#define pwmFullAdc 235 // since pwmAdc cannot be 5v at 100% pwm, this defines what level of ADC we are considering as 100%
#define compareHysteresis 10 // will realize some hysteresis to avoid flickering around compare level

/*
Target device: ATTiny85:
pin 1 - free
pin 2 - ADC3 - input from RC-circuit converting PWM to DC (up to 5v)
pin 3 - OC1B - PWM output for B mosfet
pin 4 - GND
pin 5 - OC0A - PWM output for R mosfet
pin 6 - OC0B - PWM output for G mosfet
pin 7 - ADC1 - input from variable resistor (from 0 to 5V) to set PWM level when switching from rainbow to color of load
pin 8 - VCC
Current status: works; is needed to check PWM signal U and F and choose a right circuit for ADC input
*/

const Color rainbowArr[7] = {{.r = 255, .g = 0, .b = 2}, {.r = 255, .g = 165, .b = 2}, {.r = 255, .g = 255, .b = 2}, {.r = 0, .g = 241, .b = 20}, {.r = 0, .g = 0, .b = 255}, {.r = 75, .g = 0, .b = 130}, {.r = 168, .g = 0, .b = 255}};
const Color zeroPWM = {.r = 0, .g = 255, .b =0}; // Green if PWM = 0%
const Color halfPwm = {.r = 255, .g = 255, .b =0}; // Orange if PWM=50%
const Color fullPwm = {.r = 255, .g = 0, .b =0}; // red if PWM=100%

uint8_t dotInBetween(uint8_t y1, uint8_t y2, uint8_t x12, uint8_t x) {
	int16_t diff = y2 - y1;
	diff = ((diff << 7) / x12) * x;
	return y1 + (diff >> 7);
}

Color setColorInBetween(Color color1, Color color2, uint8_t dot, uint8_t dotsBetween) {
	Color out = {.r = dotInBetween(color1.r, color2.r, dotsBetween, dot),
		.g = dotInBetween(color1.g, color2.g, dotsBetween, dot),
		.b = dotInBetween(color1.b, color2.b, dotsBetween, dot)};
	return out;
}

int main(void)
{
	bool aboveCompare = false; // will realize some hysteresis to avoid flickering around compare level
	initPwm();
	initAdc();
	int arrInd = 0;
	
	while (1)
	{
		if (transitionOver) {
			uint8_t pwm = getPwmAdc();
			uint8_t compare = getCompareAdc();
			aboveCompare = pwm > (compare - compareHysteresis * (aboveCompare));
			
			if (aboveCompare) {
				if (pwm > pwmFullAdc / 2) {
					setTargetRgb(setColorInBetween(halfPwm, fullPwm, pwm - (pwmFullAdc / 2), pwmFullAdc / 2));
				} else {
					setTargetRgb(setColorInBetween(zeroPWM, halfPwm, pwm, pwmFullAdc / 2));
				}
			} else {
				setTargetRgb(rainbowArr[arrInd]);
				arrInd++;
				if (arrInd == 7) arrInd = 0;
			}
			
		}
	}
}

