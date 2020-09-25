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
    Atrasar15ms;
    ucbEnviaMsg(BIT_RS+BIT_RW );              //definir condições - Define bus de 8 bits
    ucbEnviaMsg(BIT_RS+BIT_RW);              //definir condições - Define bus de 8 bits
    Atrasar15ms;
    ucbEnviaMsg(BIT_RS+BIT_RW);              //definir condições - Define bus de 8 bit
    ucbEnviaMsg(BIT_RW);              //definir condições - Define bus de 4 bits
    Atrasar15ms;
    ucbEnviaMsg(BIT_RW);              //Define bus de 4 bits
    ucbEnviaMsg(BIT_BL);              //Define display com 2 linhas
    Atrasar15ms;
    ucbEnviaMsg(0x00);              //Cursor desbailitado e estático
    ucbEnviaMsg(BIT_BL+BIT_D4);              //Display ativado
    Atrasar15ms;
    ucbEnviaMsg(0x00);              //Preenche toda DDRAM com "brancos" (0x20), Leva o cursor p/ 1ª linha 1ª coluna
    ucbEnviaMsg(BIT_RS);              //incrementa AC a cada caractere escrito e alinha a DDRAM com o mostrador
    Atrasar15ms;
    ucbEnviaMsg(0x00);              //Incrementa AC a cada escrita
    ucbEnviaMsg(BIT_RW+BIT_E);              //Display parado e cursor anda
    Atrasar15ms;

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





