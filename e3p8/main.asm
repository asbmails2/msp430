;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
; Programa 8 - 18/09/2017
;
; Adriano S BrandÃ£o 140033955
; Gustavo Abreu da Cunha140041133
;
; Turma D
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
Label:		bis.b 	#BIT0,&P1DIR 			;LED1 P1.0 = saída
			bis.b 	#BIT7,&P4DIR 			;LED1 P4.7 = saída
			bic.b 	#BIT1,&P2DIR 			;S1 P2.1=entrada
			bis.b 	#BIT1,&P2OUT 			;S1 com pull up/dw
			bis.b 	#BIT1,&P2REN 			;S1 hablita pull up
			bic.b 	#BIT0,&P1OUT 			;LED1 apagado
			bic.b 	#BIT7,&P4OUT 			;LED1 apagado


LA: 		bit.b 	#BIT1,&P2IN 				;ler chave
			bic.b 	#BIT0,&P1OUT 			;LED1 apagado
			jnz 	LA 					;chave aberta
T500ms:		movx.a	#0x20000,R5				;Reseta o Contador
tempo:		decx.a	R5						;Decrementa R15
			jnz		tempo

ZERO:		bic.b 	#BIT0,&P1OUT 			;LED1 apagado
			bic.b 	#BIT7,&P4OUT 			;LED1 apagado
			bit.b 	#BIT1,&P2IN 				;ler chave
			bic.b 	#BIT0,&P1OUT 			;LED1 apagado
			jnz		ZERO
T500msZERO:	movx.a	#0x20000,R5				;Reseta o Contador
tempoZERO:	decx.a	R5						;Decrementa R15
			jnz		tempoZERO

UM:			bis.b	#BIT7,&P4OUT 			;LED1 aceso
 			bit.b 	#BIT1,&P2IN 			;ler chave
			jnz 		UM 					;chave fechada
T500msUM:	movx.a	#0x20000,R5				;Reseta o Contador
tempoUM:		decx.a	R5					;Decrementa R15
			jnz		tempoUM

DOIS:		bis.b	#BIT0,&P1OUT 			;LED2 aceso
			bic.b 	#BIT7,&P4OUT 			;LED1 apagado
 			bit.b 	#BIT1,&P2IN 			;ler chave
			jnz 	DOIS 					;chave fechada
T500msDOIS:	movx.a	#0x20000,R5				;Reseta o Contador
tempoDOIS:	decx.a	R5						;Decrementa R15
			jnz		tempoDOIS

TRES:		bis.b	#BIT0,&P1OUT 			;LED2 aceso
			bis.b 	#BIT7,&P4OUT 			;LED1 aceso
 			bit.b 	#BIT1,&P2IN 				;ler chave
			jnz 	TRES 					;chave fechada
T500msTRES:	movx.a	#0x20000,R5				;Reseta o Contador
tempoTRES:	decx.a	R5						;Decrementa R15
			jnz		tempoTRES
			JMP		ZERO
                                            

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
