
#include <avr/io.h>
#include <avr/interrupt.h>

volatile float MS_L;
volatile float MS_R;

volatile float MS_LR;
volatile float MS_RR;

ISR(TIMER0_OVF_vect){

		OCR0B = MS_L;
		OCR0A = MS_LR;	
}


ISR(TIMER1_OVF_vect){

		OCR1B = MS_R;
		OCR1A = MS_RR;	
}



