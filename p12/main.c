#include <msp430.h> 

/**
 * Programa 12
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Descrição do Roteiro
 */

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
  unsigned char subida = 1;         //flag para mostrar se o led esta aumentando a luminosidade 1
                                    //Ou 0 para quando estiver diminuindo
  unsigned char pot = 0;            //variável para salvar a potência que está subindo o led.
  unsigned const char max = 50;     //constante com o valor máximo da potência
  unsigned char cont = 1;           //contador usado apra controlar o a quantidade de vezes que o led
                                    //precisa acender e apagar durante um ciclo de 50 contágens por
                                    //cada vez que sobe o valor.
  while(1) {
      if(subida){                   //se está subindo
      if(cont <= pot)               //se a contagem está abaixo ou igual a poência led on
          P4OUT |= BIT7;            //acende led
      else                          //se não, apaga o led
          P4OUT &= ~BIT7;           //apaga led

      if(cont>max)                  //se a contagem chegar ao máximo manda zerar o cont para reiniciar o ciclo
          cont=0;

      if((TA0CTL&BIT0)==1){         //Se chegou a contagem em 2500
                pot++;              //Deu um ciclo dos 50hz e adiciona uma potência
                TA0CTL ^= BIT0;     //zera o controle
                if(pot>max){        //se a potência chegou passa do máximo, zerar pot e a flag de subida para iniciar o else com o
                                    //protocolo de descida do algoritmo, o mesmo método da subida só invertido quando acende e des
                                    //liga o led.
                    pot = 0;
                    subida = 0;
                }
            }

      cont++;                       //soma o contador.
      }else{                        //se descendo

          if(cont <= pot)           //se pot menor que conte
                P4OUT &= ~BIT7;     //apaga led
          else
                P4OUT |= BIT7;      //acende led

          if(cont>max)              //se contagem do ciclo passou de max
                cont=0;

          if((TA0CTL&BIT0)==1){     //se chegou ao 2500 (1 ciclo de 50hz foi dado
                pot++;              //aumenta a potência
                TA0CTL ^= BIT0;     //zera a flag de controle
                if(pot>max){        //se a pot passar do máximo, zera a pot e põe para subir a
                    pot = 0;
                    subida = 1;
                }
           }
           cont++;                  //contador de ciclo.
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
    TA0CTL = TASSEL_2 |     //seleciona o SMCLK de 1 MHz
             MC_1 |         //modo de contagem, subida
             ID_3 |         //divisao por 8 do timer
             TACLR ;        //limpa o Timer

    TA0CCR0 = ccr0;         //adiciona o valor de ccr0
}

