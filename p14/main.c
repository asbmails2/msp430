#include <intrinsics.h>
#include <stdint.h>
#include <msp430.h>


/**
 * Programa 14
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 *
 * Turma D
 *
 * Descrição do Roteiro
 *
 *O sensor de proximidade abaixo funciona por pulsos ultrassônicos. 8 pulsos são enviados
 *quando o sinal de trigger é ativado e uma resposta do tempo de propagação é devolvida no sinal
 *echo. Configure o timer para medir o tempo de propagação que aparece no sinal echo, sabendo
 *que o trigger tem duração de 10us. Atenção, o sensor é alimentado com 5V. Use então um
 *que divisor resistivo no retorno do echo para não queimar o pino de entrada da sua plaquinha.
 */

#define TRIGGER_PIN BIT1   // P6.1
#define ECHO_PIN BIT3  // P1.3
#define LED_PIN BIT0   // P1.0
#define DISTANCE_THRESHOLD 60  // cm
#define MEASURE_INTERVAL 2048  // ~250 ms

void triggerMeasurement() {
    static volatile int trigWait;

    // Start trigger
    P6OUT |= TRIGGER_PIN;

    // Wait a small amount of time with trigger high, > 10us required (~10 clock cycles at 1MHz MCLK)
    for (trigWait = 0; trigWait < 12; trigWait++) {}

    // End trigger
    P6OUT &= ~TRIGGER_PIN;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    // Configure trigger pin, low to start
    P6DIR |= TRIGGER_PIN;
    P6OUT &= ~TRIGGER_PIN;

    // Configure LED, off to start
    P1DIR |= LED_PIN;
    P1OUT &= ~LED_PIN;

    // Configure echo pin as capture input to TA0CCR2
    P1DIR &= ~ECHO_PIN;
    P1SEL |= ECHO_PIN;

    // Set up TA0 to capture in CCR2 on both edges from P1.3 (echo pin)
    TA0CCTL2 = CM_3 | CCIS_0 | SCS | CAP | CCIE;

    // Set up TA0 to compare CCR0 (measure interval)
    TA0CCR0 = MEASURE_INTERVAL;
    TA0CCTL0 = CCIE;

    // Set up TA0 with ACLK / 4 = 8192 Hz
    TA0CTL = TASSEL__ACLK | ID__4 | MC__CONTINUOUS | TACLR;

    uint16_t lastCount = 0;
    uint32_t distance = 0;

    for (;;)
    {
        triggerMeasurement();

        // Wait for echo start
        __low_power_mode_3();

        lastCount = TA0CCR2;

        // Wait for echo end
        __low_power_mode_3();

        distance = TA0CCR2 - lastCount;
        distance *= 34000;
        distance >>= 14;  // division by 16384 (2 ^ 14)

        if (distance <= DISTANCE_THRESHOLD)
        {
            // Turn on LED
            P1OUT |= LED_PIN;
        }
        else
        {
            // Turn off LED
            P1OUT &= ~LED_PIN;
        }

        // Wait for the next measure interval tick
        __low_power_mode_3();
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void) {
    // Measure interval tick
    __low_power_mode_off_on_exit();
    TA0CCR0 += MEASURE_INTERVAL;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void) {
    // Echo pin state toggled
    __low_power_mode_off_on_exit();
    TA0IV = 0;
}
