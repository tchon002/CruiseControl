//PWM code by https://github.com/cass-m-g/RC_Car Cass Garner GitHub
//0 for fastest
// 255 for off

void M1_forward(unsigned char pwm)
{
	OCR0A = pwm;
	OCR0B = 255;
}

void M1_reverse(unsigned char pwm)
{
	OCR0B = pwm;
	OCR0A = 255;
}

void M2_forward(unsigned char pwm)
{
	OCR2A = pwm;
	OCR2B = 255;
}

void M2_reverse(unsigned char pwm)
{
	OCR2B = pwm;
	OCR2A = 255;
}

void M_off()
{
	OCR2B = 255;
	OCR2A = 255;
	OCR0B = 255;
	OCR0A = 255;
}


// Motor Initialization routine -- this function must be called
//  before you use any of the above functions
void motors_init()
{
	// configure for inverted PWM output on motor control pins:
	//  set OCxx on compare match, clear on timer overflow
	//  Timer0 and Timer2 count up from 0 to 255
	TCCR0A = TCCR2A = 0xF3;
	
	// use the system clock/8 (=2.5 MHz) as the timer clock
	TCCR0B = TCCR2B = 0x02;
	
	// initialize all PWMs to 0% duty cycle (braking)
	M_off();
	//car_speed = 140;
}