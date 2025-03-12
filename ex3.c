/*
 * File:   ex3.c
 * Author: elima
 *
 * Created on March 10, 2025, 9:47 AM
 */


#include "timer.h"
#include "stdbool.h"
#include "xc.h"

bool blink_enable = false;

void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(){
    
    IFS1bits.INT1IF = 0;
    blink_enable = !blink_enable;
    
}

int main(void) {
    
    TRISE = 0xFFFF;
    
    RPINR0bits.INT1R = 0x59; // remapping the interrupt to the specific pin
    
    IFS1bits.INT1IF = 0; // flag of the interrupt set to 0
    INTCON2bits.GIE = 1; // global enabling 
    IEC1bits.INT1IE = 1; // enabling the interrupt
    
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    TRISG = 0x0000;

    tmr_setup_period(TIMER1, 200);
    
    bool toggle = true;
    
    while(true) {
        if(blink_enable){            
            tmr_wait_period(TIMER1); 
            LATGbits.LATG9 = (int) toggle;
            toggle = !toggle;            
        } else {
            LATGbits.LATG9 = 0x0000;
        }
    }
    
    return 0;
}
