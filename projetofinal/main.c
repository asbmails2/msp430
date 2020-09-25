#include <projetofinal.h>


/**
 * Objetivo é a criação de um controlador de um estufa.
 *
 */


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    pwmconfig ();
    adcConfig ();
    ioConfig ();
    i2cconfigSlave(0x48);
    while (1){

        LigaVentilador;
        atrasar(100000);
        LigaAquecedor;
        atrasar(100000);
        retorno = ligaBombaAgua(10);
        atrasar(100000);
        DeslAquecedor;
        atrasar(100000);
        DeslVentilador;
        atrasar(100000);

        atrasar(100000);
        retorno = ligaBombaAgua(10);

    }
}

