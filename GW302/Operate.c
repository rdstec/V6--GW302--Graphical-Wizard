// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    Operate.c
// Description:      Graphic Wizard Operate File
// -----------------------------------------------------------
 

// Include Standard files
#include "Board.h"


// global variables
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : vOperateTask
// Object              : General Operation Code Run Here
// Output Parameters   :
// -----------------------------------------------------------
static void vOperateTask( void *pvParameters )
{

	portTickType xLastWakeTime;

	/*
	AT91C_BASE_PIOA->PIO_CODR = INHIBIT1; // Enable Output1 & 2
	AT91C_BASE_PIOA->PIO_CODR = INHIBIT2; // Enable Output3 & 4
	 */

	// turn on MCM module
	//AT91C_BASE_PIOA->PIO_CODR = OP_NET_ON;

	// wait 3 seconds for MCM to power on
	vTaskDelay( (3000 / portTICK_RATE_MS) );

	// Enable the clock & Start timer0 (need both)
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
	AT91C_BASE_TC0->TC_IER = AT91C_TC_COVFS | AT91C_TC_LDRAS; // enable overflow & capture

	// Enable the clock & Start timer4 (need both)
	AT91C_BASE_TC4->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
	AT91C_BASE_TC4->TC_IER = AT91C_TC_COVFS | AT91C_TC_LDRAS; // enable overflow & capture

	// Enable the clock & Start timer5 (need both)
	AT91C_BASE_TC5->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
	AT91C_BASE_TC5->TC_IER = AT91C_TC_COVFS | AT91C_TC_LDRAS; // enable overflow & capture

	// Enable the clock & Start timer6 (need both)
	AT91C_BASE_TC6->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
	AT91C_BASE_TC6->TC_IER = AT91C_TC_COVFS | AT91C_TC_LDRAS; // enable overflow & capture

	// enable timer interrupts on timer 0/4/5/6
	AT91C_BASE_AIC->AIC_IECR = (0x1 << AT91C_ID_TC0) | (0x1 << AT91C_ID_TC4) | (0x1 << AT91C_ID_TC5) | (0x1 << AT91C_ID_TC6);

	savecal = 0;

	SavecalDelayWithinMenus = 0;
	SavecalDelayOnMainDisplay = 0;


	// Resetting IP4 pulse count for Net applied (in metres)
	NetMetresPulseCount = 0UL; 


	// Initialise for calculation of Analogue Input Rolling Averages
	BaleDiaVoltsTotAccumMultSamples     = 0UL;  // Bale Diameter 
	DensityPresVoltsTotAccumMultSamples = 0UL;  // (Bale) Density Pressure  
	BaleShapeVoltsTotAccumMultSamples   = 0UL;  // Bale Shape Indicator 
	KnifePresVoltsTotAccumMultSamples   = 0UL;  // Knife Pressure

#if 0 // Removed	
	// clear status so mcm will initialize
	for( i = 0; i < MAX_MCM; i++ )
	{
		mcm[i].status = 0;
	}
#endif	

	xLastWakeTime = xTaskGetTickCount();

	while( 1 )
	{
		// every 500ms
		vTaskDelayUntil( &xLastWakeTime, (500 / portTICK_RATE_MS) );

		if( savecal )
		{
			savecal = 0;

			// write factor to flash if needed
			pFac.pf.CheckSum = CheckSumIt( &(pFac.ch[0]), (unsigned char *)&(pFac.pf.CheckSum) );
#ifndef WINDOWS
			// write main factors to SPI1 Flash @ 0x000000
			SPI1FlashWrite( SFC_PW, pFac_BASEADDRESS, &(pFac.ch[0]), sizeof(pFac.pf) );

			// write backup factors to SPI1 Flash @ 0x000000 + 800h
			SPI1FlashWrite( SFC_PW, pFac_BASEADDRESS + 0x800, &(pFac.ch[0]), sizeof(pFac.pf) );
#endif 
		}


		// Every 500 ms

		// Length of Net applied (in metres) for the current IP4 pulse count indicating Net applied (yet to be added to the Total Net Metres (m) factor) 
		NetMetresAppliedForCount = NET_METERS_PER_PULSE * (float)NetMetresPulseCount;

		// Calculating Net Pulse Frequency (Hz)
		CalculateNetPulseFrequency();


	} // end of while( 1 )
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : vStartOperateTask
// -----------------------------------------------------------
void vStartOperateTask( unsigned portBASE_TYPE uxPriority )
{

	/* Spawn the task. */
	xTaskCreate( vOperateTask, "OPERATE", configOPERATE_STACK_SIZE, (void *)NULL, uxPriority, (xTaskHandle *)NULL );
}
// -----------------------------------------------------------

