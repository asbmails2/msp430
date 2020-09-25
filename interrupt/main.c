#include <msp430.h>
// Piscar o LED2 por laço de software
// Por interrupção, a cada acionamento de SW1, inverter LED1
#define LED1 BIT0
#define LED1_OUT P1OUT
#define LED2 BIT7
#define LED2_OUT P4OUT
#define SW1 BIT1
#define SW1_OUT P2OUT

void isr_sw1(void);
int main(void) {
volatile unsigned int i;
WDTCTL = WDTPW | WDTHOLD;
P1DIR |= LED1;
LED1_OUT &= ~LED1;
P4DIR |= LED2;
LED2_OUT &= ~LED2;
P2DIR &= ~SW1;
P2REN |= SW1;
P2OUT |= SW1;
P2IES |= SW1;
P2IE |= SW1;
__enable_interrupt();
// Stop watchdog timer
while(1){
for (i=0; i<65535; i++) ;
LED2_OUT ^= LED2;
}
return 0;
}

//Também pode-se usar: #pragma vector = 42
#pragma vector = PORT2_VECTOR
__interrupt void port2(void){
int n;
n = __even_in_range(P2IV,0x10);
switch(n){
case 0x0: break;
case 0x4: isr_sw1(); break;
case 0x8: break;
case 0xC: break;
case 0x10: break;
case 0x2: break;
case 0x6: break;
case 0xA: break;
case 0xE: break;
}
}

void isr_sw1(void){
LED1_OUT ^= LED1;
}
