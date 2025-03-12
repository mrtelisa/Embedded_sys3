/*
 * File:   timer.c
 * Author: obe
 *
 * Created on March 3, 2025, 11:03 AM
 */

#include <xc.h>
#include "timer.h"
#define MAX_DELAY 200 //[ms]; max delay i can have (with the max prescaler = 256)

void tmr_setup_period(int timer, int ms) {

    const int clocks = (int) ((float) ms * 281.25);
    
	switch (timer) {
	case TIMER1:
        T1CONbits.TON = 0;  // stopping the timer
		TMR1 = 0;  // resetting the counter
		T1CONbits.TCKPS = 0b11;  // setting the prescaler
		PR1 = clocks;  // loads the maximum number the timer can reach
		IFS0bits.T1IF = 0;  // setting the flag to 0
		T1CONbits.TON = 1;  // re-starting the timer
		break;
        
	case TIMER2:  // same as the other timer
        T2CONbits.TON = 0;
		TMR2 = 0;
		T2CONbits.TCKPS = 0b11;
		PR2 = clocks;
		IFS0bits.T2IF = 0;
		T2CONbits.TON = 1;
		break;
        
	default:
		break;
	}
}

int tmr_wait_period(int timer) {
	
    int ret = 0;
    switch (timer) {
	case TIMER1:
        if(IFS0bits.T1IF == 1) {
            ret = 1;
        } else {
            while (IFS0bits.T1IF == 0) {
                ;
            }
        }
		IFS0bits.T1IF = 0;
        break;
	case TIMER2:
        if(IFS0bits.T2IF == 1) {
            ret = 1;
        } else {
            while (IFS0bits.T2IF == 0) {
                ;
            }
        }
		IFS0bits.T2IF = 0;
        break;
    }

    return ret;
}

void tmr_wait_ms(int timer, int ms){
    int cycles_int = ms / MAX_DELAY;  // num of entire cycles needed
    int rem = ms % MAX_DELAY;  //[ms] in case cycles_int = 0, rem = ms
    
    if(cycles_int > 0) {
        tmr_setup_period(timer, MAX_DELAY);
        for(int i = 0; i < cycles_int; i++) {
            tmr_wait_period(timer);
        }
    }
    
    if(rem > 0) {
        tmr_setup_period(timer, rem);
        tmr_wait_period(timer);
    }
}
