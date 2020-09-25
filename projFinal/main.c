#include <projetofinal.h>


/**
 * Objetivo é a criação de um controlador de um estufa.
 *
 * Adriano S Brandão 140033955
 * Gustavo Abreu da Cunha 140041133
 */


int main(void)
{
 	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	pwmconfig ();
	adcConfig ();
	ioConfig();
	i2cconfigSlave(0x48);
	while (1){

	    if(lumSensor >150 && lumSensor >200){
	        pwmSetPotencia(lumSensor- 149);
	    }else if (lumSensor >250)
	        pwmSetPotencia(100);
	    else
	        pwmSetPotencia(0);

	    if ((P6IN&BIT2)>0){
	        ligaBombaAgua(10);
	    }

	    if(tempSensor > 150){
	        LigaVentilador;
	    }else{
	        DeslVentilador;
	    }

	    if(tempSensor > 70){
	                LigaAquecedor;
	            }else{
	                DeslAquecedor;
	            }
/*
	    LigaVentilador;
	    __delay_cycles(1000);
	    DeslVentilador;
	    __delay_cycles(1000);
	    LigaAquecedor;
	    __delay_cycles(1000);
	    DeslAquecedor;
        __delay_cycles(1000);
        pwmSetPotencia(5);
        __delay_cycles(1000);
        pwmSetPotencia(45);
        __delay_cycles(1000);
        pwmSetPotencia(85);
        __delay_cycles(1000);
        pwmSetPotencia(100);

        __delay_cycles(1000);


	    pwmSetPotencia(5);
	            __delay_cycles(5000);
	            pwmSetPotencia(45);
	            __delay_cycles(5000);
	            pwmSetPotencia(85);
	            __delay_cycles(5000);
	            pwmSetPotencia(100);

	            __delay_cycles(5000);

	            LigaVentilador;
	                    __delay_cycles(10000);
	                    DeslVentilador;
	                    __delay_cycles(10000);
*/

	}
}

