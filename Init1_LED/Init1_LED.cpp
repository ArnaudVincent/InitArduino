#include <Arduino.h>

void setup() {

	Serial.begin(9600);

	DDRB |= (1 << PB4);

	PORTB ^= (1 << PB4);
}

void loop() {

}
