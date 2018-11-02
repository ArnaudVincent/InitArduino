#include <Arduino.h>
#include "Init2_Timer_Basic.h"

void setup() {
	Serial.begin(9600);

	/* Define directions for port pins */
	DDRB |= (1 << PB4);

	display8bit("DDRB", DDRB);

	PRESCALER_X currentPrescaler = getPrescaler(TCCR1B);
	display8bit("Current TCCR1B", TCCR1B);
	Serial.print("Current Prescaler : ");
	Serial.println(currentPrescaler);

	uint8_t current16bitMode = get16bitMode(TCCR1A, TCCR1B);
	display8bit("Current TCCR1A", TCCR1A);
	Serial.print("Current Mode : ");
	Serial.println(current16bitMode);

	setPrescaler(&TCCR1B, 1);
	set16bitMode(&TCCR1A, &TCCR1B, 0);
	TCNT1 = 0x0000;

	delay(50);

	display8bit("Updated TCCR1B", TCCR1B);
	display8bit("Updated TCCR1A", TCCR1A);
	current16bitMode = get16bitMode(TCCR1A, TCCR1B);
	currentPrescaler = getPrescaler(TCCR1B);
	Serial.print("Updated Prescaler : ");
	Serial.println(currentPrescaler);
	Serial.print("Updated Mode : ");
	Serial.println(current16bitMode);
}

uint16_t tmp = 0;

void loop() {
	uint16_t val = TCNT1;

	if (val > 40000) {
		tmp++;

		if (tmp == 400) {
			PORTB ^= (1 << PB4); // toggles the led
			tmp = 0;
		}

		TCNT1 = TCNT1 - 40000;
	}
}

/************************************************************************/
/* FUNCTIONAL APIs                                                                     */
/************************************************************************/

/**
 Get current prescaler from a Timer/Counter n Control Register X (TCCRnX)
 */
PRESCALER_X getPrescaler(uint8_t TCCRnB) {
	uint8_t ret = 0;

	// Extract CS12, CS11 and CS10 values
	for (byte i = 3; i > 0; i--) {
		byte val = (TCCRnB & (1 << (i - 1)));

		if (val != 0) {
			ret |= (1 << (i - 1));
		}
	}

	switch (ret) {
	case 0:
		return PRESCALER_INACTIF;
	case 1:
		return PRESCALER_1; // clk /1 (No prescaling)
	case 2:
		return PRESCALER_8; // clk /8 (From prescaler)
	case 3:
		return PRESCALER_64; // clk /64 (From prescaler)
	case 4:
		return PRESCALER_256; // clk /256 (From prescaler)
	case 5:
		return PRESCALER_1024; // clk /1024 (From prescaler)
	default:
		return -2;
	}
}

/**
 * Waveform Generation Mode Bit Description</br>
 * Modes of operation supported by the Timer/Counter unit
 */
void set16bitMode(uint8_t *TCCRnA, uint8_t *TCCRnB, uint16_t mode) {
	set8Bit(TCCRnB, WGM13, mode & (1 << 3));
	set8Bit(TCCRnB, WGM12, mode & (1 << 2));
	set8Bit(TCCRnA, WGM11, mode & (1 << 1));
	set8Bit(TCCRnA, WGM10, mode & (1 << 0));
}

/**
 Bit 2:0 – CSn2:0: Clock Select
 */
void setPrescaler(uint8_t *TCCRnB, PRESCALER_X prescaler) {
	set8Bit(TCCRnB, CS12, prescaler & (1 << 2));
	set8Bit(TCCRnB, CS11, prescaler & (1 << 1));
	set8Bit(TCCRnB, CS10, prescaler & (1 << 0));
}

/**
 * Waveform Generation Mode Bit Description</br>
 Modes of operation supported by the Timer/Counter unit are: Normal mode (counter),
 Clear Timer on Compare match (CTC) mode, and three types of Pulse Width Modulation (PWM) modes
 */
uint8_t get16bitMode(uint8_t TCCRnA, uint8_t TCCRnB) {
	/*
	 Combined with the WGMn3:2 bits found in the TCCRnB Register, these bits control the counting sequence of the
	 counter, the source for maximum (TOP) counter value, and what type of waveform generation to be used
	 */
	uint8_t ret = 0;

	// Extract WGMn3:2 of B and WGM1:0 of A
	set8Bit(&ret, 3, TCCRnB & (1 << WGM13));
	set8Bit(&ret, 2, TCCRnB & (1 << WGM12));
	set8Bit(&ret, 1, TCCRnA & (1 << WGM11));
	set8Bit(&ret, 0, TCCRnA & (1 << WGM10));

	return ret;
}

/************************************************************************/
/* TECHNICAL APIs                                                       */
/************************************************************************/
void display8bit(char *title, uint8_t reg) {
	char * t;
	t = from8bitsToBinary((uint8_t) reg);
	/* Insert nop for synchronization */

	Serial.print("Bin(");
	Serial.print(title);
	Serial.print(") : ");
	Serial.println(t);
	free(t);
}

/*
 pos:[0;7]
 val: 1/0
 */
void set8Bit(uint8_t *reg, uint8_t pos, uint8_t val) {

	if (val != 0) {
		*reg |= (1 << pos);
	} else {
		*reg &= ~(1 << pos);
	}
}

/**
 Return 8-bits into binary form
 */
char *from8bitsToBinary(volatile uint8_t b) {
	char * strForm = (char *) malloc(12 * sizeof(char));

	if (strForm == NULL) {
		return NULL;
	}

	fullfill(strForm, 12);

	strForm = strcpy(strForm, "B0b");

	for (char i = 8; i >= 1; i--) {
		char v = b & (1 << (i - 1));
		strForm[3 + 8 - i] = (v == 0 ? '0' : '1');
	}

	//strForm[11] = '\0';

	return (strForm);
}

void fullfill(char *str, uint16_t size) {

	for (uint16_t i = 0; i < size; i++) {
		str[i] = '\0';
	}
}
