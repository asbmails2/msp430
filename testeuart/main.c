
#include <msp430.h>
/*
 * main.c
 */
void config_serial(void);
void main (void){
int letra = 'A';
config_serial();
    while (1){
        UCA0TXBUF = letra++;
        if (letra == 'Z'+1) letra='A';
        while ( (UCA0IFG & UCTXIFG) == 0);
        UCA0IFG &= ~UCTXIFG;
    }
}
void config_serial(void){
UCA0CTL1 = UCSSEL_2 |
UCSWRST;
UCA0CTL0 = UCSPB;
//UCA0STAT = UCLISTEN; //SMCLK
//UCSI em Reset
//2 Stops
//Loopback <== <==
// Baudrate 19.200
UCA0BRW = 3;
UCA0MCTL = UCBRF_4 |
UCBRS_0 |
UCOS16; //Divisor por 3
//aprox 4/16
//aprox 0/8
//Super amostragem
// Configurar portas
P3DIR |= BIT3; //P3.3 saída
P3OUT |= BIT3; //P3.3 = 1
P3SEL |= BIT3; //Tx serial
UCA0CTL1 &= ~ UCSWRST; //UCSI sai de Reset
}
