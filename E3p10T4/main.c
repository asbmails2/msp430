#include <msp430.h> 

/**
 * Programa 10
 *
 * Adriano S BrandÃ£o 140033955
 * Gustavo Abreu da Cunha140041133
 *
 * Turma D
 *
 * Esse programa visa controlar o acendimento dos led 1 e 2 através do acionamento
 * das chaves S1 e S2 na forma:
 *      Acionando S1 troca o valor do Led 1
 *      Acionando S2 troca o valor do led 2
 *      Acionando S1 e s2 os leds piscam na frequência de 1 hz
 */

#define ATRASO 50000                             // Constante de Atraso.

//protótipos
void piscapisca (void);                    // Protótipo da função de piscar LED's.
unsigned char debou (volatile unsigned char * in, unsigned char bit);

//contadores
volatile unsigned int i =0;

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;                   // Para o watchdog timer.

    // LEDs - preparando
    P1SEL &= ~BIT0;                             // Seleciona p1.1 como i/o
    P4SEL &= ~BIT7;                             // Seleciona P4.7 como I/O.
    P1DIR |= BIT0;                              // led p1.0 como out.
    P4DIR |= BIT7;                              // led p4.7 como out.
    P1OUT &= ~BIT0;                             // led1 como apagado.
    P4OUT &= ~BIT7;                             // led2 como apagado.

    // botões, preparando
    P1SEL &= ~BIT1;                             // seleciona p1.1 e p4.7 como i/o
    P2SEL &= ~BIT1;
    P1DIR &= ~BIT1;                             // P1.1(s2)  e p2.1(s1) como entrada
    P2DIR &= ~BIT1;
    P1REN |= BIT1;                              // Pull-up/down habilitado.
    P2REN |= BIT1;
    P1OUT |= BIT1;                              // Pull-up escolhido.
    P2OUT |= BIT1;

/*
 * As chaves S1(p2,1) e S2(p1.1) estão em pull-up então são acionadas
 * Chars para salvar o estado dos botões clicados
 */

    char s1 = 0;
    char s2 = 0;


    while(1) {                                  // Loop incondicional.

        if ( (debou(&P2IN,BIT1))==0)              //se precionado salva s1 como zero.
            s1= 0;
        if ( (debou(&P1IN,BIT1))==0)             //se precionado salva s2 como zero.
            s2= 0;
        /*
         * Treço responsável por pegar se s1 está clicado e o valor anterior
         * não havia sido clicado. Assim segura o valor até haver outro clique.
         * Além disso checa se s2 está clicada para entrar no piscapisca.
         * Se S1 está precionada não entra no if.
         * No else if é feito o mesmo só que para a chave s2.
         */
        if (debou(&P2IN,BIT1) & !s1) {
            s1 = 1;
            if (debou(&P1IN,BIT1) ) {
                s2 = 1;
                piscapisca();
            }else
                P1OUT ^= BIT0;
        }else if (debou(&P1IN,BIT1) & !s2) {
            s2 = 1;
            if (debou(&P2IN, BIT1)) {
                s1 = 1;
                piscapisca();
            }
            else
                P4OUT ^= BIT7;
        }
    }
}
/*
 * Função piscapisca()
 * Nessa função salva o valor dos leds p1.0 e p4.7 antes de começar.
 * Faz os leds piscarem em 1 hz de frequência aproximadamente.
 * Por fim, recupera o valor do estado após a conclusão do loop while
 *
 */

void piscapisca (void) {

    unsigned char l1 = P1OUT;
    unsigned char l2 = P4OUT;
    P1OUT |= BIT0;                              //led acesso
    P4OUT &= ~BIT7;                             // led apagado
    while ((debou (&P2IN,BIT1))|| (debou (&P1IN,BIT1))) {
        for(i=ATRASO;i>0;i--);                  // atrasando em meio segundo
        P1OUT ^= BIT0;                          // inverte o estado do led
        P4OUT ^= BIT7;                          // inverte o estado do led
    }
    P1OUT = l1;                                 // Retorna ao estado inicial do LED1.
    P4OUT = l2;                                 // Retorna ao estado inicial do LED2.
}

/*
 * Função debou
 * trata o clique de entrada e trata os rebotes da chave através do bit e do endereço
 * da porta de entrada.
 * TRatamento de deboucing feito com dupla análise da chave após 1000 cliques de clock
 * (decremento d o contador.
 */
unsigned char debou(volatile unsigned char * in, unsigned char bit) {
    if ((*in & bit))                        //se não pressionada sai
        return 0;
    for(i = 1000;i>0;i--);
    if ((*in & bit))
        return 0;                               //se não pressionada sai
    return 1;                                   //se pressionada volta um
}
