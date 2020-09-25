/**
 * adc.c
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 *  Projeto final
 *
 *  tratamento das entradas analógicas
 *  3 entradas necessárias
 *  Luminosidade = p6.0
 *  temperatura = p6.1
 *  umidade do solo = p6.2
 *
 *  Como os valores são para uma atualização simples
 *  o tempo não precisa ser tão pequeno.
 *  Tomando o fato que a impedância de entrada é diferente em cada entrada;
 *
 *  Luminosidade = pull-up externo de 1 kohm, comportamento 1Mohm no escuro
 *   entre 10 e 20Kohm em 10 lux e indo para quase 0 por ser uma corrente baixa.
 *   Logo varia de 3.3V (escuro para quase 0V muito claro
 *
 *  temperatura = o modo de ligação em modo básico básico é vcc em 3.3V , gnd em 0v
 *  e a saída ligada direto na entrada do adc. Contará de 2ºC a 150ºC variando em 10mV/ºC
 *  então vai de 0.02V até 1,50 V então temos uma aproximação de
 *  255/148 o que nos dá um valor aproximado da temperatura
 *  igual a tempSensor*0,578
 *
 *
 *  umidade do solo = trabalha com o indice de umidade de 3,3 V para seco e 0v para umido
 *
 *  luminosidade vai de 0 a 255, Verificando expirimentalmente, uma luz num valor considerável
 *
 */

#include <projetofinal.h>


void adcConfig(void){

    P6SEL |= (BIT0 + BIT1 );

    adcConfigTimer();
    ADC12CTL0 &= ~ADC12ENC;         //desliga o ADC
    ADC12CTL0 = ADC12SHT0_3 |       //taxa de amostragem
                ADC12ON;            //liga ADC
    ADC12CTL1 = ADC12CSTARTADD_0 |  //Salva o resultado em ADC12MEM0
                ADC12SHS_1 |        //Selecionar BIT TA0.1
                ADC12SHP |          // S/H usar timer
                ADC12DIV_0 |
                ADC12SSEL_3 |       //Clock SMCLK
                ADC12CONSEQ_3;      //Canais A0, A1 e A2
    ADC12CTL2 = ADC12TCOFF |        //desativa o sensor de temperatura
                ADC12RES_0;         //8-bit
    ADC12MCTL0 = ADC12SREF_0 |      //3.3V a 0V
                 ADC12INCH_0;       //P6.0
    ADC12MCTL0 = ADC12SREF_1 |      //1,5V a 0V |     //P6.1
                 ADC12INCH_1;
    ADC12CTL0 |= ADC12ENC;          //Ativa adc
    dmaConfig();
}

void adcConfigTimer(void){

    TA0CTL = TAIDEX_0;                          //timer a para o adc
    TA0CTL = TASSEL_2 | ID_1 | MC_1 | TACLR;    //SMCLK = 1MHZ
                                                //divide o clk por 2
                                                //modo de contagem de subida
                                                //limpa o  clk
    TA0CCR0 = 5000;                             // 20ms
    TA0CCTL1 = OUTMOD_6;                        //pwm
    TA0CCR1 = 2500;

}

void dmaConfig(void){
    DMACTL0 = DMA0TSEL_24 | DMA1TSEL_24; //ADC
    DMA0CTL = DMAEN | DMADT_4; //Habilita DMA0 no modo Repetido
    DMA0SA = (__SFR_FARPTR)0x0720; //Pega o dado da MEM0
    DMA0DA = &lumSensor; //Enders destino
    DMA0SZ = 1; //Apenas 1 transferência
    DMA1CTL = DMAEN | DMADT_4; //Habilita DMA1 no modo Repetido
    DMA1SA = (__SFR_FARPTR)0x0722; //Pega o dado da MEM1
    DMA1DA = &tempSensor; //Endereço destino
    DMA1SZ = 1; //Apenas 1 transferência
}
