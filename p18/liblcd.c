/*
 * liblcd.c
 *
 *  Created on: 8 de nov de 2017
 *
 */

#include <liblcd.h>
// Bits para controle do LCD
#define BIT_RS   BIT0
#define BIT_RW   BIT1
#define BIT_E    BIT2
#define BIT_BL   BIT3
#define BIT_D4   BIT4
#define BIT_D5   BIT5
#define BIT_D6   BIT6
#define BIT_D7   BIT7

unsigned char lastByte=0;                  //Úlitmo valor escrito na porta

void lcdi2cstart (void){


    ucbConfigPinos();
    ucbConfig(0x3F);
    lcdZerarRsWr(0x00);         //Manter RS E RW = 0
    lcdBacklightON();       //Mostrador Ligado
    Atrasar15ms;          //Esperar mais de 15ms
    ucbEnviaMsg(0x38);        //Definir condições 0011
    lcdEnable(0x38);       //Pulso de Enable
    Atrasar100us;
    lcdElow(0x38);
    Atrasar15ms;
    Atrasar15ms;
    ucbEnviaMsg(0x38);        //Definir condições 0011
    lcdEnable(0x38);
    Atrasar100us;
    lcdElow(0x38);
    Atrasar1ms;
    ucbEnviaMsg(0x38);        //Definir condições 0011
    lcdEnable(0x38);
    Atrasar100us;
    lcdElow(0x38);
    ucbEnviaMsg(0x28);        //4 Bits 0010
    lcdEnable(0x28);
    Atrasar100us;
    lcdElow(0x28);
    lcdWriteByte(0x2C,0x08); // 0010 NFxx: N e F ativados
    lcdWriteByte(0x0F,0x08); //Mostrador ativado com cursor pulsante
    lcdWriteByte(0x01,0x08); //Limpa
    lcdWriteByte(0x06,0x08); //I = 0 S = 1
}

void lcdClean(void){
     lastByte=0;
     ucbEnviaMsg(lastByte);

}
void lcdBacklightON(void){
    lastByte= lastByte | BIT_BL;
    ucbEnviaMsg(lastByte);
}


void lcdBacklightOFF(void){
    lastByte= lastByte & ~BIT_BL;
    ucbEnviaMsg(lastByte);
}

void lcdZerarRsWr(unsigned char enviar){
    lastByte = ~(BIT_RS+BIT_RW) & enviar;
    sendLCD();
}

void lcdWriteByte(unsigned char ascii, unsigned char config){

        unsigned char n1=0,n0=0;
        n0 = ascii & 0x0F;     //separa nibles
        n1 = ascii & 0xF0;
        n1 = n1>>4;           //desloca o nibble para a esquerda

        lcdSendNibble(n1,config);     //Escreve
        lcd_wr_nib(n0,config);     //Escreve
        Atrasar1ms;               //Atraso
        Atrasar1ms;
;
}


void lcdSendNibble(char n ,char config){
        char byte=0x00;
        n = n<< 4;     //desloca o nible para a esquerda
        n &= 0xF0;     //zera o nibble menos significativo
        config &= 0x0F;        //Config = 0000XXXX (Bits de BL, RS, RW E EN)
        byte = config | n;      //Palavra = 1111XXXX
        ucbEnviaMsg(byte);     //Envia Nibble
        lcdEnable(byte);
        Atrasar100us;;
        lcdElow(byte);
        Atrasar1ms;               //Atraso
        Atrasar1ms;
}

void lcdElow(char byte){
    byte &= ~BIT_E;       //tira o enable
    ucbEnviaMsg(byte);
}

void lcdEhigh(char palavra){
    palavra |= BIT_E;     //ativa o Enable
    ucbEnviaMsg(palavra);
}


