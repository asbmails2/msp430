/*
 * libserial.c
 *
 *  Created on: 8 de nov de 2017
 * Biblioteca com as funções específicas para o funcionamento de envio e
 * recebimento de mensagens via protocolo serial I2C
 *
 * Com o uso dos pinos 3.0 e 3.1 com o hardware dedicado do MSP430.
 *
 */

#include <libserial.h>

#define DELAY_100us   atrasar(3)        //104 us
#define DELAY_500us   atrasar(15)       //513 us
#define DELAY_1ms     atrasar(30)       //1,08 ms
#define DELAY_5ms     atrasar(150)      //5,07 ms
#define DELAY_15ms    atrasar(450)      //15,6 ms

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

      while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar Transmitir

      UCB0CTL1 |= UCTXSTP;                        //Gerar STOP

      while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //Esperar STOP

      DELAY_15ms;                                  //Atraso p/ escravo perceber stop

      return 1;                                   //return 1 para ok
}

void atrasar (unsigned long atrasa){
   volatile unsigned long i = atrasa;
   while(i != 0)
            i--;
}





