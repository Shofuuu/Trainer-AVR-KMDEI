#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define regsel_high() PORTC |= (1<<6)
#define regsel_low() PORTC &= ~(1<<6)
#define enable_high() PORTC |= (1<<7)
#define enable_low() PORTC &= ~(1<<7)

void LCD4_data(unsigned char data){
	if(1 & data) PORTB |= (1<<4);
	else PORTB &= ~(1<<4);
	
	if(2 & data) PORTB |= (1<<5);
	else PORTB &= ~(1<<5);
	
	if(4 & data) PORTB |= (1<<6);
	else PORTB &= ~(1<<6);
	
	if(8 & data) PORTB |= (1<<7);
	else PORTB &= ~(1<<7);
}

void LCD4_cmd(unsigned char cmd){
	regsel_low();
	LCD4_data(cmd);
	enable_high();
	_delay_ms(1);
	enable_low();
	_delay_ms(1);
}

void LCD4_init(){
	//	RS	 EN
	DDRC |= (1<<6) | (1<<7);
	// Data bits
	DDRB |= (1<<4) | (1<<5) | (1<<6) | (1<<7);

	PORTC &= ~(1<<6) & ~(1<<7);
	LCD4_data(0x00);
	_delay_ms(25);
	
	LCD4_cmd(0x03);
	_delay_ms(5);
	LCD4_cmd(0x03);
	_delay_ms(11);
	LCD4_cmd(0x03);
	
	LCD4_cmd(0x02);
	LCD4_cmd(0x02);
	LCD4_cmd(0x08);
	LCD4_cmd(0x00);
	LCD4_cmd(0x0c);
	LCD4_cmd(0x00);
	LCD4_cmd(0x06);
}

void LCD4_write(unsigned char c){
	unsigned char tmp, x;
	tmp = c & 0x0f;
	x = c & 0xf0;
	
	regsel_high();
	LCD4_data(x >> 4);
	enable_high();
	_delay_ms(1);
	
	enable_low();
	_delay_ms(1);
	
	LCD4_data(tmp);
	enable_high();
	_delay_ms(1);
	enable_low();
	_delay_ms(1);
	
	/*regsel_high();
	LCD4_data(c>>4);
	enable_high();
	_delay_ms(1);
	enable_low();
	_delay_ms(1);*/
}

void LCD4_writes(char* s){
	while(*s != 0){
		LCD4_write(*s);
		s++;
	}
}

void LCD4_clear(){
	LCD4_cmd(0x00);
	LCD4_cmd(0x01);
}

void LCD4_move(unsigned char x, unsigned char y){
	unsigned char tmp, a, b;

	if(y == 0){
		tmp = 0x80+x;
		a = tmp >> 4;
		b = (0x80+x) & (0x0f);
		LCD4_cmd(a);
		LCD4_cmd(b);
	}else{
		tmp = 0xc0+x;
		a = tmp >> 4;
		b = (0xc0+x) & (0x0f);
		LCD4_cmd(a);
		LCD4_cmd(b);
	}
}

int main(){
	DDRD |= (1<<7);

	LCD4_init();

	LCD4_clear();
	LCD4_move(2,0);
	LCD4_writes("IPTEK  KMDEI");
	LCD4_move(2,1);
	LCD4_writes("ELINS SV UGM");
	
	while(1);
}
