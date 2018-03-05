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
	PORTB |=(1<<PB0); // turn on the pin we use to strobe (its active low, so rest is high)
	
	//Test pin from hw de-bounced button
	DDRB &= ~(1<<DDB3);
	
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

// NOT de-bounced for demo purposes only
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
		uint8_t button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
		volatile uint8_t in_wait_mode = 1;
		if ((button_value == 1) & (in_wait_mode == 1)){
			in_wait_mode = 0;
			//toggle led
			_delay_ms(50.0);
			if((PINB & (1<<PINB2)) ? 0 : 1){
				PORTB |= (1<<PB1); // turn on led
				PORTB &= ~(1<<PB0); // drop 
				_delay_us(2.0);
				PORTB |= (1<<PB0);
				_delay_ms(1000.0);
				PORTB &= ~(1<<PB1); //turn off led
				in_wait_mode = 1;
			}
			
		} else {
//			_delay_ms(500);
			in_wait_mode = 1;
//			PORTB ^= (1<<PB1);
		}
	}
}

void btn_trigger(){
	// trigger when we go from high to low (off to on)
	//	uint8_t last_button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
	while(1){
		uint8_t button_value = (PINB & (1<<PINB2)) ? 0 : 1; //read the button
		volatile uint8_t in_wait_mode = 1;
		if ((button_value == 1) & (in_wait_mode == 1)){
			in_wait_mode = 0;
			//toggle led
			_delay_ms(50.0);
			if((PINB & (1<<PINB2)) ? 1 : 0){
				PORTB |= (1<<PB1); // turn on led
				PORTB &= ~(1<<PB0); // drop
				_delay_us(2.0);
				PORTB |= (1<<PB0);
				_delay_ms(1000.0);
				PORTB &= ~(1<<PB1); //turn off led
				in_wait_mode = 1;
			}
			
			} else {
			//			_delay_ms(500);
			in_wait_mode = 1;

		}
	}
}


// fast blinking, then returns and puts in wait for a pin to go low (also sets led to solid
wait_for_btn(){
		// trigger when we go from high to low (off to on)
		//	uint8_t last_button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
		while(1){
			uint8_t button_value = (PINB & (1<<PINB2)) ? 0 : 1; //read the button
			volatile uint8_t in_wait_mode = 1;
			if ((button_value == 1) & (in_wait_mode == 1)){
				in_wait_mode = 0;
				//toggle led
				_delay_ms(50.0);
				if((PINB & (1<<PINB2)) ? 1 : 0){
					PORTB |= (1<<PB1); // turn on led

					_delay_ms(1000.0);
					PORTB &= ~(1<<PB1); //turn off led
					in_wait_mode = 1;
					return;

				}
				
				} else {
				in_wait_mode = 1;
				_delay_ms(100.0);
				PORTB ^= (1<<PB1);
				in_wait_mode = 1;
			}
		}
}

// assumes the clock is active low, look at PB4
void wait_for_clk_trigger(){
	
	while(1){
		//wait for trigger from pin
		// the clk should be high and we trigger when it goes low
		uint8_t hitcount = 0;
		uint8_t sclk_val = 0;
		
		//PB4 is where we r looking for sclk
		break;
	}
	PORTB |= (1<<PB1); // turn on led
	
	PORTB &= ~(1<<PB0); // drop
	_delay_us(2.0);
	PORTB |= (1<<PB0);
	_delay_ms(3000.0);
	return;
}

// assumes the clock is active high, look at PB3, we have a hardware de-bounced switcher there (digilent Pmod BTN 4)
void wait_for_clk_trigger_test(){
	
	PORTB |= (1<<PB1); // turn on led (to show user we are in this mode)
	while(1){
		//wait for trigger from pin
		// the clk should be high and we trigger when it goes low
		uint8_t hitcount = 0;
		uint8_t pmod_btn_val = PINB & (1<<PINB3);
		if(pmod_btn_val == 0){
			PORTB &= ~(1<<PB1); //turn off led as indicator
			_delay_us(14.0);
			PORTB &= ~(1<<PB0); // drop
			_delay_us(1.0);
			PORTB |= (1<<PB0);
			_delay_ms(1000.0); //bug here is the user re runs within a second
			return;
		}
	}
	return;
}

void trigger_low(){
	// drops pin for 2us
	// trigger when we go from high to low (off to on)
	//	uint8_t last_button_value = (PINB & (1<<PINB2)) ? 1 : 0; //read the button
	while(1){
		uint8_t button_value = (PINB & (1<<PINB2)) ? 0 : 1; //read the button
		if (button_value == 1){
			//toggle led
			_delay_ms(50.0);
			if((PINB & (1<<PINB2)) ? 1 : 0){
				PORTB ^= (1<<PB1);
				
			}
			
		}
	}
}

int main(void)
{
	init_avr();
	while(1){
		wait_for_btn();
		wait_for_clk_trigger_test();
	}
}

