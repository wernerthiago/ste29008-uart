/*
 * Uart.cpp
 *
 *  Created on: 13 de nov de 2015
 *      Author: aluno
 */

#include "Uart.h"
#include "avr/io.h"
#include "avr/interrupt.h"

Uart Uart::uart;


Uart::Uart(){
	//SERIAL
	UBRR0H = 0;
	UBRR0L = 103;
	UCSR0C = 0x06;
	UCSR0B = 0x98;
	//LED
	PINB = 0x23;
	DDRB = 0x24;
	PORTB = 0x80;
}

void Uart::put(uint8_t c) {
    uart.tx.push(c);
    UCSR0B |= 0x20;
}

uint8_t Uart::get() {
    return uart.rx.pop();
}

bool Uart::has_data() {
    return uart.rx.has_data();
}

void Uart::rx_interrupt_handler() {
    uart.rx.push(UDR0);

	//TEST
//	PORTB &= ~_BV(PORTB5);	//LIGA LED
//	PORTB |= _BV(PORTB5);	//DESLIGA LED
}

void Uart::empty_interrupt_handler(){
	//TEST
//	PORTB &= ~_BV(PORTB5);	//LIGA LED
//	PORTB |= _BV(PORTB5);	//DESLIGA LED
    if(uart.tx.has_data())
        UDR0 = uart.tx.pop();
    else UCSR0B &= ~0x20;
}

ISR(USART_RX_vect ){
	Uart::rx_interrupt_handler();
}

ISR(USART_UDRE_vect ){
	Uart::empty_interrupt_handler();
}

void Uart::interrupton(){
	sei();
}

void Uart::interruptoff(){
	cli();
}