/*
 * atrasar.c
 */

#include <projetofinal.h>

void atrasar (unsigned long atrasa){
   volatile unsigned long i = atrasa;
   while(i != 0)
            i--;
}





