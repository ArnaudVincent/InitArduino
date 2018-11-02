/*
 * Init2_Timer_Basic.h
 *
 *  Created on: 2 nov. 2018
 *      Author: Arnaud
 */

#ifndef INIT2_TIMER_BASIC_H_
#define INIT2_TIMER_BASIC_H_

#include <Arduino.h>

#define PRESCALER_64 64
#define PRESCALER_256 256
#define PRESCALER_1024 1024
#define PRESCALER_8 8
#define PRESCALER_1 1
#define PRESCALER_INACTIF -1

#define PRESCALER_X uint16_t

void display8bit(char *title, uint8_t reg);
char *from8bitsToBinary(volatile uint8_t b);
void fullfill(char *str, uint16_t size);
PRESCALER_X getPrescaler(uint8_t TCCRnB);

void set8Bit(uint8_t *reg, uint8_t pos, uint8_t val);
uint8_t get16bitMode(uint8_t TCCRnA, uint8_t TCCRnB);
void set16bitMode(uint8_t *TCCRnA, uint8_t *TCCRnB, uint16_t mode);
void setPrescaler(uint8_t *TCCRnB, PRESCALER_X prescaler);

#endif /* INIT2_TIMER_BASIC_H_ */
