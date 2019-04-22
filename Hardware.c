
#include <avr/io.h>

// don't touch
void timer0_init(){
	DDRB |= (1<<7);
	DDRD |= (1<<0);
	TCCR0A |= (1<<7)|(1<<5)|(1<<1)|1;
	TIMSK0 |= (1<<0);
	OCR0A = 0;
	OCR0B = 0;
	TCCR0B |= (1<<2);
}

//don't touch
void timer1_init(){
	DDRB |= (1<<6)|(1<<5);
	TCCR1A |= (1<<7)|(1<<5)|1;
	TCCR1B |= (1<<3);
	TIMSK1 |= (1<<0);
	OCR1A = 0;
	OCR1B = 0;
	TCCR1B |= (1<<2);	
}

void timer3_init(){
	TCCR3B |= (1<<CS30)|(1<<CS31);
	TIMSK3 |= (1<<0);
}

void ADC_init(){
	
	//configure the multiplexer to run in single conversion mode not interrupt
	ADMUX |= (1<<7)|(1<<6)|(1<<5);
	ADCSRA |= (1<<7)|(1<<2)|(1<<1)|1;
}

void Button_init(){
	//enable sw1
	DDRC &= ~(1<<0)|(1<<7);

	//LED setup
	DDRE |=(1<<6);
	DDRB |=(1<<0)|(1<<1)|(1<<2);

}