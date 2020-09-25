/*
 * libserial.h
 *
 *  Created on: 8 de nov de 2017
 *
 * Biblioteca base para os comandos da comunicação serial I2C
 *
 * Pinos 3.0 e 3.1 como SDA e SCL respectivamente.
 * Configuração no arquivo .c
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 */


#ifndef LIBSERIAL_H_
#define LIBSERIAL_H_

#include "msp430.h"
#define Atrasar100us   atrasar(3)                   //Medido 104 useg
#define Atrasar500us   atrasar(15)      //513 us
#define Atrasar1ms     atrasar(30)     //1,08 ms
#define Atrasar5ms     atrasar(150)       //5,07 ms
#define Atrasar15ms    atrasar(450)       //15,6 ms

void ucbConfigPinos(void);

void ucbConfig(unsigned char end);

int ucbEnviaMsg(unsigned char msg);

void atrasar (unsigned long atrasa);

#endif /* LIBSERIAL_H_ */
