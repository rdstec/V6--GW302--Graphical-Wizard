// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    main.c
// Description:      Graphic Wizard Main Files
// -----------------------------------------------------------
 

// Include Standard files
#include "Board.h"

#include "stdio.h"


// functions

// -----------------------------------------------------------
// Function Name       : Main
// Object              : Software entry point
// Input Parameters    : none.
// Output Parameters   : none.
// -----------------------------------------------------------
int main( void )
{
	
// turn off wakeup on WKUP1/0
AT91C_BASE_SHDWC->SHDWC_SHMR = AT91C_SHDWC_WKMODE1_NONE | AT91C_SHDWC_WKMODE0_NONE;

// set maximum reset time (0xf << 8) & enable user reset pin -> not working as expected, only extends reset input
// AT91C_BASE_RSTC->RSTC_RMR = RESET_KEY | (0xf << 8) | AT91C_RSTC_URSTEN;
// Enable User Reset and set its minimal assertion to 0.5 seconds
AT91F_RSTSetMode( AT91C_BASE_RSTC, AT91C_RSTC_URSTEN | (0x4 << 8) ); // 0.5 sec == 2^(13 + 1) / 32768hz

// Enable the clock of the PIOA
AT91F_PIOA_CfgPMC();

// configure the PIO Lines corresponding to OUT1 to OUT6 to be outputs
AT91C_BASE_PIOA->PIO_PER = TESTPIN | OUTPUT_MASK_PIOA; // Set in PIO mode
AT91C_BASE_PIOA->PIO_OER = TESTPIN | OUTPUT_MASK_PIOA; // Configure as Output
AT91C_BASE_PIOA->PIO_SODR = OUTPUT_MASK_PIOA; // Set Outputs - OFF if set
/* turn on OP6 - 12V supply on the machine */
AT91C_BASE_PIOA->PIO_CODR = OUT6; // Set Outputs - ON if clear
/* turn on key illumination, clear test pin  */
AT91C_BASE_PIOA->PIO_CODR = TESTPIN | ILLUMINATION; // Set Outputs - ON if clear


// pull up disable
AT91C_BASE_PIOA->PIO_PPUDR = PIOA_PULLUP_OFF; 
  
// Enable the clock of the PIOB
AT91F_PIOB_CfgPMC();
AT91C_BASE_PIOB->PIO_PER = OUTPUT_MASK_PIOB; // Set in PIO mode
AT91C_BASE_PIOB->PIO_OER = OUTPUT_MASK_PIOB; // Configure as Output
AT91C_BASE_PIOB->PIO_SODR = OUTPUT_MASK_PIOB; // Set Outputs - OFF if set
// pull up disable
AT91C_BASE_PIOB->PIO_PPUDR = PIOB_PULLUP_OFF;

AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_PIOB, INTERRUPT_PRIORITY7, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, PIOB_irq_handler );
AT91C_BASE_PIOB->PIO_IER = AT91C_PIO_PB0; // enable interrupt - PFAIL, controller enabled later

// enable PWM on PWM0 to 7
// enable clock
AT91F_PWMC_CfgPMC();
// all PWM setup and started by SetOutputPWM()
SetOutputPWM( BACK, 0, 0 ); // start disabled and off (bootloader may leave it running!)
 
// setup AtoD
// enable clock
AT91F_ADC0_CfgPMC();
// set timings, sets mode register _MR
// clears TRGEN for software conversions only, and clears LOWRES for 10bit conversion
// enable AtoD on ADC0_AD2 & ADC0_AD3
AT91F_ADC_CfgTimings( AT91C_BASE_ADC0, 48, 6, 500, 5000 );
// CfgTimings leaves trigger bits at 0, the following selects timer1 TIOA as hardware trigger
AT91C_BASE_ADC0->ADC_MR |= (AT91C_ADC_TRGEN_EN | AT91C_ADC_TRGSEL_TIOA1 | AT91C_ADC_SLEEP_MODE);
//AT91F_ADC_CfgModeReg( AT91C_BASE_ADC0, ??? );
AT91F_ADC_EnableChannel( AT91C_BASE_ADC0, ATOD_CHANNELS );
// set start bit in control register _CR
//AT91F_ADC_StartConversion( AT91C_BASE_ADC0 );

// as timer0 is used to start atod, we'll enable atod interrupt before starting timer
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_ADC0, INTERRUPT_PRIORITY7, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, ADC0_irq_handler );
AT91C_BASE_ADC0->ADC_IER = AT91C_ADC_EOC7; // enable 'end of conversion on channel 7' interrupt
// atod interrupt enabled in Power.c

