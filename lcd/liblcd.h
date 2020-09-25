/*
 * liblcd.h
 *
 *  Created on: 8 de nov de 2017
 *
 *  Biblioteca base para os comandos com o lcd
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 */

#ifndef LIBLCD_H_
#define LIBLCD_H_

#include <msp430.h>
#include <libserial.h>

// Bits para controle do LCD
#define BIT_RS   BIT0
#define BIT_RW   BIT1
#define BIT_E    BIT2
#define BIT_BL   BIT3
#define BIT_D4   BIT4
#define BIT_D5   BIT5
#define BIT_D6   BIT6
#define BIT_D7   BIT7


void lcdi2cstart (void);
void lcdClean(void);
void lcdBacklightON(void);
void lcdBacklightOFF(void);
void lcdZerarRsWr(unsigned char enviar);
void lcdWriteByte(unsigned char ascii, unsigned char config);
void lcdSendNibble(char n ,char config);
void lcdElow(char byte);
void lcdEhigh(char palavra);


unsigned char lastByte=0;                  //Úlitmo valor escrito na porta

#endif /* LIBLCD_H_ */
