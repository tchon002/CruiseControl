#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "io.h"
#include "usart_ATmega1284.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\PWM.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\io.c"

unsigned char temp;
#define buttonA (~PINA & 0x1E)

void Read_Input(){
	if(USART_HasReceived(0)){
		temp = USART_Receive(0);
	}
	PORTC = temp;
	USART_Send(temp,0);
}



int main(void){
	DDRC = 0xFF; PORTC = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	initUSART(0);
	
	while(1){
		Read_Input();
	}
	return 0;
}
