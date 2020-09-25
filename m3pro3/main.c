#include <msp430.h>

void lcdi2cstart (void);
void lcdClean(void);
void lcdBacklightON(void);
void lcdBacklightOFF(void);
void lcdZerarRsWr(unsigned char enviar);
void lcdWritemensagem(unsigned char ascii, unsigned char config);
void lcdSendNibble(char n ,char config);
void lcdElow(char mensagem);
void lcdEhigh(char palavra);
void ucbConfigPinos(void);
void ucbConfig(unsigned char end);
int ucbEnviaMsg(unsigned char msg);
void atrasar (unsigned long atrasa);
void escy();
void escx();
void configTimers();
void configAdc();
void config_dma();


#define Atrasar100us   atrasar(3)                   //Medido 104 useg
#define Atrasar500us   atrasar(15)      //513 us
#define Atrasar1ms     atrasar(30)     //1,08 ms
#define Atrasar5ms     atrasar(150)       //5,07 ms
#define Atrasar15ms    atrasar(450)       //15,6 ms
#define Atrasar2ms     atrasar(50)     //1,08 ms


// Bits para controle do LCD
#define RS   BIT0
#define RW   BIT1
#define E    BIT2
#define BL   BIT3

//lcd config = 0x09 é para mandar a mesngem de lcd ligado
// e deixar o lcd em modo escrita

int x,y,mensagem,i,j,k,temp;

volatile int a;

unsigned char lastmensagem=0;                  //Úlitmo valor escrito na porta

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    configTimers();
    configAdc();
    config_dma();
    lcdi2cstart();

    lcdWritemensagem(0x5E,0x09);
            lcdWritemensagem(0x30,0x09);
            lcdWritemensagem(0x31,0x09);
            lcdWritemensagem(0x32,0x09);
            lcdWritemensagem(0x43,0x09);
            lcdWritemensagem(0x34,0x09);
            lcdWritemensagem(0x35,0x09);

    while (1){
        while ( (ADC12IFG&ADC12IFG0) == 0){} //Espera conversão
        while ( (ADC12IFG&ADC12IFG1) == 0){} //Esperar conversão

       /* escx();

        lcdWritemensagem(0xC0,0x08);     //pula linha se chegou no final dela

        escy();

        lcdWritemensagem(0x02,0x08);
        */atrasar(100);
    }
}


//configurando os pinos das interface serial
void ucbConfigPinos(void){
    P3REN |= (BIT0 + BIT1); //Enable resistor
    P3OUT |= (BIT0 + BIT1); //Pull-up
    P3SEL |= (BIT0 + BIT1); //seleciona em modo específico I2C dos pinos
}

/*
 * Método para a configuração do timer com o
 *
 * */
void ucbConfig(unsigned char end){

     UCB0CTL1 |= UCSWRST;                      // Enable SW reset
     UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
     UCB0BRW = 10;                             // 100khz
     UCB0I2CSA = end;                          // Endereço do Escravo
     UCB0CTL1 = UCSSEL_2;                      // Use SMCLK, keep SW reset

}

int ucbEnviaMsg(unsigned char msg){

      UCB0CTL1 |= UCTR    |                       //Mestre transmissor
                  UCTXSTT;                        //Gerar START e envia endereço
      while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar TXIFG (completar transm.)
      if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
         return -1;

      UCB0TXBUF = msg;                           //Dado a ser escrito

      while ( (UCB0IFG & UCTXIFG) == 0){          //Esperar Transmitir
         }
      UCB0CTL1 |= UCTXSTP;                        //Gerar STOP

      while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //Esperar STOP

   //   DELAY_15ms;                                  //atrasar p/ escravo perceber stop

      return 1;                                   //return 1 para ok
}

void atrasar (unsigned long atrasa){
   volatile unsigned long i = atrasa;
   while(i != 0)i--;
}


void lcdi2cstart (void){


    ucbConfigPinos();
    ucbConfig(0x3F);
    lcdZerarRsWr(0x00);         //Manter RS E RW = 0
    lcdBacklightON();       //Mostrador Ligado
    Atrasar15ms;          //Esperar mais de 15ms
    ucbEnviaMsg(0x38);        //Definir condições 0011
    lcdEhigh(0x38);       //Pulso de Enable
    Atrasar100us;
    lcdElow(0x38);
    Atrasar15ms;
    Atrasar15ms;
    ucbEnviaMsg(0x38);        //Definir condições 0011
    lcdEhigh(0x38);
    Atrasar100us;
    lcdElow(0x38);
    Atrasar1ms;
    ucbEnviaMsg(0x38);        //Definir condições 0011
    lcdEhigh(0x38);
    Atrasar100us;
    lcdElow(0x38);
    ucbEnviaMsg(0x28);        //4 Bits 0010
    lcdEhigh(0x28);
    Atrasar100us;
    lcdElow(0x28);
    lcdWritemensagem(0x2C,0x08); // 0010 NFxx: N e F ativados
    lcdWritemensagem(0x0F,0x08); //Mostrador ativado com cursor pulsante
    lcdWritemensagem(0x01,0x08); //Limpa
    lcdWritemensagem(0x06,0x08); //I = 0 S = 1
}

