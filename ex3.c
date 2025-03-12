/*
 * File:   ex3.c
 * Author: elima
 *
 * Created on March 10, 2025, 9:47 AM
 */


#include "timer.h"
#include "stdbool.h"
#include "xc.h"

int counter = 0;

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(){
    
    IFS0bits.T2IF = 0;
    counter += 1;
    if (counter == 5) {
        LATGbits.LATG9 = 1;
        
    }
    if (counter == 10) {
        LATGbits.LATG9 = 0;
        counter = 0;
    }
}

int main(void) {
    
    INTCON2bits.GIE = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    TRISA = 0x0000;
    TRISG = 0x0000;

    tmr_setup_period(TIMER1, 200);
    tmr_setup_period(TIMER2, 100);
    
    bool toggle = true;
 
    while(true) {
        
        tmr_wait_period(TIMER1); 
        
        LATA = (int) toggle;
 
        toggle = !toggle;
    }
    
    return 0;
}
