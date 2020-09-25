#include <msp430.h>
#include <liblcd.h>
#include <libserial.h>


/**
 * Programa 18
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Escreva um programa em C que faça o teste do LCD: escreva todos os
 * caracteres da tabela ASCII (todos os 128 caracteres) nas duas linhas do LCD, escrevendo um
 * caractere a cada 0.2 segundos, aproximadamente, e esperando 3 segundos antes de apagar o
 * display para voltar a escrever na primeira linha.
 *
 */

int byte,i,j,k;
volatile a;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    lcdi2cstart();
    byte = 0x00;
    while (1){
        if (byte >= 0x7F)
         byte = 0x00;            //chegando em 128 volta ao 0
        for (i=0;i<16;i++){
            lcdWriteByte(byte,0x09);  //backlight e RS em 1
            for (j=0;j<4;j++)
                atraso(5000);
            byte++;                  //Próximo
        }

        lcdWriteByte(0xC0,0x08);     //pula linha se chegou no final dela

        for (i=0;i<16;i++){
            lcdWriteByte(byte,0x09);
            byte++;
            for (j=0;j<4;j++)
                atraso(5000);
        }
        for (k=0;k<100;k++)
            atraso(3000);
        lcdWriteByte(0x01,0x08);         //Quando terminar o display, dá um atraso de 3s e reinicia
    }
}
