#include <msp430.h>

void initucb(unsigned char endereco);
void initlcd (void);
void delay (unsigned long atrasa);
void enviaLCD(unsigned char byte, unsigned char conf);
void enviaLcdNib(unsigned char byte, unsigned char conf);
int enviaI2C(unsigned char msgi);
void colocaQuadradosY(void);
void colocaQuadradosX(void);
void initTimerADC();
void initDMA();

// Bits para controle do LCD
#define RS   BIT0
#define RW   BIT1
#define E    BIT2
#define BL   BIT3

#define DELAY15ms      delay(450)
#define DELAY1ms       delay(30)
#define DELAY100us     delay(3)
#define DELAY2ms       delay(50)


//lcd config = 0x09 é para mandar a mesngem de lcd ligado
// e deixar o lcd em modo escrita

int x,y,k;

unsigned char msg=0;                  //Úlitmo valor escrito na porta


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    initTimerADC();
    initDMA();
    initlcd();

    while (1){
        //aguarda conversão
        while ( (ADC12IFG&ADC12IFG0) == 0);
        while ( (ADC12IFG&ADC12IFG1) == 0);

        colocaQuadradosY();

        enviaLCD(0xC0,0x08);     //pula linha se chegou no final dela

        colocaQuadradosX();

        enviaLCD(0x02,0x08);
        DELAY2ms;
    }
}

void delay (unsigned long ciclos){
   volatile unsigned long i =ciclos;
   while(i != 0)i--;
}

/*
 * Método para a configuração do UCB
 *
 * */
void initucb(unsigned char endereco){

     P3REN |= BIT0;    //Enable resistor
     P3OUT |= BIT0;    //Pull-up
     P3SEL |= BIT0;    //seleciona em modo específico I2C dos pinos
     P3REN |= BIT1;    //Enable resistor
     P3OUT |= BIT1;    //Pull-up
     P3SEL |= BIT1;    //seleciona em modo específico I2C dos pinos

     UCB0CTL1 |= UCSWRST;                      // REset
     UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
     UCB0BRW = 10;                             // 100khz
     UCB0I2CSA = endereco;                     // endereço de envio do escravo
     UCB0CTL1 = UCSSEL_2;                      // SMCLK

}

void initlcd (void){

    initucb(0x3F);
    msg = ~(RS+RW);         //zerar display
   enviaI2C(msg);
    msg= msg | BL;          //luz ligada
   enviaI2C(msg);

    DELAY15ms;        //inicia configuração

    msg = 0x38;

   enviaI2C(msg);
    msg |= E;     //ativa o Enable
   enviaI2C(msg);
    DELAY100us;
    msg &= ~E;
   enviaI2C(msg);
    DELAY15ms;
    DELAY15ms;

   enviaI2C(msg);
    msg |= E;     //ativa o Enable
   enviaI2C(msg);
    DELAY100us;
    msg &= ~E;
   enviaI2C(msg);
    DELAY15ms;
    DELAY15ms;

   enviaI2C(msg);
    msg |= E;     //ativa o Enable
   enviaI2C(msg);
    DELAY100us;
    msg &= ~E;
   enviaI2C(msg);
    DELAY15ms;
    DELAY15ms;

    msg = 0x28;

   enviaI2C(0x28);        //4 Bits 0010
    msg |= E;     //ativa o Enable
   enviaI2C(msg);
    DELAY100us;
    msg &= ~E;
   enviaI2C(msg);

    enviaLCD(0x2C,0x08); // 0010 NFxx: N e F ativados
    enviaLCD(0x0F,0x08); //Mostrador ativado com cursor pulsante
    enviaLCD(0x01,0x08); //Limpa
    enviaLCD(0x06,0x08); //I = 0 S = 1
}

int enviaI2C(unsigned char msgi){

      UCB0CTL1 |= UCTR    |                       //Mestre transmissor
                  UCTXSTT;                        //Gerar START e envia endereço
      while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar TXIFG (completar transm.)
      if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
         return -1;
      UCB0TXBUF = msgi;                           //Dado a ser escrito
      while ( (UCB0IFG & UCTXIFG) == 0){}         //Esperar Transmitir
      UCB0CTL1 |= UCTXSTP;                        //Gerar STOP
      while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //Esperar STOP
      return 1;                                   //return 1 para ok
}


void enviaLCD(unsigned char byte, unsigned char conf){

    char nib2 = byte & 0x0F; //pega nibble menos sig
    char nib1 = byte & 0xF0; //pega nibble mais sig
    nib1 = nib1 >>4;         //passa o nib mais sig para a posição de nib normal.
    enviaLcdNib(nib1,conf);     //Escreve
    enviaLcdNib(nib2,conf);     //Escreve
    DELAY2ms;                   //atrasar
}

void enviaLcdNib(unsigned char byte, unsigned char conf){
    unsigned char mensagem=0x00;
    byte = byte << 4;        //nibble deslocado para esquerda
    byte &= 0xF0;     //limpa a parte não significativa
    conf &= 0x0F;     //configuração (4bits
    mensagem = conf | byte;      //Palavra = 1111XXXX
    enviaI2C(mensagem);
     mensagem |= E;     //ativa o Enable
    enviaI2C(mensagem);
     DELAY100us;
     mensagem &= ~E;
    enviaI2C(mensagem);
}

