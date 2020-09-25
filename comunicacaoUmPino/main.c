#include <msp430.h> 


/**
 * Fazer a comunicação serial com a taxa de 9600 baud rate
 * de forma que o
 * ccr0 = Tbit
 * p1.3 - tx
 * Tbit = 1/f = 1/9600
 * AClk = 32768hz
 * MClk = 1MHz
 *
 * aclk = 3,4
 * mclk = 104,15
 *
 * CCR0 = 104;
 *
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	txCounter = 8;
	P1OUT &= ~BIT3;
	TACTL  |= MC_UP;
	__low_power_mode_0();
	


	return 0;
}

void config(){
    /*
     * configura p1.3
     * configurar o Timer A[smclk , ccr0 = 104 , ccie para ccr0;
     * __enable_interrupt.
     */
}

#progma vector=TA0CCR0
__interrupt void ISR_Ta(){
    if(txCounter--){
    if(dataTX&BIT0){
        P1OUT |= BIT3;
    }else
        P1OUT &= ~BIT3;

    dataTX>>=1; //dataTX = dataTX >>1
    }else{
        __low_power_mode_off_on_exit();
    }

}