// time clock divider defines
// AT91C_TC_CLKS_TIMER_DIV1_CLOCK = MCK / 2 = 47923200 / 2 
// AT91C_TC_CLKS_TIMER_DIV2_CLOCK = MCK / 8 = 47923200 / 8
// AT91C_TC_CLKS_TIMER_DIV3_CLOCK = MCK / 32 = 47923200 / 32
// AT91C_TC_CLKS_TIMER_DIV4_CLOCK = MCK / 128 = 47923200 / 128
// AT91C_TC_CLKS_TIMER_DIV5_CLOCK = MCK / 1024 = 47923200 / 1024

// --- timer0 setup for capture on TIOA0 --
AT91F_TC0_CfgPMC();
// Set the Mode of the Timer Counter, MCK/128,
AT91C_BASE_TC0->TC_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK | AT91C_TC_LDRA_RISING | AT91C_TC_LDRB_FALLING;
// timer started in operate.c

// --- timer 0 interrupt setup ---
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_TC0, INTERRUPT_PRIORITY1, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, TC0_irq_handler );
// interrupt enabled in operate.c

// --- timer1 setup ---
// timer1 starts atod sampling on rising edge of TIOA output
AT91F_TC1_CfgPMC();
//* Set the Mode of the Timer Counter - toggle TIOA0 at 1000Hz
AT91C_BASE_TC1->TC_CMR = AT91C_TC_CLKS_TIMER_DIV2_CLOCK | AT91C_TC_WAVE | AT91C_TC_WAVESEL_UP_AUTO |
  AT91C_TC_ACPA_CLEAR | AT91C_TC_ACPC_SET;
AT91C_BASE_TC1->TC_RA = 1000;
AT91C_BASE_TC1->TC_RC = 5990; // 47923200 / 8 / 5990 == 1000.0668
// timer started in power.c

// --- timer1 interrupt setup ---
TC1_interrupt_count = 0;
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_TC1, INTERRUPT_PRIORITY1, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, TC1_irq_handler );
AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS; // enable 'RC compare' interrupt
AT91F_AIC_EnableIt( AT91C_BASE_AIC, AT91C_ID_TC1 );

// --- timer4 setup for capture on TIOA4 --
AT91F_TC4_CfgPMC();
// Set the Mode of the Timer Counter, MCK/128,
AT91C_BASE_TC4->TC_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK | AT91C_TC_LDRA_RISING | AT91C_TC_LDRB_FALLING; 
// timer started in operate.c

// --- timer 4 interrupt setup ---
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_TC4, INTERRUPT_PRIORITY1, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, TC4_irq_handler );
// interrupt enabled in operate.c

// --- timer5 setup for capture on TIOA5 --
AT91F_TC5_CfgPMC();
// Set the Mode of the Timer Counter, MCK/128,
AT91C_BASE_TC5->TC_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK | AT91C_TC_LDRA_RISING | AT91C_TC_LDRB_FALLING; 
// timer started in operate.c

// --- timer 5 interrupt setup ---
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_TC5, INTERRUPT_PRIORITY1, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, TC5_irq_handler );
// interrupt enabled in operate.c

// --- timer6 setup for capture on TIOA6 --
AT91F_TC6_CfgPMC();
// Set the Mode of the Timer Counter, MCK/128,
AT91C_BASE_TC6->TC_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK | AT91C_TC_LDRA_RISING | AT91C_TC_LDRB_FALLING; 
// timer started in operate.c

// --- timer 6 interrupt setup ---
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_TC6, INTERRUPT_PRIORITY1, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, TC6_irq_handler );
// interrupt enabled in operate.c

// setup TWI (IIC) - runs in display loop
// enable clock
AT91F_TWI_CfgPMC();
// set clock divider CKDIV = 0, CHDIV = 57, CLDIV = 57 for 399.36 khz at MCK of 47923200
AT91C_BASE_TWI->TWI_CWGR = (0 << 16) | (57 << 8) | (57 << 0);
//AT91C_BASE_TWI->TWI_CWGR = (0 << 16) | (93 << 8) | (93 << 0); // 249.6 khz

// setup SPI1
SPI1Setup();

// setup CAN - taken care of in task setup
//InitialiseCAN();

// enable shutdown peripheral - has no effect as is always clocked by slow clock
//AT91F_SHDWC_CfgPMC();

