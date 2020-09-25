#include <msp430.h> 

/**
 * Programa 11
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Descrição do Roteiro
 *
 * Escreva um programa em C que faça piscar o LED verde (P4.7) em exatamente
 * 1Hz (0,5s apagado e 0,5s aceso). Use a técnica de amostragem da flag de
 * overflow (TAIFG) do timer para saber quando o timer atingiu o valor máximo
 *
 */


void config(unsigned int ccr0);
void main(void)
{
  /*
   * Para configurar o timerA usaremos a funcao que
   * ativa o led 4.7 e o timer A0 com o Clock SMCLK
   * em modo subida.
   * Mandamos o valor do ccr0
   */
config((32768/2));
while(1) {                          //loop infinito
       if((TA0CTL&BIT0)==1){        //verifica se chegou no ccr0
          P4OUT ^= BIT7;            //troca o valor do out pro led
          TA0CTL ^= BIT0;           //zera o ponto de
      }
   }
}

void config(unsigned volatile int ccr0){

    WDTCTL = WDTPW + WDTHOLD; //parando o bulldog

    //configurando led 4.7
    P4SEL &= ~BIT7;         //seleciona o pino 4.7
    P4DIR |= BIT7;          //coloca como out
    P4OUT &= ~BIT7;         //zera a saida

    //timer setup

    //TA0CCTL0 = CCIE       //ativa interrupcao
    TA0CTL = TASSEL_1 |     //seleciona o ACLK
             MC_1 |         //modo de contagem, subida
             ID_0 |         //sem divisao do timer
             TACLR ;        //limpa o Timer

    TA0CCR0 = ccr0;         //adiciona o valor de ccr0
}
