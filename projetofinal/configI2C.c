/*
 * configI2C.c
 *
 *  Created on: 7 de dez de 2017
 *      Author: asb
 */

#include <projetofinal.h>

/*
 * Método para a configuração do timer com o
 *
 * */
void i2cconfigSlave(unsigned char end){
    P3REN |= (BIT0 + BIT1); //Enable resistor
    P3OUT |= (BIT0 + BIT1); //Pull-up
    /*
    P3SEL |= (BIT0 + BIT1); //seleciona em modo específico I2C dos pinos
     UCB0CTL1 |= UCSWRST;                      // Enable SW reset
     UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
     UCB0BRW = 10;                             // 100khz
     UCB0I2COA = end;                          // Endereço do Escravo
     UCB0CTL1 = UCSSEL_2;                      // Use SMCLK, keep SW reset
     UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
     UCB0IE |= UCRXIE;                         // Enable RX interrupt
     */
}



