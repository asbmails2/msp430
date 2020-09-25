#include <msp430.h>

/*
 * Programa 16
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
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
#define Atrasar100us   atrasar(3)                   //Medido 104 useg

void configTimer(void);
void configPino(void);
void enviamensagem(unsigned char bytemsg);
unsigned char recebemensagem(void);
void acenderLeds (unsigned char bytemsg);
void startTimer(void);
void stopTimer(void);
void atrasar (unsigned long atrasa);

volatile unsigned int i = 0;
volatile unsigned char  estS1 = 0;
volatile unsigned char  estS2 = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    configPino();
    __enable_interrupt();

    while(1){
        if((P3IN & BIT4) != 0){
            acenderLeds(recebemensagem());
        }
    }
}


void configPino(void){

    //Btão p2.1
      P2DIR &= ~BIT1; // IN
      P2REN |= BIT1; // HABILITA R
      P2OUT |= BIT1; // PULL-UP
      P2IE |= BIT1;
      P2IFG &= ~BIT1;


      //Btão p1.1
      P1SEL &= ~BIT1; //i/o
      P1DIR &= ~BIT1; // IN
      P1REN |= BIT1; // HABILITA R
      P1OUT |= BIT1; // PULL-OUT
      P1IE |= BIT1; // PULL-OUT
      P1IFG &= ~BIT1;
    //configurando as poras 3.3 e 3.4


    // Configurar portas de entrada e saída

    P3DIR |= BIT3; //P3.3 saída
    P3OUT &= ~BIT3; //P3.3 = 0
    P3SEL &= ~BIT3; //I/O

    P3DIR &= ~BIT4; //p3.4 in
    P3SEL &= ~BIT4;  //i/o
    P3REN |= BIT4; //habilita R
    P3OUT &= ~BIT4; //pull-down

    //Leds
    P1OUT &= ~BIT0;             // Apagando o LED1
    P4OUT &= ~BIT7;             // Apagando o LED2
    P1DIR |= BIT0;              // Coloca o Led 1 como saída
    P4DIR |= BIT7;              // Coloca o Led 2 como saida
}

void configTimer(void){
   /*
    * Para configura o clock em 1200 baund usamos
    * SMCLK = 1MHz contudo será desligado até o start
    *
    * com o ccr0 = 104 , o TAIFG vai para 1201,92 Hz
    * Valor próximo ao valor desejado de 1200.
    */
   TA0CTL = TASSEL_0 |     //deixa o clock parado
             MC_1 |         //modo de contagem, subida
             ID_3 |         // divisao do timer por 8
             TACLR ;        //limpa o Timer

    TA0CCR0 = 104;         //adiciona o valor de ccr0
}

void startTimer(void){
    TA0CTL = TASSEL_2 |     //deixa o clock parado
                TACLR ;        //limpa o Timer
}

void stopTimer(void){
    TA0CTL = TASSEL_0 |     //deixa o clock parado
                TACLR ;        //limpa o Timer
}

/*
 * Nessa função será enviado uma mensagem
 * para o início é dado um pulso alto por 3 batidas do baund
 * e quando der uma baixa no próximo baund será dados
 * conta 8 bits e zera a saída.
 */
void enviamensagem(unsigned char bytemsg){

    P3OUT |= BIT3;  //1 --
    Atrasar100us;
    P3OUT ^= BIT3;  //0 --
    Atrasar100us;
    P3OUT ^= BIT3;  //1 --
    //agora começa o envio
    for(i=0;i<8;i++){
            Atrasar100us;
            if((bytemsg&BIT7)!=0){
                P3OUT |= BIT3;
            }else{
                P3OUT &= ~BIT3;
            }

            bytemsg = bytemsg << 1;
      }
}

unsigned char recebemensagem (void){
    P1IE &= ~BIT1;
    P2IE &= ~BIT1;
    unsigned char recebeu = 0;  //variável para guardar a mensagem recebida
    unsigned char bit = 0;
    while((P3IN&BIT4) != 0){}  //espera zerar a entrada
    Atrasar100us;
    while((P3IN&BIT4) == 0);    //enquanto é zero a entrada

    for(i=0;i<8;i++){
        recebeu = recebeu << 1;
        Atrasar100us;
        bit = ((P3IN&BIT4)>>4);
        recebeu |= bit;

    }
    P1IE |= BIT1;
    P2IE |= BIT1;
    return recebeu;
}

void acenderLeds (unsigned char bytemsg){
    if(bytemsg == 0x01 || bytemsg == 0xA0)
      P1OUT ^= BIT0;
    if(bytemsg == 0x10 || bytemsg == 0xA)
      P4OUT ^= BIT7;
}

void atrasar (unsigned long atrasa){
   volatile unsigned long i = atrasa;
   while(i != 0)
            i--;
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
       i = 0;
       for(;i<30000;i++);
      }

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
   for(;i<3;i++);
   P2IFG &= ~BIT1;
}
