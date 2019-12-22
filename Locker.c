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

	unsigned char data, pos = 6;
	const unsigned char ch[12] = {
		'*', '0', '#',
		'7', '8', '9',
		'4', '5', '6',
		'1', '2', '3'
	};
	const char* pwd = "6842";
	char pass[5];
	char* status = "Password";

	LCD4_move((16-strlen(status))/2,0);
	LCD4_writes(status);
	LCD4_move(5,1);
	LCD4_writes("[____]");

	while(1){
		data = GetKeyPressed();
		
		LCD4_move(pos,1);
		if(data != 0xff && data != 0 && data != 2){
			LCD4_write('*');
			
			if((pos-6) > 3){
				LCD4_clear();
				memset(pass,0,strlen(pass));
				status = "overflow";
			}
			
			pass[pos-6] = ch[data];
			pos++;
			
			_delay_ms(150);
		}

		LCD4_move((16-strlen(status))/2,0);
		LCD4_writes(status);
		if(data == 0){
			status = "true";
			for(int x=0;x<4;x++){
				if(pass[x] != pwd[x]){
					status = "false";
					break;
				}
			}

			LCD4_clear();
		}
	}
}
