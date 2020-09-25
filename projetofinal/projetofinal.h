/*
 * projetofinal.h
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 *  Projeto final
 */

#ifndef PROJETOFINAL_H_
#define PROJETOFINAL_H_


#include "msp430.h"

#define Atrasar100us   atrasar(3)                   //Medido 104 useg
#define Atrasar500us   atrasar(15)      //513 us
#define Atrasar1ms     atrasar(30)     //1,08 ms
#define Atrasar5ms     atrasar(150)       //5,07 ms
#define Atrasar15ms    atrasar(450)       //15,6 ms

//controle do ventilador
#define LigaVentilador P8OUT|=BIT2
#define DeslVentilador P8OUT&=~BIT2

// controle do aquecedor
#define LigaAquecedor P8OUT|=BIT1
#define DeslAquecedor P8OUT&=~BIT1


void pwmSetPotencia (int pote);
void pwmconfig (void);
void atrasar (unsigned long atrasa);
void adcConfig(void);
void adcConfigTimer(void);
void dmaConfig(void);
void ioConfig (void);
void i2cconfigSlave(unsigned char end);

// função com retorno 0 para ok e -1 para acabou a água
int ligaBombaAgua (volatile int tempoSegundos);



int tempSensor;   //sensor de temperatura
int lumSensor;    //sensor de luminosidade
int umiSensor;    //sensor de umidade

volatile int retorno = 0;
volatile unsigned char cmd =0;
volatile int bomba =0;

#endif /* PROJETOFINAL_H_ */


