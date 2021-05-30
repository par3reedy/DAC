//Patrick Reedy; May 3, 2021

#include "msp.h"
#include "DCO.h"
#include <math.h>

void set_DCO(uint32_t freq){
    CS->KEY = CS_KEY_VAL;                                                   //Unlock the CS registers
    CS->CTL0 = 0;                                                           //Clear the CTL0 register
    CS->CTL0 = freq;                                                        //Set DCO to desired Frequency
    CS->CTL1 = 0;
    CS->CTL1 = (CS_CTL1_SELS__DCOCLK
              | CS_CTL1_DIVS__2
              | CS_CTL1_SELM__DCOCLK
              | CS_CTL1_DIVM__1
              | CS_CTL1_DIVHS__2);
    CS->KEY = 0;                                                            //Lock the CS registers
}


