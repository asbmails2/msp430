#include <msp430.h> 

/**
 * Programa 9
 *
 * Adriano S BrandÃ£o 140033955
 * Gustavo Abreu da Cunha140041133
 *
 * Turma D
 *
 **/
void atraso(void);
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1OUT &= ~BIT0;             // Apagando o LED1
    P4OUT &= ~BIT7;             // Apagando o LED2
    P1DIR |= BIT0;              // Coloca o Led 1 como saída
    P4DIR |= BIT7;              // Coloca o Led 2 como saida
    P1DIR &= ~BIT1;             // Coloca P1.1 como entrada
    P1OUT |= BIT1;              // Aciona os resistores pull up/dw
    P1REN |= BIT1;              // Esolhe (habilita) pull up

    while(1){                   // Vamos fazer o Loop



        P1OUT &= ~BIT0;         // Apagando o LED1
        P4OUT &= ~BIT7;         // Apagando o LED2
        atraso();               // Batida da chave - Debouncer - delay

        while((P1IN & BIT1) == 0){
                 P1OUT &= ~BIT0; //Apagando o LED1
                 P4OUT &= ~BIT7; //Apagando o LED2
         }
        atraso();                // Batida da chave - Debouncer - delay
                                 // Lendo a chave, fica no laco enquanto ela estiver aberta
        while((P1IN & BIT1) == BIT1){
            P1OUT &= ~BIT0;      // Apagando o LED1
            P4OUT &= ~BIT7;      // Apagando o LED2
         }



        // O que faz quando apertar a chave
        atraso();                // Batida da chave - Debouncer - delay
        P4OUT |= BIT7;           //Acende LED 2
        atraso();                // Batida da chave - Debouncer - delay

        // Lendo a chave, fica no laco enquanto ela estiver fechada
        while((P1IN & BIT1) == 0){
            P1OUT &= ~BIT0;       //Apagando o LED1
            P4OUT |= BIT7;        //Apagando o LED2
         }


        // Lendo a chave,
        while((P1IN & BIT1) == BIT1){
            P1OUT &= ~BIT0;        //Apagando o LED1
            P4OUT |= BIT7;         // Acende LED2
         }


        // A��o ao acionar a chave
        atraso();                  // Batida da chave - Debouncer - delay
        P4OUT &= ~BIT7;            //LED2 apagado
        P1OUT |= BIT0;             //Acende LED1
        atraso();                  // Batida da chave - Debouncer - delay

        // Lendo a chave,
        while((P1IN & BIT1) == 0){
            P4OUT &= ~BIT7;        //LED2 apagado
            P1OUT |= BIT0;         //Acende LED1
         }

        // Lendo a chave,
        while((P1IN & BIT1) == BIT1){
            P4OUT &= ~BIT7;        //LED2 apagado
            P1OUT |= BIT0;         //Acende LED1
         }



         // Quando apertar
         atraso();                  // Batida da chave - Debouncer - delay
         P1OUT |= BIT0;             // Acende LED2
         P4OUT |= BIT7;             // Acende LED2
         atraso();                  // Batida da chave - Debouncer - delay

         // Lendo a chave
        while((P1IN & BIT1) == 0){
            P1OUT |= BIT0;          // Acende LED1
            P4OUT |= BIT7;          // Acende LED2
         }

         // Lendo a chave
         while((P1IN & BIT1) == BIT1){
             P1OUT |= BIT0;         // Acende LED1
             P4OUT |= BIT7;         // Acende LED2
          }

    }

    return 0;
}

// Batida da chave - Debouncer - delay
void atraso(void) {
    volatile unsigned int k = 20000
            ;
    while(k > 0){
        k--;
    }
    return;
}
