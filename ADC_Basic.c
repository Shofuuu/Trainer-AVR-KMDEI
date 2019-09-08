#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "segment.h"

void ADC_init(){
	ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	ADMUX |= (1<<REFS1) | (1<<REFS0);
}

int ADC_Read(){
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)));
	return ADCW;
}

int main(){
	ADC_init();
	segment_init();
	DDRB |= (1<<3);
	
	while(1){
		if(ADC_Read() > 800) PORTB |= (1<<3);
		else PORTB &= ~(1<<3);
		segment_write(ADC_Read());
	}
}
