/*
 * configI2C.c
 *
 *  Created on: 7 de dez de 2017
 *      Author: asb
 */

#include <projetofinal.h>

/*
 * Método para a configuração do timer com o
 *
 * */
void i2cconfigSlave(unsigned char end){
    P3REN |= (BIT0 + BIT1); //Enable resistor
    P3OUT |= (BIT0 + BIT1); //Pull-up

    P3SEL |= (BIT0 + BIT1); //seleciona em modo específico I2C dos pinos
     UCB0CTL1 |= UCSWRST;                      // Enable SW reset
     UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
     UCB0BRW = 10;                             // 100khz
     UCB0I2COA = end;                          // Endereço do Escravo
     UCB0CTL1 = UCSSEL_2;                      // Use SMCLK, keep SW reset
     UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
     UCB0IE |= UCRXIE;                         // Enable RX interrupt

}

// USCI_B0 Data ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6: break;                           // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10:                                  // Vector 10: RXIFG
    RXData = UCB0RXBUF;                     // Get RX data
    switch (RXData){
    case 1:
        LigaVentilador;
    break;
    case2:
    DeslVentilador;
    break;
    case 3:
        LigaAquecedor;
        break;
    case 4:
        DeslAquecedor;
        break;
    case 5:
        ligaBombaAgua(5);
        break;
    case 6:

        break;
    default:
        UCB0TXBUF = (char)tempSensor;

        break;
    }
    break;
  case 12: break;                           // Vector 12: TXIFG
  default: break;
  }
}

