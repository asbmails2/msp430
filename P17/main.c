#include <msp430.h> 
#include <libserial.h>
#include <liblcd.h>

/**
 * Programa 17
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Descrição do Roteiro
 * Escreva duas funções lcdBacklightON() e lcdBacklightOFF() para ativar e
 * desativar a retroiluminação do LCD. Na configuração da interface serial, mantenha a frequência
 * da linha SCL abaixo de 100kHz. O chip PCF8574 possui o endereço 0x3F no barramento I2C.
 *
 */




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	    lcdi2cstart();
        while(1){
            lcdBacklightOFF();
            atrasar(10000);
            lcdBacklightON();
            atrasar(100000);


        }


}
