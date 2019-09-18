#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

unsigned char number[10]={
	0xc0, 0xf9, 0xa4, 0xb0, 0x99,
	0x92, 0x82, 0xf8, 0x80, 0x90
};

void segment_init(){
	//segment
	DDRC = 0xff;
	DDRD = 0xff;
	PORTC = 0xff;

	//button
	DDRB &= ~(1<<0) & ~(1<<1);

	//buzzer
	DDRB |= (1<<3);
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
		segment_select(0);
		segment_number(num/1000);
		_delay_ms(1);

		segment_select(1);
		segment_number(((num-(num/1000)*1000)/100));
		_delay_ms(1);

		segment_select(2);
		segment_number(((num-(num/100)*100)/10));
		_delay_ms(1);

		segment_select(3);
		if(num>=10) segment_number(((num-(num/10)*10)/1));
		else segment_number(num);
		_delay_ms(1);
}

int main(){
	segment_init();

	int counter = 0;
	unsigned char pa_stat = 0;
	unsigned char pb_stat = 0;

	while(1){
		if(counter == 1000) PORTB |= (1<<3);
		else PORTB &= ~(1<<3);

		if((PINB & (1<<1))==0){
			if(pa_stat == 0){
				if(counter>=9999) counter = 9999;
				else counter++;
			}

			pa_stat = 1;
		}else if((PINB & (1<<1))!=0)
			pa_stat = 0;

		if((PINB & (1<<0))==0){
			if(pb_stat == 0){
				if(counter<=0) counter = 0;
				else counter--;
			}

			pb_stat = 1;
		}else if((PINB & (1<<0))!=0)
			pb_stat = 0;

		segment_write(counter);
	}
}
