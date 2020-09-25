#include <msp430.h> 

/*
 * Programa 15

 *
 * Turma D
 *
 * Descrição do Roteiro
 *
 * Utilize a interface serial USCI-A0 do MSP430 para controlar os LEDs de outro microcontrolador.
 * A ligação entre dois microcontroladores deve ser feita conforme a figura 2. Configure essa
 * interface para se comunicar em 1200 de baud rate. Use as interrupções dos botões (portas
 * P2.1 e P1.1) para enviar dados e as interrupções da USCI para receber dados e controlar os
 * LEDs. A interface serial está roteada nos pinos P3.3 (TX) e P3.4 (RX). Não se esqueça de
 * configurar esses pinos para sua funcionalidade dedicada.
 *
 *
 */


void configUSCI(void);
void configPino(void);
void enviamensagem(volatile unsigned char bytemsg);

volatile unsigned int i = 0;

volatile unsigned char  estS1 = 0;
volatile unsigned char  estS2 = 0;


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    configUSCI();
    configPino();
    __enable_interrupt();

    while(1){

    };
}


void configUSCI(void){
    UCA0CTL1 = UCSSEL_1 |   //ACLK
    UCSWRST;                //UCSI em Reset
    UCA0CTL0 = UCSPB;       //2 Stops
    //UCA0STAT = UCLISTEN;  //Loopback <== <==

    // Baudrate 1.200
    UCA0BRW =  27;            //Divisor por 27
    UCA0MCTL = UCBRF_0 |      //aprox 0/16
               UCBRS_2 ;      //aprox 0/8
    UCA0MCTL &= ~UCOS16;      //sem Super amostragem

    //Habilitando inerrupções
    UCA0IE = UCRXIE; //Enable RX interrupt






}
void configPino(void){

    //Btão p2.1
    P2DIR &= ~BIT1; // IN
    P2REN |= BIT1; // HABILITA R
    P2OUT |= BIT1; // PULL-UP
    P2IES |= BIT1;
    P2IE |= BIT1;


    //Btão p1.1
    P1SEL &= ~BIT1; //i/o
    P1DIR &= ~BIT1; // IN
    P1REN |= BIT1; // HABILITA R
    P1OUT |= BIT1; // PULL-OUT
    P1IE |= BIT1; // PULL-OUT
    P1IES |= BIT1;

    //configurando as poras 3.3 e 3.4

    // Configurar portas do uart
    P3DIR |= BIT3; //P3.3 saída
    P3OUT |= BIT3; //P3.3 = 1
    P3SEL |= BIT3; //Tx serial
    UCA0CTL1 &= ~ UCSWRST; //UCSI sai de Reset
    P3DIR &= ~BIT4; //p3.4 in
    P3SEL |= BIT4;  //rx serial

    //Leds
    P1OUT &= ~BIT0;             // Apagando o LED1
    P4OUT &= ~BIT7;             // Apagando o LED2
    P1DIR |= BIT0;              // Coloca o Led 1 como saída
    P4DIR |= BIT7;              // Coloca o Led 2 como saida
    UCA0IE=3;

}


#pragma vector=PORT1_VECTOR
__interrupt void PORT1_VECTOR_ISR (void) {
    //sai do modo low power
    //P1IE &= ~BIT1;
   int k;
   k = P1IV;
   if(k==0x04){
       if(estS2==0){
                 enviamensagem(0x10);
                 estS2++;
             }else{
                 enviamensagem(0x0A);
                 estS2=0;
             }
      }
   i = 0;
   for(;i<30000;i++);
   P1IFG &= ~BIT1;
   //entre modo low power
   }


#pragma vector=PORT2_VECTOR
__interrupt void PORT2_VECTOR_ISR (void) {
    //sai do modo low power
    //P2IE &= ~BIT1;
   int k;
   k = __even_in_range(P2IV,0x10);

   if(k==0x04){
      if(estS1==0){
                     enviamensagem(0x01);
                     estS1++;
                 }else{
                     enviamensagem(0xA0);
                     estS1=0;
                 }
   }
   //entre modo low power
   i = 0;
   for(;i<30000;i++);
   P2IFG &= ~BIT1;
}


#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_VECTOR_ISR (void){

    if(UCA0RXBUF == 0x01 || UCA0RXBUF == 0xA0) //Toggle red LED if "A" RXed
    {
    P1OUT ^= BIT0;
    }

    if(UCA0RXBUF == 0x10 || UCA0RXBUF == 0xA) //Toggle green LED if "B" RXed
    {
    P4OUT ^= BIT7;
    }
    UCA0IFG &= ~UCTXIFG;



}

void enviamensagem(volatile unsigned char bytemsg){
    UCA0TXBUF = bytemsg;
    while ( (UCA0IFG & UCTXIFG) == 0);
    UCA0IFG &= ~UCTXIFG;
}
