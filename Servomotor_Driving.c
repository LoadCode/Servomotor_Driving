/*
 * Servomotor_Driving.c
 *
 * Created: 10/05/2015
 *  Author: blogdelingeniero1.wordpress.com
 */ 

/***********************************************************************************/
/* This code describes how to write values in servomotor (Tower Pro Servomotor 9g) */
/* the compare channel has the followin equation:    CCx = (D*PER)/T               */
/* where T is the period of PWM (1/50Hz in this case), D the width of pulse in secs*/
/* In my case, the servomotor has a value of CCA=780 for 0°, CCA=1700 for 90°      */
/* and CCA = 2650 for 180°                                                         */
/***********************************************************************************/

#define F_CPU 16e6
#include <avr/io.h>
#include <util/delay.h>


void PWMInit()
{
	TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc; //Channel A enable and waveform-mode in PWM single-slope
	TCC0.CTRLA = TC_CLKSEL_DIV4_gc; //timer's clock = F_CPU/4 and timer enable (in this case 8MHz)
	TCC0.CTRLC = 0x00;   //CMPx Compare Output Value x off
	TCC0.CTRLD = 0x00;   //All event stuff off
	TCC0.CTRLE = 0x00;   //Byte mode off
	TCC0.PER   = 19999;  //For F_pwm = 50Hz @F_CPU = 16e6, F_per = 4MHz (see xmega AU manual page 175 for equation)	
}

void SetClockTo16MHz()
{
	//Function to change the clock source to a external 16MHz crystal
	
	//frequency range selection and rising time (the slowest)
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL |= OSC_XOSCEN_bm; //Enables the external oscillator
	while(!(OSC.STATUS & OSC_XOSCRDY_bm)); //wait until the oscilator gets a stable frequency
	CCP = CCP_IOREG_gc; //Disables security 4 clock cycles
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc; //changes system's clock multiplexer to external oscillator
}

int main(void)
{
	PORTC.DIR = PIN0_bm; // pin C0 as Output for TCC0.CCA
	SetClockTo16MHz(); 
	PWMInit();
	
    while(1)
    {
		//the following values works for my servo, yours may differ
        TCC0.CCA = 780;  //0°
		_delay_ms(300);
		TCC0.CCA = 1700; //90°
		_delay_ms(300);
		TCC0.CCA = 2650; //180°
		_delay_ms(300);
    }
}