#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "adc.h"

unsigned char number[10]={
	0xc0, 0xf9, 0xa4, 0xb0, 0x99,
	0x92, 0x82, 0xf8, 0x80, 0x90
};

int _pow(int x, int y){
        if(y == 0)
        	return 1;
        else if(y == 1)
        	return x;
        else
        	return x*_pow(x,y-1);
}

int get_value(unsigned char digit, int num){
	return ((num-(num/_pow(10,digit))*_pow(10,digit))/_pow(10,digit-1));
}

void segment_init(){
	//segment
	DDRC = 0xff;
	DDRD = 0xff;
	PORTC = 0xff;
}

void segment_select(unsigned char index){
	if(index > 3) index = 3;
	PORTC = ~(1<<(index+2));
}

void segment_number(unsigned char num){
	if(num != 1 && num != 4){
		PORTD = number[num];
		_delay_ms(3);
		PORTD = (number[num]<<7);
		PORTD = ~(number[num]>>7);
		_delay_ms(1);
	}else{
		PORTD = number[num];
		_delay_ms(1);
	}
}

void segment_write(int num){
	int y=4;

	for(int x=0;x<4;x++,y--){
		segment_select(x);
		segment_number(get_value(y,num));
		_delay_ms(1);
	}
}

int main(){
	segment_init();
	ADC_init();

	while(1){
		segment_write(ADC_Read());
	}
}
