/*
 * ioConfig.c
 *
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 *  Projeto final
 *
 *  configuração das portas digitais que serão usadas
 *
 *  4 portas - 3 saídas e 1 entrada
 *
 *  saídas 0 off - 1 on
 *  p3.7 - relé bomba de água
 *  p8.2 - relé ventilador
 *  p8.1 - relé aquecedor
 *
 *  entradas
 *  p2.3 - sensor de tanque vazio
 *  com curto circuito para vazio e circuito aberto para cheio
 *  pull-up deixará 1 quando cheio e 0 quando vazio
 *
 */

#include<projetofinal.h>

void ioConfig (void){

    P3SEL &= ~BIT7;         //i/o
    P8SEL &= ~(BIT1+BIT2);  //i/o
    P2SEL &= ~BIT3;

    P3DIR |= BIT7;         //out
    P8DIR |= (BIT1+BIT2);  //out
    P2DIR &= ~BIT3;        //in

    P3OUT &= ~BIT7;         //out =0
    P8OUT &= ~(BIT1+BIT2);  //out =0

    P2REN |= BIT3;          //resistor ativo
    P2OUT |= BIT3;          //pull-up

}

int ligaBombaAgua (volatile int tempoSegundos){
    if((P2IN&BIT3) != BIT3)                        //checa se tem água no reservatório
                return 1;

    volatile int i = tempoSegundos * 2;         //dobra o tempo para ler a cada meio segundo

    P3OUT |= BIT7;                              //liga bomba

    for(;(i>0)&&(retorno);i--){
        atrasar(15000);                         //meio segundo aproximadamente
        if((P2IN&BIT3) != BIT3)                    //checa se tem água no reservatório
            return 1;
    }

    P3OUT &= ~BIT7;                             //out =0
    return 0;

}


