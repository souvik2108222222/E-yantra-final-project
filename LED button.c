/* 
*	LED button.c 
 * 
*	Created: 14-01-2024 18:24:27 
*	Author : souvik pal 
 */  
 
#define F_CPU 16000000 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <util/delay.h> 
 
 
// Define debounce delay 
#define DEBOUNCE_DELAY 50  // Adjust as needed 
 
// Function to configure Interrupt switch void interrupt_switch_config(void) { 
 	DDRE &= ~(1 << PE7);   // PORTE 7 pin set as input 
 	PORTE |= (1 << PE7);   // PORTE7 internal pull-up enabled } 
 
// Function to configure LED bar-graph displays void LED_bargraph_config(void) { 
 DDRJ = 0xFF;            // PORT J is configured as output for the first bar-graph  PORTJ = 0x00;           // Output is set to 0 initially  
 	DDRK = 0xFF;            // PORT K is configured as output for the second bar-graph 
 	PORTK = 0x00;           // Output is set to 0 initially } 
 
// Function to Initialize PORTS void port_init(void) {  	interrupt_switch_config(); 
 	LED_bargraph_config(); 
} 
 
// Function to perform software debounce uint8_t debounce_switch(void) {  	static uint8_t state = 0;  // Initial state  	uint8_t current_state = (PINE & (1 << PE7)) == 0; 
 
 	switch (state) { 
 	 	case 0:  // Initial or stable low state  	 	if (current_state == 0) {  	 	 	_delay_ms(DEBOUNCE_DELAY); 
 	 	 	state = 1;  // Move to the next state 
 	 	} 
 	 	break; 
 
 	 	case 1:  // Transition from low to high detected 
 	 	if (current_state == 1) {  	 	 	_delay_ms(DEBOUNCE_DELAY); 
 	 	 	state = 2;  // Move to the next state 
 	 	 	} else { 
 	 	 	state = 0;  // Reset to the initial state 
 	 	} 
 	 	break; 
 
 	 	case 2:  // Stable high state  	 	if (current_state == 1) {  	 	 	return 1;  // Debounced high 
 	 	 	} else { 
 	 	 	_delay_ms(DEBOUNCE_DELAY); 
 	 	 	state = 0;  // Move back to the initial state 
 	 	} 
 	 	break; 
 	} 
 
 	return 0;  // Default: debounced low 
} 
 
// Main Function int main(void) { 
 	port_init(); 
 
 	while (1) { 
 	 	if (debounce_switch()) { 
 	 	 	// Control Bar-graph 1 connected to PORTJ  	 	 	for (uint8_t i = 0; i < 8; i++) {  	 	 	 	PORTJ = (1 << i);  // Turn on the i-th LED 
 	 	 	 	_delay_ms(1);    // Adjust delay as needed 
 	 	 	} 
 
 	 	 	_delay_ms(1);  // Delay before turning off LEDs 
 
 	 	 	// Control Bar-graph 2 connected to PORTK  	 	 	for (uint8_t i = 0; i < 8; i++) {  	 	 	 	PORTK = (1 << i);  // Turn on the i-th LED 
 	 	 	 	_delay_ms(1);    // Adjust delay as needed 
 	 	 	} 
 
 	 	 	_delay_ms(1);  // Delay before starting the next sequence 
 	 	} 
 	} 
 
 	return 0; 
} 
