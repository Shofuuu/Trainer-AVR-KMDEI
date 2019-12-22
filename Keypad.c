#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"

unsigned char GetKeyPressed(){
	unsigned char x,c;

	PORTA |= 0xf0; // Pull Up for read data

	for(x=0;x<3;x++){
		DDRA &= ~(0xfe); // All pin set as input
		DDRA |= (0x08>>x); // Select Column

		for(c=0;c<4;c++){
			if(!(PINA & (0x80>>c))){
				return (c*3+x); // Get assorted number button array
			}
		}
	}
	
	return 0xff;
}

int main(){
	LCD4_init();
	LCD4_clear();

	unsigned char data, ch_tmp;
	const unsigned char ch[12] = {
		'*', '0', '#',
		'7', '8', '9',
		'4', '5', '6',
		'1', '2', '3'
	};

	LCD4_move(0,0);
	LCD4_writes("Keypad Demo");

	while(1){
		data = GetKeyPressed();
		
		LCD4_move(0,1);
		if(data != 0xff){
			LCD4_write(ch[data]);
			ch_tmp = data;
		}else
			LCD4_write(' ');
			
		LCD4_move(15,1);
		LCD4_write(ch[ch_tmp]);
	}
}
