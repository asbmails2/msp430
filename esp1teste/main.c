#include <msp430.h> 


/**
 * esp code teste
 * Conexão do ESP 01
 * TX: .
   RX: conectado ao TX2 do MEGA via um conversor de nível
   VCC: 3.3V
   GND: Ground. Nunca é demais lembrar que é necessário conectar o GND do ESP8266 ao GND do MEGA.
   CH_PD (*): conectado ao pin 4 do MEGA para reset de SW para iniciar comunicação
   RST: RESET em geral, conecta em VCC
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	return 0;
}

void pinconfig(void){
    P3SEL |= BIT0 + BIT1;
}

void reset
