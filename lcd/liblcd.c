/*
 * liblcd.c
 *
 *  Created on: 8 de nov de 2017
 *
 */

#include <liblcd.h>


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

    int nibble1,nibble0,a=0;

    nibble1 = 0;
    nibble0 = 0;

    nibble0 = ascii & 0x0F;     //Define os Nibbles
    nibble1 = ascii & 0xF0;
    nibble1 = nibble1>>4;           //Desloca para que fique no menos significativo

    lcd_wr_nib(nibble1,config);     //Escreve
    lcd_wr_nib(nibble0,config);     //Escreve
    atraso(50);                     //Atraso
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
