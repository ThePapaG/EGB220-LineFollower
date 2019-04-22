
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ISR.h"
#include "Hardware.h"

//implicit decleration
void handle_intersection(void);
void handle_right(void);
void handle_left(void);
bool indicator_check(int i);
void PID(void);
void button_press(void);
void geterror(void);
void readsensor(void);

volatile double Sensor1;
volatile double Sensor2;
volatile double Sensor3;
volatile double Sensor4;
volatile double Sensor5;
volatile double Sensor6;
volatile double LeftIndicator;
volatile double RightIndicator;

volatile int error;
volatile int lasterror;
volatile float correction;
volatile int Multiplier = 1;
volatile int RIGHT = 0;
volatile int limt = 128;
volatile int Laps = 0;
volatile int Sensorcount = 0;
volatile bool CornerIndicator = false;
volatile bool Righton = false;

volatile Kp = 100;
volatile Ki = 0;
volatile Kd = 250;
#define MAX_SPEED 80

volatile int limit = 128;
volatile int INDICATORlimit = 150;

// DON'T CHANGE
#define LED1_ON PORTB |=(1<<0);
#define LED1_OFF PORTB &= ~(1<<0);
#define LED0_ON PORTE |=(1<<6);
#define LED0_OFF PORTE &= ~(1<<6);
#define WHITE < limit
#define BLACK > limit
#define INDICATORWHITE < INDICATORlimit
#define INDICATORBLACK > INDICATORlimit
#define CONVERT ADCSRA |= (1<<ADSC);
#define CONV_WAIT while (ADCSRA & (1<<ADSC));
#define STOP MS_L= 0; MS_LR = 0; MS_R = 0; MS_RR = 0;

void Cornerhandle(){

	if(LeftIndicator INDICATORWHITE && CornerIndicator == false){
		CornerIndicator = true;
		PORTE ^= (1<<6);
	}

	else if(LeftIndicator INDICATORBLACK && CornerIndicator == true){
		CornerIndicator = false;
	}
}

void Righthandle(){

	if(RightIndicator INDICATORWHITE && Righton==false){
		Righton=true;
		RIGHT++;
	}

	else if(RightIndicator INDICATORBLACK && Righton == true){
		Righton=false;
	}

	switch(RIGHT){

		case 1:
			_delay_ms(10);
			STOP
			_delay_ms(1000);
			main();
	}
}

void PID(){

	if(Sensor4 BLACK && Sensor6 BLACK){
		MS_LR=MAX_SPEED/2;
		MS_RR=MAX_SPEED/2;
		MS_L=0;
		MS_R=0;
		while((Sensor4 BLACK && Sensor6 BLACK)){
			readsensor();
		}
	}

	double rightsensor = Sensor4;
	double leftsensor = Sensor6;

	error = rightsensor + (- leftsensor);

	if(Sensor5 BLACK){error = error*2;}

	//find the P and D terms
	float P = Kp * error;
	float D = Kd * (error - lasterror);

	//Calculate the correction needed
	correction = P + D;

	// if it is left then set right faster
	if(correction>0){
		MS_L = MAX_SPEED - ((correction));
		MS_R = MAX_SPEED;
		MS_LR=0;
		MS_RR=0;
	}

	//if it is left then set right faster
	else if(correction<0){
		MS_L = MAX_SPEED;
		MS_R = MAX_SPEED +((correction));
		MS_LR=0;
		MS_RR=0;
	}

	//any other case should be error==0 so set both motors to the same speed
	else if(correction==0){
		MS_L = MAX_SPEED;
		MS_R = MAX_SPEED;
		MS_LR=0;
		MS_RR=0;
	}

  lasterror = error;

  if(MS_L > 255){ MS_L = 255;}
  if(MS_L < 0){ MS_L = 0;}
  if(MS_R > 255){ MS_R = 255;}
  if(MS_R < 0 ){ MS_R = 0;}
}

void readsensor(){

	switch(Sensorcount){

		case 0:

			//Sensor6
			//choose ADC7
			//s4
			ADMUX |= (1<<0)|(1<<1)|(1<<2);
			//Start conversion
			CONVERT
			//wit for conversion
			CONV_WAIT
			Sensor6 = ADCH;
			ADMUX &= ~((1<<0)|(1<<1)|(1<<2));

			//Sensor4
			// choose ADC5
			//s2
			ADMUX |= (1<<0)|(1<<2); 
			// Start conversion
			CONVERT
			// wait for conversion to complete
			CONV_WAIT
			Sensor4 = ADCH;
			ADMUX &= ~((1<<0)|(1<<2));

			//Sensor5
			//choose ADC6
			//s3
			ADMUX |=(1<<1)|(1<<2);
			//Start conversion
			CONVERT
			//wait for conversion
			CONV_WAIT
			Sensor5 = ADCH;
			ADMUX &= ~((1<<1)|(1<<2));

			Sensorcount++;

		break;

		case 1:

		//LeftIndicator
		//Choose ADC11
		//s5
		ADCSRB |= (1<<5);
		ADMUX |= (1<<0)|(1<<1);
		//Start conversion
		CONVERT
		//Wait for conversion
		CONV_WAIT
		LeftIndicator = ADCH;
		ADCSRB &= ~(1<<5);
		ADMUX &= ~((1<<0)|(1<<1));

		//RightIndicator
		//Choose ADC4
		//s1
		ADMUX |= (1<<2);
		//Start conversion
		CONVERT
		//Wait for conversion
		CONV_WAIT
		RightIndicator = ADCH;
		ADMUX & = ~(1<<2);

		Sensorcount = 0;
	}
}

int main(){

    sei();
	timer0_init();
	timer1_init();
	timer3_init();
	ADC_init();
	Button_init();
	RIGHT = 0;

	if(laps==0){
		while(!((PINC >> 7) & 0b1)){
			_delay_ms(2000);
		}
	}

	laps++;

	while(1){
		readsensor();
		PID();
	}

	return 0;
}
