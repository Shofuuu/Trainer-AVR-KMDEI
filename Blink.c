#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(){
	DDRD = 0b10000000;

	while(1){
		PORTD = 0b10000000;
		_delay_ms(500);
		PORTD = 0b00000000;
		_delay_ms(500);
	}
}
