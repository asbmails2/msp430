#include <msp430.h>

/**
 * Programa 13
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Descrição do Roteiro
 * Repita o programa 12 usando as saídas do timer, ou seja, implemente a onda
 * PWM por hardware e não por software.
 */

int main(void)
{


  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // LED2 = P4.7
    P4SEL |= BIT7;                              // Seleciona P4.7 com função mapping.
    P4DIR |= BIT7;                              // P4.7, que é o LED2, será saída.
    P4OUT &= ~BIT7;                             // Define LED2 como apagado.

    PMAPKEYID = 0x02D52;                        // senha para liberara acesso ao remapeamento
    P4MAP7 = PM_TB0CCR1A;                       // Saída do clock é o led 4.7

    /*
     * SMCLK ~= 1 MHz:
     * Com o id_1 dividi 2 o SMCLK
     * 0,5MHz é 1s
     * TB0CCR0 precisa 0.02s
     * TB0CCR0 será de 10488Hz
     */

      TB0CTL = TBSSEL_2 |                   //seleciona o SMCLK
                   ID_1 |                   //Divide o clock por 2
                   MC_1 |                   //modo de subida simples até chegar o clock
                  TBCLR;                    //zerar a tag de subida.

      TB0EX0 = TBIDEX_0;                    //liberar a trava do sistema
      TB0CCTL1 = (OUTMOD_7 | CLLD_1);       //ativando a saída da PWM do clock B pela porta 4.7
                                            //de acordo com o flaco de subida do ccr1 para ligar o led
                                            //e desligar o led quando for o flanco de descida do ccr1
      TB0CTL &= ~TBCLR;                     //zera a trava do controle do timer B
      TB0CCR0 = 10500;                      //coloca o ccr0 do timer b como 10500
                                            //isso limita a cotagem do timer b até 10500.

        // 100% = 10500
        // 1 passo = 5% =  525
        // multiplicativo começa em 0 e termina em 20:


        int pot = 0;                        //contador para a potência indica a quantidade de pontos que o liga
                                            //o led a cada ciclo de 20
        const int max = 20;                 //constante para o máximo da potência
        int subindo = 1;                    // 1 = sobe e 0 = desce

        while(1) {                                  // Loop incondicional.

                if((TB0CTL & TBIFG) != 0) {         //Checa se chegou ao ccr0 do timer b
                    P4OUT ^= BIT7;                  // XOR no LED2 para inverter estado.
                    TB0CTL &= ~TBIFG;               //zera o controle do timer b
                    if (subindo==1)                 //se está subindo
                        pot++;                          //aumenta a potência
                    else                            //se descendo
                        pot--;                          //diminui a potência do led

                    TB0CCR1 = 525 * pot;            //varia o ccr1 que também varia o TB0CTL
                                                    //quando chegar no valor de ccr1
                                                    //para poder ter a vaa
                }
                if (pot > max){                     //se chegou ao máximo põe estado para
                    subindo = 0;                        //descendo
                    pot = max;                          //pot igual ao máximo.
                }
                if (pot<0){                          //se a pot chegou abaixo de 0
                    subindo = 1;                        //põe para subir a pwm
                    pot = 0;                            //põe a potência em zero
                }
            }
}
