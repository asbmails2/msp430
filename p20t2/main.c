#include <msp430.h> 
#include "clock.h"
#include "pmm.h"

void config_adc();
void config_timerA();
void config_timerB();
void config_dma();

/**
 * main.c
 */
int dadox,dadoy;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    pmmVCore(1);
    pmmVCore(2);
    clockInit();
    config_adc();
    config_timerA();
    config_timerB();
    config_dma();
    while(1){

    while ( (ADC12IFG&ADC12IFG0) == 0); //Espera conversão
    TB0CCR1 = 39*dadox;                 //Muda PWM (10000/256 passos = 39)

    while ( (ADC12IFG&ADC12IFG1) == 0); //Esperar conversão
    TB0CCR2 = 39*dadoy;                 //Muda PWM

    }
}

void config_dma(void){
    DMACTL0 = DMA0TSEL_24 | DMA1TSEL_24; //ADC
    DMA0CTL = DMAEN | DMADT_4; //Habilita DMA0 no modo Repetido
    DMA0SA = (__SFR_FARPTR)0x0720; //Pega o dado da MEM0
    DMA0DA = &dadox; //Endereço destino
    DMA0SZ = 1; //Apenas 1 transferência
    DMA1CTL = DMAEN | DMADT_4; //Habilita DMA1 no modo Repetido
    DMA1SA = (__SFR_FARPTR)0x0722; //Pega o dado da MEM1
    DMA1DA = &dadoy; //Endereço destino
    DMA1SZ = 1; //Apenas 1 transferência
}

void config_timerA(void){
    TA0CTL = TAIDEX_0;
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;    //SMCLK = 1MHZ
    TA0CCR0 = 10000;         //Tempo de 20ms
    TA0CCTL1 = OUTMOD_6;
    TA0CCR1 = 5000;
}

void config_timerB(void){
    P4SEL |= BIT3;              //Ativa o pino 4.3
    P4DIR |= BIT3;
    P4SEL |= BIT7;              //Ativa o pino 4.7
    P4DIR |= BIT7;
    PMAPKEYID = 0x02D52;
    P4MAP3 = PM_TB0CCR1A;
    P4MAP7 = PM_TB0CCR2A;
    TB0EX0 = TBIDEX_0;          //Não reduz a frequência
    TB0CTL = TBSSEL_2|ID_0|MC_1|TBCLR;      //SMCLK = 1MHz
    TB0CCTL1 = OUTMOD_6;
    TB0CCTL2 = OUTMOD_6;
    TB0CCR0 = 10000;          //Freq do PWM = 100Hz
    TB0CTL &= ~TBCLR;
}

void config_adc(void){
    ADC12CTL0 &= ~ADC12ENC; //Desabilitar para configurar

    ADC12CTL0 = ADC12SHT0_3 | //ADC tempo amostragem ADCMEM[0-7]
                ADC12ON; //Ligar ADC

    ADC12CTL1 = ADC12CSTARTADD_0 | //Resultado em ADC12MEM0
                ADC12SHS_1 | //Selecionar BIT TA0.1
                ADC12SHP | // S/H usar timer
                ADC12DIV_0 |
                ADC12SSEL_3 | //Clock ADC = SMCLK
                ADC12CONSEQ_3; //Repete sequência de canais (A0 E A1)

    ADC12CTL2 = ADC12TCOFF | // Desligar sensor temperatura
            ADC12RES_0; // Resolução 8-bit

    ADC12MCTL0 = ADC12SREF_0 | //VR+ = AVCC e VR- = AVSS        EIXO X
            ADC12INCH_0; //P6.0

    ADC12MCTL1 = ADC12EOS | //Fim de sequência                  EIXO Y
            ADC12SREF_0 | //VR+ = AVCC e VR- = AVSS
            ADC12INCH_1; //P6.1

    ADC12CTL0 |= ADC12ENC; //Habilitar ADC12
}
