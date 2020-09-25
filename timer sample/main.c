#include "msp430f5529.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; /* Good dog, no barking */

  P4DIR |= BIT7; /* Digital out to a LED */


  TA1CCR0 = 32768-1;
  TA1CTL = TASSEL_1 + MC_1 + TACLR + ID_0;
  TA1CCTL0 = CCIE ;

  __bis_SR_register(GIE); /* Enable maskable interrupts */

  while(1) { /* CPU does nothing and thus no harm */ }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    P4OUT ^= BIT7;
}