AT91F_PIO_CfgPeriph( AT91C_BASE_PIOA, // PIO controller base address
  // Peripheral A
  ((unsigned int) AT91C_PA0_TWD) |
  ((unsigned int) AT91C_PA1_TWCK) |
  ((unsigned int) AT91C_PA2_RXD0) |
  ((unsigned int) AT91C_PA3_TXD0) |
  ((unsigned int) AT91C_PA11_SPI0_NPCS0) |
  ((unsigned int) AT91C_PA12_SPI0_NPCS1) |
  ((unsigned int) AT91C_PA16_SPI0_MOSI) |
  ((unsigned int) AT91C_PA17_SPI0_SPCK) |
  /*((unsigned int) AT91C_PA18_PWM0) |
  ((unsigned int) AT91C_PA19_PWM1) |
  ((unsigned int) AT91C_PA20_PWM2) |
  ((unsigned int) AT91C_PA21_PWM3) |  PWM set by SetOutputPWM()
  ((unsigned int) AT91C_PA22_PWM4) |
  ((unsigned int) AT91C_PA23_PWM5) |
  ((unsigned int) AT91C_PA24_PWM6) |
  ((unsigned int) AT91C_PA25_PWM7) |*/
  /*((unsigned int) AT91C_PA26_CANRX0) |
  ((unsigned int) AT91C_PA27_CANTX0) |*/
  /*((unsigned int) AT91C_PA28_CANRX1) | 		
  ((unsigned int) AT91C_PA29_CANTX1) |*/
  ((unsigned int) AT91C_PA30_DRXD) |
  ((unsigned int) AT91C_PA31_DTXD),
  // Peripheral B
  ((unsigned int) AT91C_PA4_SPI1_NPCS0) |
  ((unsigned int) AT91C_PA5_SPI1_NPCS1) |
  /*((unsigned int) AT91C_PA6_SPI1_NPCS2) |*/
  ((unsigned int) AT91C_PA8_SPI1_MISO) |
  ((unsigned int) AT91C_PA9_SPI1_MOSI) |
  ((unsigned int) AT91C_PA10_SPI1_SPCK)
  );

#if 0 // test 
//AT91F_PIO_CfgOutput( AT91C_BASE_PIOB, AT91C_PB21_TIOB4 );
//AT91F_PIO_ClearOutput( AT91C_BASE_PIOB, AT91C_PB21_TIOB4 );
//AT91F_PIO_CfgDirectDrive( AT91C_BASE_PIOB, AT91C_PB21_TIOB4 );
#endif

AT91F_PIO_CfgPeriph( AT91C_BASE_PIOB, // PIO controller base address
  // Peripheral A
  0,
  /*((unsigned int) AT91C_PB22_TIOA5),*/
  // Peripheral B
  0
  );
  
// disable USB transceiver - may not be necessary for SAM7A3
// enable peripheral clock of UDP
AT91C_BASE_PMC->PMC_PCER = ((unsigned int)1 << AT91C_ID_UDP); // AT91F_UDP_CfgPMC();
// disable transceiver
AT91C_BASE_UDP->UDP_TXVC = AT91C_UDP_TXVDIS; // AT91F_UDP_DisableTransceiver( AT91C_BASE_UDP );
// turn off peripheral clock for UDP
AT91C_BASE_PMC->PMC_PCDR = ((unsigned int)1 << AT91C_ID_UDP); // AT91F_PMC_DisablePeriphClock( AT91C_BASE_UDP, (unsigned int) 1 << AT91C_ID_UDP) );
  
/* Start FreeRTOS application tasks */
vStartPowerTask( mainPOWER_TASK_PRIORITY );
vStartTWITask( mainTWI_TASK_READ_PRIORITY );
vStartComTasks( mainCOM_TEST_PRIORITY, 9600 );
//vStartCANTasks( mainCAN_TEST_PRIORITY ); // Removed
vStartDisplayTask( mainDISPLAY_TASK_PRIORITY );
vStartOperateTask( mainOPERATE_TASK_PRIORITY );

/* 
NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
The processor MUST be in supervisor mode when vTaskStartScheduler is 
called.  The demo applications included in the FreeRTOS.org download switch
to supervisor mode prior to main being called.  If you are not using one of
these demo application projects then ensure Supervisor mode is used here.
*/

/* Now all the tasks have been started - start the scheduler. */
vTaskStartScheduler();

return( 0 );
  
} // End main()
// -----------------------------------------------------------

