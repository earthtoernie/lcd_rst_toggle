/*
 * pin_toggle.c
 *
 * Created: 3/4/2018 1:35:51 PM
 * Author : Ernie
 */

//http://www.elecrom.com/avr-tutorial-2-avr-input-output/

#define F_CPU 2000000UL


// PB0, PB1, PB2, PB3, PB4, PB5
#include <avr/io.h>
#include<util/delay.h>

// sets up the pins and timers and what not
void init_avr(){
	//bad pins... (don't use)
	//don't set B3 to output. messes with usb power?? (D-)
	//also B4 is usb (D+)
	//B5 is reset
	//(good pins)
	// light (MISO) is B1
	// B0 (MOSI) (lets use for other output output)
	// B2 (sclk.. use for input to button)
	DDRB |= (1<<DDB1); // LED as output
	DDRB |= (1<<DDB0); // other pin as output
	DDRB &= ~(1<<DDB2); // input to switch
	PORTB |= (1<<PB2); // enable pull-up
	PORTB |=(1<<PB0); // turn on the pin we use to strobe (its active low, so rest in high)
	return;
}

//used to test sleep and clock rate. not used in actual project
//never terminates
void toggle_led(){
	while(1){
		_delay_ms(1000);
		PORTB |=(1<<PB1); // turn on led
		_delay_ms(1000);
		PORTB &=~(1<<PB1); // turn off led
	}
}

void poll_btn(){
	// button is active low
	while (1){
		// say one if button is 'pushed' (really low)
		uint8_t button_value = (PINB & (1<<PINB2)) ? 0 : 1; //read the button
		if(button_value == 1){
			PORTB |=(1<<PB1); // turn on led
		} else {
			PORTB &=~(1<<PB1); // turn off led
		}
	}
}

// NOT de-bounced for demo purposes onle
void bad_bounce(){
	// trigger when we go from high to low (off to on)
//	uint8_t last_button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
	while(1){
		uint8_t button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
		if (button_value == 1){
			//toggle led
			PORTB ^= (1<<PB1);
			while((PINB & (1<<PINB2)) ? 1 : 0){
				// do nothing while till release
			}
		}	
	}
}

//not the best bounce
void good_bounce(){
	// trigger when we go from high to low (off to on)
	//	uint8_t last_button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
	while(1){
		uint8_t button_value = (PINB & (1<<PINB2)) ? 0 : 1; //read the button
		volatile uint8_t in_wait_mode = 1;
		if ((button_value == 1) & (in_wait_mode == 1)){
			in_wait_mode = 0;
			//toggle led
			_delay_ms(50);
			if((PINB & (1<<PINB2)) ? 1 : 0){
				PORTB |= (1<<PB1); // turn on led
				PORTB &= ~(1<<PB0); // drop 
				_delay_us(2);
				PORTB |= (1<<PB0);
				_delay_ms(1000);
				PORTB &= ~(1<<PB1); //turn off led
				in_wait_mode = 1;
			}
			
		} else {
//			_delay_ms(500);
			in_wait_mode = 1;
		}
	}
}

void trigger_low(){
	// drops pin for 2us
	// trigger when we go from high to low (off to on)
	//	uint8_t last_button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
	while(1){
		uint8_t button_value = (PINB & (1<<PINB2)) ? 0 : 1; //read the button
		if (button_value == 1){
			//toggle led
			_delay_ms(50);
			if((PINB & (1<<PINB2)) ? 1 : 0){
				PORTB ^= (1<<PB1);
				
			}
			
		}
	}
}

int main(void)
{
	init_avr();
	good_bounce();
}

