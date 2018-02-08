#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
#include "io.h"
#include "usart_ATmega1284.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\io.c"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\PWM.h"
#define buttonA (~PINA & 0x0F)



void Drive_Motor(){
	if(buttonA == 0x01){
		M1_forward(200);
	}
	else if(buttonA == 0x02){
		M1_reverse(200);
	}
	else if(buttonA == 0x04){
		M1_forward(50);
	}
	else if(buttonA == 0x08){
		M1_reverse(50);
	}
	else{
		M_off();
	}
}
//PA0 is motor forward
//PA1 is motor reverse
//PB3 is connected to IN3 on motor driver
//PB4 is connected to IN4 on motor driver
int main(void){
	DDRC = 0xFD; PORTC = 0x02;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	motors_init();
	while(1){
		Drive_Motor();
	}
	return 0;
}