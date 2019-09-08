#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void USART_init(int baudrate){
	int ubrr = ((F_CPU)/(baudrate*16UL)-1);

	UBRRL = (unsigned char)(ubrr);
	UBRRH = (ubrr>>8);
	UCSRB |= (1<<RXEN) | (1<<TXEN);
	UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

unsigned char USART_read(void){
	while(!(UCSRA & (1<<RXC)));
	return UDR;
}

void USART_write(char c){
	while(!(UCSRA & (1<<UDRE)));
	UDR = c;
}

void USART_writes(char* s){
	while(*s){
		USART_write(*s);
		s++;
	}
}

void USART_writeln(char* s){
	while(*s){
		USART_write(*s);
		s++;
	}
	for(int x=0;x<strlen(&s);x++)
		USART_write('\r');
	USART_write('\n');
}

int main(){
	USART_init(9600);
	USART_writeln("KMDEI Keluarga Mahasiswa Diploma Elektronika dan Instrumentasi");

	while(1);
}
