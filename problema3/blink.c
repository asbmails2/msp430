#include <msp430.h>				


void configTimers();
void configAdc();
void configDma();

int x,y;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    configTimers();
    configAdc();
    configDma();
    while(1){
        while ( (ADC12IFG&ADC12IFG0) == 0){} //Espera conversão
        TB0CCR1 = 19*x;                      //Atualiza a PWM (5000/256 passos = 19)
        while ( (ADC12IFG&ADC12IFG1) == 0){} //Esperar conversão
        TB0CCR2 = 19*y;                      //atualiza a PWM
    }
}

void configTimers(void){
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
                ADC12CONSEQ_3;      //Canais A0 e A1
    ADC12CTL2 = ADC12TCOFF |        //desativa o sensor de temperatura
                ADC12RES_0;         //8-bit
    ADC12MCTL0 = ADC12SREF_0 |      //3.3V a 0V
                 ADC12INCH_0;       //P6.0
    ADC12MCTL1 = ADC12EOS |         //lê até aqui
            ADC12SREF_0 |           //3.3V a 0V
            ADC12INCH_1;            //P6.1
    ADC12CTL0 |= ADC12ENC;          //Ativa adc
}

void config_dma(void){

    DMACTL0 = DMA0TSEL_24 | DMA1TSEL_24; //ADC
    DMA0CTL = DMAEN | DMADT_4; //Habilita DMA0 no modo Repetido
    DMA0SA = (__SFR_FARPTR)0x0720; //Pega o dado da MEM0
    DMA0DA = &x; //Endereço destino
    DMA0SZ = 1; //Apenas 1 transferência
    DMA1CTL = DMAEN | DMADT_4; //Habilita DMA1 no modo Repetido
    DMA1SA = (__SFR_FARPTR)0x0722; //Pega o dado da MEM1
    DMA1DA = &y; //Endereço destino
    DMA1SZ = 1; //Apenas 1 transferência
}
