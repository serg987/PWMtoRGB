/*
 * adcIO.h
 *
 * Created: 8/22/2021 11:04:39 AM
 *  Author: Sergey Kiselev
 */ 

#include <avr/io.h>

#ifndef pwmInMux
#define pwmInMux 3 // ADC3 input
#endif

#ifndef compareInMux
#define compareInMux 1 // ADC1 input
#endif

#define adscMask (1 << ADSC)

void initAdc() {
	ADMUX |= (pwmInMux << MUX0) | (1 << ADLAR); // setting Left Adjust the ADCH and ADCL registers (to get 8bit value)
	//ADMUX &= ~(1 << REFS1) & ~(1 << REFS0) // setting AREF=VCC
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1); // setting ADC clock to F_CPU/8
	ADCSRA |= (1 << ADEN);   //enabling the ADC
}

uint8_t getAdc() {
	ADCSRA |= adscMask;
	while (ADCSRA & adscMask); // waiting for conversion
	return ADCH;
}

uint8_t getPwmAdc() {
	ADMUX &= ~(15 << MUX0);
	ADMUX |= (pwmInMux << MUX0);
	return getAdc();
}

uint8_t getCompareAdc() {
	ADMUX &= ~(15 << MUX0);
	ADMUX |= (compareInMux << MUX0);
	return getAdc();
}
