#include <msp430.h> 


/**
 * Programa 19
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Configure o módulo do conversor A/D para amostrar a tensão de entrada que representa apenas
 * um eixo da posição do joystick (ou de um potenciômetro qualquer). Não se esqueça de ajustar
 * os limites superior e inferior da tensão de referência para maximizar a precisão da leitura.
 * Configure um timer para realizar essa amostragem a cada 10ms. Grave num vetor de memória
 * os últimos 5 segundos de amostragem. Você pode plotar o resultado da leitura no próprio CCS
 * colocando o vetor na aba de expressões e clicando com o botão direito, selecione a opção
 * “Graph”.
 */


#include "clock.h"
#include "pmm.h"
void configTimer();
void configAdc();

/**
 * main.c
 */

float x;
int valores[500];
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    configTimer();
    configAdc();
    int volatile i=0;
    int
    * end = valores;
    while(1){
        while ( (ADC12IFG & BIT0) == 0){} //Espera conversão
        i++;
        *end = ADC12MEM0;                //Atualiza a PWM (3,3/256 passos ~= 0,01289)
        end++;
        if(end >= valores + 499){
          end = valores;
        }
    }
}

void configTimer(void){
    TA0CTL = TAIDEX_0;                          //timer a para o adc
    TA0CTL = TASSEL_2 | ID_1 | MC_1 | TACLR;    //SMCLK = 1MHZ
                                                //divide o clk por 2
                                                //modo de contagem de subida
                                                //limpa o  clk
    TA0CCR0 = 5000;                             // 20ms
    TA0CCTL1 = OUTMOD_6;                        //pwm
    TA0CCR1 = 2500;

}

void configAdc(void){
    ADC12CTL0 &= ~ADC12ENC;         //desliga o ADC
    ADC12CTL0 = ADC12SHT0_3 |       //taxa de amostragem
                ADC12ON;            //liga ADC
    ADC12CTL1 = ADC12CSTARTADD_0 |  //Salva o resultado em ADC12MEM0
                ADC12SHS_1 |        //Selecionar BIT TA0.1
                ADC12SHP |          // S/H usar timer
                ADC12DIV_0 |
                ADC12SSEL_3 |       //Clock SMCLK
                ADC12CONSEQ_2;      //Canais A0
    ADC12CTL2 = ADC12TCOFF |        //desativa o sensor de temperatura
                ADC12RES_0;         //8-bit
    ADC12MCTL0 = ADC12SREF_0 |      //3.3V a 0V
                 ADC12INCH_0;      //P6.0       //lê até aqui
    ADC12CTL0 |= ADC12ENC;          //Ativa adc
}
