//Assistance from http://stackoverflow.com/questions/28896680/interfacing-ultrasonic-range-sensor-hcsr-04-with-avr-atmega-32
//Took code from source and reworked it to fit my atmega and my project
#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
#include "usart_ATmega1284.h"

//Trig C0
//Echo C1
void Sonar_Init(){
	TCCR1B |= 1 << CS10;
	DDRC |= 1 << PINC0;
}
void Read_Sonar(){
	int a = 0;
	int b = 0;
	TCNT1=0;
	PORTC|=1<<PINC0;
	while(TCNT1<100);
	PORTC&=~(1<<PINC0);

	TCNT1=0;
	while(!(PINC&(1<<PINC1)) && TCNT1<300000) ;  // checking if echo has become high  and not exceeding the time for max range i.e 5 m
	///send_string("out");
	b=TCNT1;
	if(b<300000){
		TCNT1=0;
		while(PINC&(1<<PINC1));    // waiting until the echo become low again
		a=TCNT1;
		a = a/58;
		PORTA = a;
	}
	_delay_ms(50);
}

int main(void){
	DDRC = 0xD; PORTC = 0x02;
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	Sonar_Init();
	while(1){
		Read_Sonar();
	}
}
