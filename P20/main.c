#include <msp430.h> 


/**
 * Programa 20
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Neste programa, iremos ler simultaneamente (ou quase) o valor da posição de dois eixos do
 * potenciômetro. Use a informação da posição dos dois eixos para controlar a luminosidade dos
 * dois LEDs da launchpad. Se a posição do joystick estiver no centro, os dois LEDs devem estar
 * em 50% da luminosidade.
 * Use o eixo X para controlar a luminosidade do LED vermelho e o eixo Y para controlar a
 * luminosidade do LED verde. Trabalhe com um sinal PWM a 100Hz com 256 passos (de 0 a 255).
 * Ajuste a resolução do conversor para 8-bits para ficar na mesma resolução da PWM. Amostre a
 * posição do potenciômetro a cada 20ms (50Hz).
 */

#include <msp430.h>

void pin_config(void);
void clock_config(void);
void timer_config(void);
void adc_config(void);
int values[500];

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // A amostragem deve ser feita a cada 10ms e uma estrutura de dados deve
    // armazenar os dados dos últimos 5 segundos de amostragem.
    // Então, em 5000ms haverá 500 leituras. É necessário uma estrutura com
    // 500 posições. Utilizaremos um array e um ponteiro para percorrê-lo.
    int * pointer = values;

    // Para um resistor de 2,5kOhm como resistor de entrada externa do conversor AD,
    // o tempo de amostragem deve ser:
    // tsample > (2,5k + 1,8k) x ln(2^13).25p + 800ns
    // tsample > 1,77us

    pin_config();
    clock_config();
    timer_config();
    adc_config();

    int volatile i = 0;

    while(1) {
        if{ ( (ADC12IFG & BIT0) == 0);
        i++;
        *pointer = ADC12MEM0;
        pointer++;

        if (pointer == values + 499) {
            pointer = values;
        }}

        if(cont <= pot)               //se a contagem está abaixo ou igual a poência led on
                  P4OUT |= BIT7;            //acende led
              else                          //se não, apaga o led
                  P4OUT &= ~BIT7;           //apaga led

    }

}

void pin_config(void) {

    // Entrada anaólogica A0 (P6.1) com pull-up (DIR = 0, REN = 1, OUT = 1)
    P6SEL |= BIT1;                             // Seleciona P6.1 com função I/O.
    //P6DIR &= ~BIT1;                             // P1.1, que é a chave S2, será entrada.
    //P6REN |= BIT1;                              // Pull-up/down habilitado.
    //P6OUT |= BIT1;                              // Pull-up escolhido.

    // Timers
    P5SEL |= (BIT2 | BIT3);                     // Port select XT2.
    P5SEL |= (BIT4 | BIT5);                     // Port select XT1.

    P4SEL &= ~BIT7;         //seleciona o pino 4.7
        P4DIR |= BIT7;          //coloca como out
        P4OUT &= ~BIT7;         //zera a saida

    P1SEL &= ~BIT0;         //seleciona o pino 1.0
    P1DIR |= BIT0;          //coloca como out
    P1OUT &= ~BIT0;         //zera a saida
}
//----------------------------------------------------------------------------------------

void clock_config(void) {
    UCSCTL6 &= ~(XT1OFF | XT1DRIVE_0 | XT1BYPASS);
    UCSCTL6 &= ~(XT2OFF | XT2DRIVE_0 | XT2BYPASS);

    UCSCTL6 &= ~(SMCLKOFF | XTS);               // SMCLK ligado e modo de baixa frequência
    UCSCTL6 = XCAP_3;                           // Internal load cap

    // Loop until XT1 fault flag is cleared
    do {
        UCSCTL7 = 0;                            // Clear XT1 fault flags
    } while (UCSCTL7);                          // Test XT1 fault flag

    UCSCTL8 = (SMCLKREQEN | MCLKREQEN | ACLKREQEN);

    UCSCTL4 = (SELA_0 | SELS_5);                // ACLK = XT1CLK = 32768Hz
                                                // SMCLK = XT2CLK = 4Mhz

    UCSCTL5 = DIVS_2;                           // SMCLK = (SMCLK)/4 = 1Mhz

    do {                                        // Limpa as flags de falta XT2, XT1 e DCO
      UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);

      SFRIFG1 &= ~OFIFG;                        // Limpa a flag de falta
     } while (SFRIFG1 & OFIFG);                 // Testa se a flag de falta estabilizou

}
//----------------------------------------------------------------------------------------

void timer_config(void) {

    // SMCLK = 1 MHz: 1 048 576 <-> 1s = 1000ms
    //                        TA0CCR0  <-> 0.01s = 10ms
    //
    //                        TA0CCR0 = ~10000
    //                        TA0CCR1 = 1,77us = 0.00177ms = 2;

    TA0CTL = (TASSEL__SMCLK |                   // SMCLK
             ID_0 |                             // Dividir por 1
             MC__UP);                           // Modo UP
    TA0CCR0 = 10000;                            // Período de 10 ms
    TA0CCTL1 = OUTMOD_6;
    TA0CCR1 = 5000;

}
//----------------------------------------------------------------------------------------

void adc_config(void) {
    ADC12CTL0 &= ~ADC12ENC;         // Desabilitar para configurar

    ADC12CTL0 = ADC12SHT0_3 |       // ADC tempo amostragem ADCMEM[0-7]
                ADC12ON;            // Ligar ADC

    ADC12CTL1 = ADC12CSTARTADD_0 |  // Armazenar na Posição 0
                ADC12SHS_1       |  // Usar TA0.1 (Selecionar (ADC12SC bit))
                ADC12SHP         |  // S/H usar timer
                ADC12DIV_0       |  // Divisor = 1
                ADC12SSEL_3      |  // SMCLK
                ADC12CONSEQ_2;      // Modo Único Canal

    ADC12CTL2  = ADC12TCOFF |       // Desligar sensor temperatura
                 ADC12RES_2;        // Resolução 12-bit

    ADC12MCTL0 = ADC12EOS    |      // Fim
                 ADC12SREF_0 |      // VR+ = AVCC e VR- = AVSS
                 ADC12INCH_1;       // A1 = Canal 1

    ADC12CTL0 |= ADC12ENC;          // Habilitar ADC12
}
