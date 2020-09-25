#include <msp430f5529.h>
int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //configurando led 4.7
     P4SEL &= ~BIT7;         //seleciona o pino 4.7
     P4DIR |= BIT7;          //coloca como out
     P4OUT &= ~BIT7;         //zera a saida

    P7DIR |= BIT5 + BIT4;
    P7SEL |= BIT5 + BIT4 ;
    P7DS |=  BIT5 + BIT4;
    P5DIR |= BIT6 + BIT7;
    P5SEL |= BIT6 + BIT7 ;
    P5DS |=  BIT6 + BIT7 ;

    TB0CTL = TBCLGRP_0 + CNTL__12 + TBSSEL__SMCLK + MC__CONTINUOUS + ID__8 ;// 12 Bits, SMCLK, Continuous Counter, /8
    TB0EX0 = TBIDEX__8; // Divide by 8
    TB0CCR0 = 0xd00 ;
    TB0CCR1 = 0xD01;
    TB0CCR2 = 0x800;
    TB0CCR3 = 0xA00;
    TB0CCTL0 = CLLD_0 + OUTMOD_7 ;
    TB0CCTL1 = CLLD_0 + OUTMOD_7 ;
    TB0CCTL2 = CLLD_0 + OUTMOD_7 ;
    TB0CCTL3 = CLLD_0 + OUTMOD_7 ;

    while ( 1 ) ;

}
