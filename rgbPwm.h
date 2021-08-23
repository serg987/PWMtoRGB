/*
* rgbPwm.h
*
* Created: 8/15/2021 11:04:24 PM
*  Author: Sergey Kiselev
*/

#define outR DDB0
#define outG DDB1
#define outB DDB4
#define port PORTB
#define portDDR DDRB
#define cycleR OCR0A
#define cycleG OCR0B
#define cycleB OCR1B

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>

volatile uint8_t currentR = 0;
volatile uint8_t currentG = 0;
volatile uint8_t currentB = 0;
volatile uint8_t targetR = 0;
volatile uint8_t targetG = 0;
volatile uint8_t targetB = 0;
volatile int16_t deltaR = 0;
volatile int16_t deltaG = 0;
volatile int16_t deltaB = 0;
volatile uint16_t expandedR = 0;
volatile uint16_t expandedG = 0;
volatile uint16_t expandedB = 0;
volatile bool transitionOver = false;

typedef struct Color{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Color;

void initPwm() {
	// port config
	portDDR |= (1 << outR) | (1 << outG) | (1 << outB);
	
	// Timer 0 config
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1); // Fast PWM non-inverting mode
	TCCR0B = (1 << CS01) | (1 << CS00); // CK / 64
	// Interrupts for Timer0
	TIMSK |= (1 << TOIE0);
	sei();
	
	// Timer 1  config
	TCCR1 |=  (1 << CS12) | (1 << CS11) | (1 << CS10); // CK/64
	GTCCR |= (1 << PWM1B) | (1 << COM1B0); // Enable PWM1B non inverting mod
	OCR1C = 255;
}

uint8_t fMax(int16_t a, int16_t b, int16_t c) {
	if (a > b) {
		if (a > c) return a;
		if (b > c) return b;
		return c;
		} else {
		if (b > c) return b;
		if (a > c) return a;
		return c;
	}
}

void setTargetRgb(Color color) {
	transitionOver = false;
	targetR = color.r;
	targetG = color.g;
	targetB = color.b;
	int16_t changeR = targetR - currentR;
	int16_t changeG = targetG - currentG;
	int16_t changeB = targetB - currentB;
	int16_t	maxSteps = fMax(abs(changeR), abs(changeG), abs(changeB));
	deltaR = (changeR << 7) / maxSteps;
	deltaG = (changeG << 7) / maxSteps;
	deltaB = (changeB << 7) / maxSteps;
}

void transition() {
	if (abs(currentR - targetR) > 0) expandedR += deltaR;
	if (abs(currentG - targetG) > 0) expandedG += deltaG;
	if (abs(currentB - targetB) > 0) expandedB += deltaB;
	currentR = expandedR >> 7;
	currentG = expandedG >> 7;
	currentB = expandedB >> 7;
	transitionOver = (currentR == targetR && currentG == targetG && currentB == targetB);
	cycleR = currentR;
	cycleG = currentG;
	cycleB = currentB;
}

ISR(TIMER0_OVF_vect) {
		transition();
}