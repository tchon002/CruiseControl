#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
#include "usart_ATmega1284.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\io.c"
#define inputhall (~PINA & 0x01)

enum Hall_State{wait, read}hall_state;
void Read_Hall_Sensor(){
	switch(hall_state){
		case wait:
		if (inputhall == 0x01){
			hall_state = read;
		}
		else{
			hall_state = wait;
		}
		break;
		
		case read:
		if (inputhall == 0x01){
			hall_state = read;
		}
		else{
			hall_state = wait;
			i = i + 1;
		}
		break;
	}

}


enum Hall_Calc{wait_min, calculate}calc_state;
void Hall_Sensor_Calc(){
	switch(calc_state){
		case wait_min:
		//1,200,000 is about 55 seconds
		//1,350,000 is about 1 min
		//22,500 is about 1 second
		if(cnt < 22500){
			cnt = cnt + 1;
			calc_state = wait_min;
		}
		else{
			calc_state = calculate;
		}
		break;
		
		case calculate:
		//rpm = i/(2*3.14*.032);
		rpm = i;
		cnt = 0;
		i = 0;
		//change state to wait_min after testing
		calc_state = wait_min;
		break;
	}
}


//PA0 hall effect sensor
int main(void){
	//int last_tick = clock();
	unsigned char v = 0;
	unsigned char count = 0;
	unsigned char i = 0;
	DDRC = 0xFD; PORTC = 0x02;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	while(1){
		Read_Hall_Sensor();
		Hall_Sensor_Calc();
		PORTD = rpm;
	}
	return 0;
}