void lcdClean(void){
     lastmensagem=0;
     ucbEnviaMsg(lastmensagem);

}
void lcdBacklightON(void){
    lastmensagem= lastmensagem | BL;
    ucbEnviaMsg(lastmensagem);
}


void lcdBacklightOFF(void){
    lastmensagem= lastmensagem & ~BL;
    ucbEnviaMsg(lastmensagem);
}

void lcdZerarRsWr(unsigned char enviar){
    lastmensagem = ~(RS+RW) & enviar;
    ucbEnviaMsg(lastmensagem);
}

void lcdWritemensagem(unsigned char ascii, unsigned char config){

    int nibble1,nibble0;

    nibble1 = 0;
    nibble0 = 0;

    nibble0 = ascii & 0x0F;     //Define os Nibbles
    nibble1 = ascii & 0xF0;
    nibble1 = nibble1>>4;           //Desloca para que fique no menos significativo

    lcdSendNibble(nibble1,config);     //Escreve
    lcdSendNibble(nibble0,config);     //Escreve
    atrasar(50);                     //atrasar
}


void lcdSendNibble(char n ,char config){
        char mensagem=0x00;
        n = n<< 4;     //desloca o nible para a esquerda
        n &= 0xF0;     //zera o nibble menos significativo
        config &= 0x0F;        //Config = 0000XXXX (Bits de BL, RS, RW E EN)
        mensagem = config | n;      //Palavra = 1111XXXX
        ucbEnviaMsg(mensagem);     //Envia Nibble
        lcdEhigh(mensagem);
        Atrasar100us;;
        lcdElow(mensagem);
        Atrasar1ms;               //atrasar
        Atrasar1ms;
}

void lcdElow(char mensagem){
    mensagem &= ~E;       //tira o enable
    ucbEnviaMsg(mensagem);
}

void lcdEhigh(char palavra){
    palavra |= E;     //ativa o Enable
    ucbEnviaMsg(palavra);
}