void initTimerADC(void){
    TA0CTL = TAIDEX_0;
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;    //SMCLK = 1MHZ
    TA0CCR0 = 10000;         //Tempo de 20ms
    TA0CCTL1 = OUTMOD_6;
    TA0CCR1 = 5000;

    ADC12CTL0 &= ~ADC12ENC;         //desliga o ADC
    ADC12CTL0 = ADC12SHT0_3 | ADC12ON;            //liga ADC
    //salva em ADC12MEM0, usa o TA0.1, seleciona ele como fonte, não divide, fonte sinal SMCK ,Canais a0 e a1
    ADC12CTL1 = ADC12CSTARTADD_0 | ADC12SHS_1 | ADC12SHP | ADC12DIV_0 | ADC12SSEL_3 | ADC12CONSEQ_3;
    //desligar sensor temp e ativa o modo 8bit
    ADC12CTL2 = ADC12TCOFF |  ADC12RES_0;
    //ReferÊncia amostral em 3.3 a 0V e pino p6.0
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_0;
    //Lê até o A1, referÊncia amostral em 3.3 a 0V e pino p6.1
    ADC12MCTL1 = ADC12EOS |  ADC12SREF_0 | ADC12INCH_1;
    //liga o adc
    ADC12CTL0 |= ADC12ENC;
}

void initDMA(void){
    //mapeia o dma para o ADC
    DMACTL0 = DMA0TSEL_24 | DMA1TSEL_24;
    DMA0CTL = DMAEN | DMADT_4;          //habilita o modo repetido no DMA0
    DMA0SA = (__SFR_FARPTR)0x0720;      //acessa a memória do adc canal A0
    DMA0DA = &x;                        //destino x
    DMA0SZ = 1;                         //1 transferência por vez
    DMA1CTL = DMAEN | DMADT_4;          //habilita o modo repetido no DMA1
    DMA1SA = (__SFR_FARPTR)0x0722;      //acessa a memória do adc canal A1
    DMA1DA = &y;                        //destino y
    DMA1SZ = 1;                         //1 transferência por vez
}


void colocaQuadradosX (void){
    int blocos = 0;  //número de blocos a ser preenchido
    if(x<=0x10)blocos=7;
    else if(x<=0x20)blocos=6;
    else if(x<=0x30)blocos=5;
    else if(x<=0x40)blocos=4;
    else if(x<=0x50)blocos=3;
    else if(x<=0x60)blocos=2;
    else if(x<=0x70)blocos=1;

    k=7-blocos;
    for(;k>0;k--)
       enviaLCD(0x10,0x09);  //quadrado
    k=blocos;
    for(k=blocos; k>0;k--)
       enviaLCD(0xFF,0x09);  //quadrado

    enviaLCD(0x7F,0x09);  //backlight e RS em 1
    enviaLCD(0x7E,0x09);  //backlight e RS em 1

    blocos = 0;
    if(x>=0xF0)blocos = 7;
    else if(x>=0xD0)blocos = 6;
    else if(x>=0xC0)blocos = 5;
    else if(x>=0xB0)blocos = 4;
    else if(x>=0xA0)blocos = 3;
    else if(x>=0x90)blocos = 2;
    else if(x>=0x80)blocos = 1;

    k=blocos;
    for(; k>0;k--)
       enviaLCD(0xFF,0x09);  //quadrado
    k=7-blocos;
    for(; k>0;k--)
       enviaLCD(0x10,0x09);  //quadrado
}

void colocaQuadradosY(){
    int blocos = 0;  //número de blocos a ser preenchido
       if(y<=0x10)blocos=7;
       else if(y<=0x20)blocos=6;
       else if(y<=0x30)blocos=5;
       else if(y<=0x40)blocos=4;
       else if(y<=0x50)blocos=3;
       else if(y<=0x60)blocos=2;
       else if(y<=0x70)blocos=1;

       k=7-blocos;
       for(;k>0;k--)
          enviaLCD(0x10,0x09);  //quadrado
       k=blocos;
       for(k=blocos; k>0;k--)
          enviaLCD(0xFF,0x09);  //quadrado

       enviaLCD(0x76,0x09);  //backlight e RS em 1
       enviaLCD(0x5E,0x09);  //backlight e RS em 1

       blocos = 0;
       if(y>=0xF0)blocos = 7;
       else if(y>=0xD0)blocos = 6;
       else if(y>=0xC0)blocos = 5;
       else if(y>=0xB0)blocos = 4;
       else if(y>=0xA0)blocos = 3;
       else if(y>=0x90)blocos = 2;
       else if(y>=0x80)blocos = 1;

       k=blocos;
       for(; k>0;k--)
          enviaLCD(0xFF,0x09);  //quadrado
       k=7-blocos;
       for(; k>0;k--)
          enviaLCD(0x10,0x09);  //quadrado
}

