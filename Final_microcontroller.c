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
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\scheduler.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\PWM.h"
#include "C:\Users\Owner\Documents\Atmel Studio\7.0\Senior_Design\Senior_Design\io.c"
#define inputhall (~PINA & 0x01)
unsigned char vehicle_speed = 0;
unsigned char desired_speed = 0;
unsigned long cnt = 0;
unsigned char i = 0;
unsigned char userinput = 0;
//unsigned char spd = 210;
unsigned char go_spd = 210;


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
		if(cnt < 68000){
			cnt = cnt + 1;
			calc_state = wait_min;
		}
		else{
			calc_state = calculate;
		}
		break;
		
		case calculate:
		vehicle_speed = i;
		cnt = 0;
		i = 0;
		//change state to wait_min after testing
		calc_state = wait_min;
		break;
	}
}

//0 fastest, 255 is off
enum Drive_State{drive, received}drive_state;
void Drive_Motor(){
	switch(drive_state){
		case drive:
		if(USART_HasReceived(0)){
			drive_state = received;
		}
		
		desired_speed = (3*userinput)/(2*3.14159*.032*100);
		if(go_spd <= 3){
			go_spd = 4;
		}
		else if(go_spd >= 250){
			go_spd = 240;
		}
		
		if((go_spd > 3) && (go_spd < 250)){
			//Speed Up
			//must make sure to not pass 0
			if(desired_speed > vehicle_speed){
				go_spd = go_spd - 1;
			}
			//Slow Down
			//make sure to not pass 255
			else if(desired_speed < vehicle_speed){
				go_spd = go_spd + 1;
			}
		}
		
		M1_forward(go_spd);
		break;
		
		case received:
		userinput = USART_Receive(0);
		USART_Send(0x03,0);
		drive_state = drive;
		break;
	}
	
	
}

//PA0 hall effect sensor
//PB3 is connected to IN3 on motor driver
//PB4 is connected to IN4 on motor driver
int main(void){
	DDRC = 0xFF; PORTC = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	motors_init();
	initUSART(0);
	USART_Flush(0);
	while(1){
		Read_Hall_Sensor();
		Hall_Sensor_Calc();
		Drive_Motor();
	}
	return 0;
}