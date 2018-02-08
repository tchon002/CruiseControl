#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
#include "usart_ATmega1284.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\io.c"
#define inputhall (~PINA & 0x01)

void Read_Hall_Sensor(){
	if(inputhall == 0x01){
		PORTB = 0x03;
	}
	else{
		PORTB = 0x00;
	}
}

//PA0 hall effect sensor
int main(void){
	DDRC = 0xFD; PORTC = 0x02;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	while(1){
		Read_Hall_Sensor();
	}
	return 0;
}