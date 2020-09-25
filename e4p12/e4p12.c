#include <msp430.h> 



void config(unsigned int ccr0);
/*
 *  Usando as interrupções do timer, escreva um programa em C que faça o LED
 *  pulsar (e não piscar) em 0,5Hz (1s acendendo e 1s apagando). Para isso, gere, por software, um
 *  sinal modulado em largura de pulso (PWM). A frequência do sinal PWM deve ser de 50Hz.
 *  Para atingir a frequência de 50Hz precisamos achar
 *  1000000Hz divide por 8 fica 125kHz.
 *  Para obter 50Hz basta agora usar o CCR0 como 2500.
 */
void main(void)
{
  /*
   * Para configurar o timerA usaremos a funcao que
   * ativa o led 4.7 e o timer A0 com o Clock SMCLK
   * em modo subida.
   * Mandamos o valor do ccr0
   */
  config(2500);
  unsigned char subida = 1;
  unsigned char pot = 0;
  unsigned char max = 100;
  unsigned char cont = 1;
  while(1) {
      if(subida){
      if(cont <= pot){
          P4OUT |= BIT7;         //acende led
      }else{
          P4OUT &= ~BIT7;         //apaga led
      }
      if(cont>max){
          cont=0;
      }
      if((TA0CTL&BIT0)==1){
                pot++;
                TA0CTL ^= BIT0;
                if(pot>max){
                    pot = 0;
                    subida = 0;
                }
            }

      cont++;
      }else{
          if(cont <= pot){
                    P4OUT &= ~BIT7;         //acende led
                }else{
                    P4OUT |= BIT7;         //apaga led
                }
                if(cont>max){
                    cont=0;
                }
                if((TA0CTL&BIT0)==1){
                          pot++;
                          TA0CTL ^= BIT0;
                          if(pot>max){
                              pot = 0;
                              subida = 1;
                          }
                      }

                cont++;
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
    TA0CTL = TASSEL_2 |     //seleciona o SMCLK
             MC_1 |         //modo de contagem, subida
             ID_3 |         //sem divisao do timer
             TACLR ;        //limpa o Timer

    TA0CCR0 = ccr0;         //adiciona o valor de ccr0
}

