#include <Arduino.h>

void setup() {

	Serial.begin(9600);

	/**
	 The DDxn bit in the DDRx Register selects the direction of this pin. If DDxn is written logic one, Pxn is configured
	 as an output pin. If DDxn is written logic zero, Pxn is configured as an input pin.
	 */
	DDRB |= (1 << PB4); // Set port 10 as Output (1)

	/**
	 If PORTxn is written logic one when the pin is configured as an output pin, the port pin is driven high (one). If
	 PORTxn is written logic zero when the pin is configured as an output pin, the port pin is driven low (zero).
	 */
	PORTB ^= (1 << PB4);
}

void loop() {

}