void configTimers(void){
    TA0CTL = TAIDEX_0;
       TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;    //SMCLK = 1MHZ
       TA0CCR0 = 10000;         //Tempo de 20ms
       TA0CCTL1 = OUTMOD_6;
       TA0CCR1 = 5000;
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

void escx(){
               int on=0;
               int off=0;
                       if(x<=0x10){
                                   for(on=0; on<7;on++)
                                       lcdWritemensagem(0xFF,0x09);  //quadrado
                               }else if(x<=0x20){
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                                   for(on=0; on<6;on++)
                                       lcdWritemensagem(0xFF,0x09);  //quadrado

                               }else if(x<=0x30){
                                   for(off=0; off<2;off++)
                                       lcdWritemensagem(0x10,0x09);  //quadrado
                                   for(on=0; on<5;on++)
                                       lcdWritemensagem(0xFF,0x09);  //quadrado

                               }else if(x<=0x40){
                                   for(off=0; off<3;off++)
                                       lcdWritemensagem(0x10,0x09);  //quadrado
                                   for(on=0; on<4;on++)
                                       lcdWritemensagem(0xFF,0x09);  //quadrado

                               }else if(x<=0x50){
                                   for(off=0; off<4;off++)
                                       lcdWritemensagem(0x10,0x09);  //quadrado
                                   for(on=0; on<3;on++)
                                       lcdWritemensagem(0xFF,0x09);  //quadrado
                               }else if(x<=0x60){
                                   for(off=0; off<5;off++)
                                       lcdWritemensagem(0x10,0x09);  //quadrado
                                   for(on=0; on<2;on++)
                                       lcdWritemensagem(0xFF,0x09);  //quadrado
                               }else if(x<=0x70){
                                   for(off=0; off<6;off++)
                                       lcdWritemensagem(0x10,0x09);  //quadrado
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                               }else{
                                   for(off=0; off<7;off++)
                                       lcdWritemensagem(0x10,0x09);  //quadrado
                               }

                       lcdWritemensagem(0x7F,0x09);  //backlight e RS em 1
                       lcdWritemensagem(0x7E,0x09);  //backlight e RS em 1

                       on=0;
                       off=0;

                           if(x>=0xF0){
                               for(on=0; on<7;on++)
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                           }else if(x>=0xD0){
                               for(on=0; on<6;on++)
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                               lcdWritemensagem(0x10,0x09);  //quadrado
                           }else if(x>=0xC0){
                               for(on=0; on<5;on++)
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                               for(off=0; off<2;off++)
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                           }else if(x>=0xB0){
                               for(on=0; on<4;on++)
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                               for(off=0; off<3;off++)
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                           }else if(x>=0xFF0){
                               for(on=0; on<3;on++)
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                               for(off=0; off<4;off++)
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                           }else if(x>=0x90){
                               for(on=0; on<2;on++)
                                   lcdWritemensagem(0xFF,0x09);  //quadrado
                               for(off=0; off<5;off++)
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                           }else if(x>=0x80){
                               lcdWritemensagem(0xFF,0x09);  //quadrado
                               for(off=0; off<6;off++)
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                           }else{
                               for(off=0; off<7;off++)
                                   lcdWritemensagem(0x10,0x09);  //quadrado
                           }


}

void escy(){
    int on=0;
           int off=0;
           if(y<=0x10){
                       for(on=0; on<7;on++)
                           lcdWritemensagem(0xFF,0x09);  //quadrado
                   }else if(y<=0x20){
                       lcdWritemensagem(0x10,0x09);  //quadrado
                       for(on=0; on<6;on++)
                           lcdWritemensagem(0xFF,0x09);  //quadrado

                   }else if(y<=0x30){
                       for(off=0; off<2;off++)
                           lcdWritemensagem(0x10,0x09);  //quadrado
                       for(on=0; on<5;on++)
                           lcdWritemensagem(0xFF,0x09);  //quadrado

                   }else if(y<=0x40){
                       for(off=0; off<3;off++)
                           lcdWritemensagem(0x10,0x09);  //quadrado
                       for(on=0; on<4;on++)
                           lcdWritemensagem(0xFF,0x09);  //quadrado

                   }else if(y<=0x50){
                       for(off=0; off<4;off++)
                           lcdWritemensagem(0x10,0x09);  //quadrado
                       for(on=0; on<3;on++)
                           lcdWritemensagem(0xFF,0x09);  //quadrado
                   }else if(y<=0x60){
                       for(off=0; off<5;off++)
                           lcdWritemensagem(0x10,0x09);  //quadrado
                       for(on=0; on<2;on++)
                           lcdWritemensagem(0xFF,0x09);  //quadrado
                   }else if(y<=0x70){
                       for(off=0; off<6;off++)
                           lcdWritemensagem(0x10,0x09);  //quadrado
                       lcdWritemensagem(0xFF,0x09);  //quadrado
                   }else{
                       for(off=0; off<7;off++)
                           lcdWritemensagem(0x10,0x09);  //quadrado
                   }

           lcdWritemensagem(0x5E,0x09);  //backlight e RS em 1
           lcdWritemensagem(0x5F,0x09);  //backlight e RS em 1

           on=0;
           off=0;

               if(y>=0xF0){
                   for(on=0; on<7;on++)
                       lcdWritemensagem(0xFF,0x09);  //quadrado
               }else if(y>=0xD0){
                   for(on=0; on<6;on++)
                       lcdWritemensagem(0xFF,0x09);  //quadrado
                   lcdWritemensagem(0x10,0x09);  //quadrado
               }else if(y>=0xC0){
                   for(on=0; on<5;on++)
                       lcdWritemensagem(0xFF,0x09);  //quadrado
                   for(off=0; off<2;off++)
                       lcdWritemensagem(0x10,0x09);  //quadrado
               }else if(y>=0xB0){
                   for(on=0; on<4;on++)
                       lcdWritemensagem(0xFF,0x09);  //quadrado
                   for(off=0; off<3;off++)
                       lcdWritemensagem(0x10,0x09);  //quadrado
               }else if(y>=0xFF0){
                   for(on=0; on<3;on++)
                       lcdWritemensagem(0xFF,0x09);  //quadrado
                   for(off=0; off<4;off++)
                       lcdWritemensagem(0x10,0x09);  //quadrado
               }else if(y>=0x90){
                   for(on=0; on<2;on++)
                       lcdWritemensagem(0xFF,0x09);  //quadrado
                   for(off=0; off<5;off++)
                       lcdWritemensagem(0x10,0x09);  //quadrado
               }else if(y>=0x80){
                   lcdWritemensagem(0xFF,0x09);  //quadrado
                   for(off=0; off<6;off++)
                       lcdWritemensagem(0x10,0x09);  //quadrado
               }else{
                   for(off=0; off<7;off++)
                       lcdWritemensagem(0x10,0x09);  //quadrado
               }


}

