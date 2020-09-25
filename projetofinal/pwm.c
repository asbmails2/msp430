/**
 * Pwm
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 *  Projeto final
 *
 *  PWM com saída no pino 4.3
 */

        // 100% = 10500
        // 1 passo = 1% =  105

#include <projetofinal.h>

void pwmconfig (){
        P4SEL |= BIT3;              //Ativa o pino 4.3
        P4DIR |= BIT3;              //coloca como saída
        PMAPKEYID = 0x02D52;        //chave apra editar mapa
        P4MAP3 = PM_TB0CCR1A;       //p 4.3 como saída do timer b
        TB0CTL = TBSSEL_2 |                   //seleciona o SMCLK
                   ID_1 |                   //Divide o clock por 2
                   MC_1 |                   //modo de subida simples até chegar o clock
                  TBCLR;                    //zerar a tag de subida.
        TB0CCTL1 = (OUTMOD_7);                //ativando a saída da PWM do clock B pela porta 4.7
                                            //de acordo com o flaco de subida do ccr1 para ligar o led
                                            //e desligar o led quando for o flanco de descida do ccr1
        TB0CTL &= ~TBCLR;                     //zera a trava do controle do timer B
        TB0CCR0 = 2100;                      //coloca o ccr0 do timer b como 10500
        TB0CCR1 =  0;                    //varia o ccr1 que também varia o TB0CTL                                     //isso limita a cotagem do timer b até 10500.

}

void pwmSetPotencia (int pote){
        TB0CCR1 = 21 * pote;
}


