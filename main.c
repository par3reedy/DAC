//Patrick Reedy; May 3, 2021
 * This assignment's purpose was to use the MSP432P401R to interface with the
 * MCP4921 Digital to Analog Converter (DAC) to be able to output voltages inputted
 * from a keypad.*/

/* Data Sheets for Parts Used
 * MCU:
 * https://www.ti.com/lit/ug/slau356i/slau356i.pdf?ts=1619924832815&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FMSP432P401R
 * DAC: https://ww1.microchip.com/downloads/en/DeviceDoc/22248a.pdf
 * Keypad: https://www.parallax.com/product/4x4-matrix-membrane-keypad/
 */


/*--------------------------------------------------------------------------------------------------------------------------------------
*  Library Definitions
--------------------------------------------------------------------------------------------------------------------------------------*/
#include "msp.h"
#include "dac.h"
#include "DCO.h"
#include "KEYPAD.h"

/*--------------------------------------------------------------------------------------------------------------------------------------
*  Functions Used from other Source Files
--------------------------------------------------------------------------------------------------------------------------------------*/
void DAC_init(void);
void DAC_write(uint16_t data);
uint16_t DAC_volt_conv(uint16_t keypad_volts);
void set_DCO(uint32_t freq);
void keypad_init(void);
char get_key(void);

/*--------------------------------------------------------------------------------------------------------------------------------------
*  Main
--------------------------------------------------------------------------------------------------------------------------------------*/
void main(void)
{
    uint16_t voltage = 0;                                   //Final voltage intialized
    int32_t hundreths, tenths, ones,                        //Variables for input value
            BTNPRESS = BTNMISS;                             //Initialize BTNPRESS to MISS
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             //Stop watchdog timer
    LEDPORT->SEL0 &= ~LED;                                  //Initialize LED for GPIO
    LEDPORT->SEL1 &= ~LED;
    LEDPORT->DIR |= LED;
    keypad_init();                                          //Initialize keypad
    DAC_init();                                             //Initialize DAC
    while(1){
        LEDPORT->OUT &= ~LED;                               //Reset the LED color
        LEDPORT->OUT |= WHITE;                              //Change to LED to WHITE to show waiting for input
        while(BTNPRESS == BTNMISS){                         //Wait until a button is being pressed
            BTNPRESS = get_key();                           //Set BTNPRESS to the current button being pressed
            hundreths = BTNPRESS;                           //Save button pressed to hundreths place
        }
        LEDPORT->OUT &= ~LED;                               //Reset the LED color
        LEDPORT->OUT |= RED;                                //Change the LED color to BLUE for completed hundreths value
        while(BTNPRESS != BTNMISS){                         //Wait until a button is not being pressed
            BTNPRESS = get_key();
        }
        while(BTNPRESS == BTNMISS){                         //Wait until a button is being pressed
            BTNPRESS = get_key();                           //Set BTNPRESS to the current button being pressed
            tenths = BTNPRESS;                              //Save button pressed to tenths place
        }
        LEDPORT->OUT &= ~LED;                               //Reset the LED color
        LEDPORT->OUT |= BLUE;                               //Change the LED color to BLUE for completed tenths value
        while(BTNPRESS != BTNMISS){                         //Wait until a button is not being pressed
            BTNPRESS = get_key();
        }
        while(BTNPRESS == BTNMISS){                         //Wait until a button is being pressed
            BTNPRESS = get_key();                           //Set BTNPRESS to the current button being pressed
            ones = BTNPRESS;                                //Save button pressed to hundreths place
        }
        LEDPORT->OUT &= ~LED;                               //Reset the LED color
        LEDPORT->OUT |= GREEN;                              //Change the LED color to GREEN for completed input
        while(BTNPRESS != BTNMISS){                         //Wait until a button is not being pressed
            BTNPRESS = get_key();
        }
        voltage = (hundreths*100) + (tenths*10) + ones;     //Create decimal value for keypad inputs (3,3,0 -> 330)
        set_DCO(FREQ_24_MHZ);                               //SMCLK is DCOCLK divided by 2 (12 MHz)
        DAC_write(DAC_volt_conv(voltage));                  //Write to DAC the voltage from input converted to DAC input
        __delay_cycles(20000000);                           //Wait so LED can show user the input is finished
    }
}
