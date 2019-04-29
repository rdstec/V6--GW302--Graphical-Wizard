// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    interrupts.c
// Description:      Graphic Wizard Interrupts File
// -----------------------------------------------------------


// Include Standard files
#include "Board.h"

#include <stdlib.h>


// functions

// -----------------------------------------------------------
// Function Name       : TC0_irq_handler
// Object              : C handler interrupt function
// Output Parameters   : increment timer0_interrupt_count
// -----------------------------------------------------------
void TC0_irq_handler( void )
{
	static unsigned short int time, count;
	static unsigned int stime, tlast;
	static unsigned char fnow;

	register int status, ctime, ltime;

	// Acknowledge interrupt status by reading register
	status = AT91C_BASE_TC0->TC_SR;

	if( status & AT91C_TC_COVFS ) // timer overflow MCK/128
	{
		fnow = 0;
		time++;
		ctime = (time * 0x10000UL) + AT91C_BASE_TC0->TC_CV;
		ltime = ctime - stime;
		if( ltime > 1872000UL ) // MCK/128 - 5.00 seconds zero time
		{
			f0time = ltime;	
			stime = ctime;
			f0count = count;
			count = 0;
		}
		else if( ltime > 374400UL ) // MCK/128 - 1.00 seconds now time
		{
			if( count )
			{
				fnow = 1;
			}	
		}
	}

	if( status & AT91C_TC_LDRAS ) // capture
	{
		// debounce of 500us with MCK/128 (187 * 128/MCK Seconds) for 2KHz limit
		// but use 180 so we can actually measure 2000Hz
		if( (AT91C_BASE_TC0->TC_RA - tlast) > 180UL )
		{ 	
			count++;
			if( fnow )
			{
				fnow = 0;
				ctime = (time * 0x10000UL) + AT91C_BASE_TC0->TC_RA;

				f0time = ctime - stime;
				stime = ctime;
				f0count = count;
				count = 0;
			}
		}
		tlast = AT91C_BASE_TC0->TC_RA;
	}

	// reset interrupt, any value can be written so use peripheral ID
	AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_TC0;
} 
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : TC1_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
void TC1_irq_handler( void )
{
	//static unsigned char off_length;
	register unsigned int i;

	TC1_interrupt_count++;
	
	if( TC1_count0 )
	{
		TC1_count0--;
	}
	else
	{
		TC1_count0 = 99; // 10Hz or 100 msec
		AllTime++; // Incremented every 100 msec
	}

	for( i = 0; i < MAX_MCM; i++ )
	{
		if( mcm[i].timeout < 10000 ) // incremented every millisecond.
		{
			mcm[i].timeout++;
		}
	}
	
	if( buzz_length )
	{
		buzz_length--;
		AT91C_BASE_PIOA->PIO_CODR = BUZZER; /* buzzer ON */
	}
	else if( off_length )
	{
		off_length--;
		AT91C_BASE_PIOA->PIO_SODR = BUZZER; /* buzzer OFF */
	}                               
	else if( buzz_count )
	{
		buzz_count--;
		buzz_length = BEEP_ON;
		off_length = BEEP_OFF;
	}
	else
	{
		AT91C_BASE_PIOA->PIO_SODR = BUZZER;
	}

	// Acknowledge interrupt status by reading register
	i = AT91C_BASE_TC1->TC_SR;

	// reset interrupt, any value can be written so use peripheral ID
	AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_TC1;
} 
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : TC4_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
void TC4_irq_handler( void )
{
	static unsigned int tlast;

	register int status;

	// Acknowledge interrupt status by reading register
	status = AT91C_BASE_TC4->TC_SR;

	if( status & AT91C_TC_COVFS ) // timer overflow MCK/128/65536
	{
		if( tl_confimation_delay[0] < 255 )
		{
			tl_confimation_delay[0]++;
		}
	}

	if( status & AT91C_TC_LDRAS ) // capture
	{
		// debounce of 500us with MCK/128 (187 * 128/MCK Seconds) for 2KHz limit
		// but use 180 so we can actually measure 2000Hz
		if( (AT91C_BASE_TC4->TC_RA - tlast) > 180UL )
		{
			if( tl_target_count[0] < 255 )
			{
				tl_target_count[0]++;
			}
		}
		tlast = AT91C_BASE_TC4->TC_RA;
	}

	// reset interrupt, any value can be written so use peripheral ID
	AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_TC4;
} 
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : TC5_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
void TC5_irq_handler( void )
{
	static unsigned int tlast;

	register int status;

	// Acknowledge interrupt status by reading register
	status = AT91C_BASE_TC5->TC_SR;

	if( status & AT91C_TC_COVFS ) // timer overflow MCK/128
	{
		if( tl_confimation_delay[1] < 255 )
		{
			tl_confimation_delay[1]++;
		}
	}

	if( status & AT91C_TC_LDRAS ) // capture
	{
		// debounce of 500us with MCK/128 (187 * 128/MCK Seconds) for 2KHz limit
		// but use 180 so we can actually measure 2000Hz
		if( (AT91C_BASE_TC5->TC_RA - tlast) > 180UL )
		{
			if( tl_target_count[1] < 255 )
			{
				tl_target_count[1]++;
			}
		}
		tlast = AT91C_BASE_TC5->TC_RA;
	}

	// reset interrupt, any value can be written so use peripheral ID
	AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_TC5;
} 
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : TC6_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
void TC6_irq_handler( void )
{
	static unsigned short int time, count;	
	static unsigned int stime, tlast;
	static unsigned char fnow;

	register int status, ctime, ltime;

	// Acknowledge interrupt status by reading register
	status = AT91C_BASE_TC6->TC_SR;

	if( status & AT91C_TC_COVFS ) // timer overflow MCK/128
	{
		fnow = 0;

		time++;
		ctime = (time * 0x10000UL) + AT91C_BASE_TC6->TC_CV;
		ltime = ctime - stime;
		if( ltime > 44928UL ) // MCK/128 - 0.12 seconds zero time
		{
			f6time = ltime;	
			stime = ctime;
			f6count = count;
			NetPulseCount += count;
			BeakEngagePulseCount += count;
			NetMetresPulseCount += count; 
			count = 0;
			
			newf6 = 1;
		}
		else if( ltime > 14976UL ) // MCK/128 - 0.04 seconds now time
		{
			if( count )
			{
				fnow = 1;
			}	
		}
	}

	if( status & AT91C_TC_LDRAS ) // capture
	{
		// debounce of 500us with MCK/128 (187 * 128/MCK Seconds) for 2KHz limit
		// but use 180 so we can actually measure 2000Hz
		if( (AT91C_BASE_TC6->TC_RA - tlast) > 180UL )
		{ 	
			count++;
			if( fnow )
			{
				fnow = 0;
				ctime = (time * 0x10000UL) + AT91C_BASE_TC6->TC_RA;

				f6time = ctime - stime;	
				stime = ctime;
				f6count = count;
				NetPulseCount += count; 
				BeakEngagePulseCount += count;
				NetMetresPulseCount += count;
				count = 0;
				
				newf6 = 1; 
			}
		}
		tlast = AT91C_BASE_TC6->TC_RA;
	}

	// reset interrupt, any value can be written so use peripheral ID
	AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_TC6;
} 
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : PIOB_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
/* The interrupt is defined as "naked" as the full context is saved on entry as part of the context switch. */
void PIOB_irq_handler( void ) __attribute__((naked));
/* function is ordinary c routine */
void PIOB_irq_function( void );

void PIOB_irq_handler( void )
{
    /* Save the context of the interrupted task. */
    portSAVE_CONTEXT();
    
    /* Call the handler function.  This must be a separate 
    function unless you can guarantee that handling the 
    interrupt will never use any stack space. */
    PIOB_irq_function();

    /* Restore the context of the task that is going to 
    execute next. This might not be the same as the originally 
    interrupted task.*/
    portRESTORE_CONTEXT();
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : PIOB_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
void PIOB_irq_function( void )
{
register unsigned int status;

portBASE_TYPE SwitchRequired = pdFALSE;

// reading interrupt status register clears all pending interrupt flags
status = AT91C_BASE_PIOB->PIO_ISR;

// interrupting /*& pin low*/
if( status & AT91C_PIO_PB0 )/* && !((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB0)  )*/
{
	// Resume Power task - save factors and turn off
    PowerSwitchedOff = 0;
	SwitchRequired = xTaskResumeFromISR( POWERtaskhandle );
}

if( SwitchRequired )                                       \
{
	portYIELD_FROM_ISR();
}

// reset interrupt, any value can be written so use peripheral ID
AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_PIOB;
} 
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : ADC0_irq_handler
// Object              : C handler interrupt function
// -----------------------------------------------------------
/* The interrupt is defined as "naked" as the full context is saved on entry as part of the context switch. */
void ADC0_irq_handler( void ) __attribute__((naked));

/* function is ordinary c routine */
void ADC0_irq_function( void );

void ADC0_irq_handler( void )
{
    /* Save the context of the interrupted task. */
    portSAVE_CONTEXT();
    
    /* Call the handler function.  This must be a separate 
    function unless you can guarantee that handling the 
    interrupt will never use any stack space. */
    ADC0_irq_function();

    /* Restore the context of the task that is going to 
    execute next. This might not be the same as the originally 
    interrupted task.*/
    portRESTORE_CONTEXT();
}

void ADC0_irq_function( void )
{
static unsigned char Vcount;

static unsigned char SampleCount;

static unsigned long int BaleDiaVoltsAccumMultSamples; 
static unsigned long int DensityPresVoltsAccumMultSamples; 
static unsigned long int BaleShapeVoltsAccumMultSamples;
static unsigned long int KnifePresVoltsAccumMultSamples;

register unsigned long int BaleDiaVoltsMultSample;          
register unsigned long int DensityPresVoltsMultSample;       
register unsigned long int BaleShapeVoltsMultSample;  
register unsigned long int KnifePresVoltsMultSample;         

register unsigned int status;
portBASE_TYPE SwitchRequired = pdFALSE;

EOC7_interrupt_count++;

// reading conversion register clears eoc flag
status = AT91C_BASE_ADC0->ADC_CDR7;

if( status < (unsigned int)ATODVMIN )
  {
  Vcount++;
  if( Vcount > 2 )
    {
    // Resume Power task - save factors and turn off
    PowerSwitchedOff = 0;
    SwitchRequired = xTaskResumeFromISR( POWERtaskhandle );
    }	
  }
else if( Vcount )
  {
  Vcount--;	
  }

// Reading conversion register clears eoc flag

// AnIP1: Bale Diameter (read value)
BaleDiaVoltsMultSample     = (unsigned long int)( AT91C_BASE_ADC0->ADC_CDR0 );

// AnIP2: (Bale) Density Pressure (read value)
DensityPresVoltsMultSample = (unsigned long int)( AT91C_BASE_ADC0->ADC_CDR1 );

// AnIP3: Bale Shape Indicator (read value)
BaleShapeVoltsMultSample   = (unsigned long int)( AT91C_BASE_ADC0->ADC_CDR2 );

// AnIP4: Knife Pressure (read value)
KnifePresVoltsMultSample   = (unsigned long int)( AT91C_BASE_ADC0->ADC_CDR3 );

if( SampleCount )
{
	SampleCount--;

	// Accumulating samples (each multiplied by INPUT_ACCURACY_MULTIPLIER) 
	BaleDiaVoltsAccumMultSamples     += BaleDiaVoltsMultSample;
	DensityPresVoltsAccumMultSamples += DensityPresVoltsMultSample;
	BaleShapeVoltsAccumMultSamples   += BaleShapeVoltsMultSample;
	KnifePresVoltsAccumMultSamples   += KnifePresVoltsMultSample;
}
else // SampleCount == 0 
{
	// Saving total accumulated (multiplied) samples -- samples collected in NUM_ACCUMULATED_SAMPLES x 1 ms
	BaleDiaVoltsTotAccumMultSamples     = BaleDiaVoltsAccumMultSamples; 
	DensityPresVoltsTotAccumMultSamples = DensityPresVoltsAccumMultSamples; 
	BaleShapeVoltsTotAccumMultSamples   = BaleShapeVoltsAccumMultSamples;
	KnifePresVoltsTotAccumMultSamples   = KnifePresVoltsAccumMultSamples;
	
	// Start accumulating next group of samples -- first sample collected on SampleCount==0 
	BaleDiaVoltsAccumMultSamples     = BaleDiaVoltsMultSample;
	DensityPresVoltsAccumMultSamples = DensityPresVoltsMultSample; 
	BaleShapeVoltsAccumMultSamples   = BaleShapeVoltsMultSample;
	KnifePresVoltsAccumMultSamples   = KnifePresVoltsMultSample;

	SampleCount = (NUM_ACCUMULATED_SAMPLES - 1); 
}


// clear overrun errors
status = AT91C_BASE_ADC0->ADC_SR;

if( SwitchRequired )                                          \
{
	portYIELD_FROM_ISR();
}

// reset interrupt, any value can be written so use peripheral ID
AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_ADC0;
} 
// -----------------------------------------------------------


#if 0 // Removed
// -----------------------------------------------------------
// Function Name       : CAN0_irq_handler
// Object              : C handler interrupt function
// checks CAN0 peripheral for received data, and queues data
// -----------------------------------------------------------
/* The interrupt is defined as "naked" as the full context is saved on entry as part of the context switch. */
void CAN0_irq_handler( void ) __attribute__((naked));

/* function is ordinary c routine */
void CAN0_irq_function( void );

void CAN0_irq_handler( void )
{
    /* Save the context of the interrupted task. */
    portSAVE_CONTEXT();
    
    /* Call the handler function.  This must be a separate 
    function unless you can guarantee that handling the 
    interrupt will never use any stack space. */
    CAN0_irq_function();

    /* Restore the context of the task that is going to 
    execute next. This might not be the same as the originally 
    interrupted task.*/
    portRESTORE_CONTEXT();
}

void CAN0_irq_function( void )
{
register unsigned int cStatus, mStatus;
portBASE_TYPE SwitchRequired = pdFALSE;

// must align word data for interrupt else we get a prefetch abort
// struct CanMessage CanM __attribute__ ((aligned (16)));
struct CanMessage CanM;

// reading status clears it, so it is saved to local variable for later use
cStatus = AT91C_BASE_CAN0->CAN_SR;

// message box 1 interrupt
if( cStatus & AT91C_CAN_MB1 )
  {
  // new message received - only MCM in message box 1
  mStatus =	AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MSR;
  if( mStatus & AT91C_CAN_MRDY )
    {
    // get message from CAN0 peripheral
    CanM.Id = AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MID; // Message ID
    CanM.Data.l[0] = AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MDL; // Message Data Low 		
    CanM.Data.l[1] = AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MDH; // Message Data High 		
    
    // set to receive next message, clears MRDY & MABT 
    AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MCR = AT91C_CAN_MTCR;

    // post the CAN message message to queue    
    xQueueSendFromISR( RxCAN_MCM, (void *)&CanM, &SwitchRequired );
    }
  }

if( SwitchRequired )                                          \
{
	portYIELD_FROM_ISR();
}

// reset interrupt, any value can be written so use peripheral ID
AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_CAN0;

}
// -----------------------------------------------------------
#endif


// -----------------------------------------------------------
// Function Name       : debug_serial_setup
// -----------------------------------------------------------
void debug_serial_setup( void )
{
// Enable clock - system peripherals are always on
// AT91F_DBGU_CfgPMC();

// select DEBUG, PIOA pin 30 & 31 to peripheral A
AT91C_BASE_PIOA->PIO_ASR = AT91C_PA30_DRXD | AT91C_PA31_DTXD; 
// disable PIO control of pin 30 & 31 (enabling peripheral control)
AT91C_BASE_PIOA->PIO_PDR = AT91C_PA30_DRXD | AT91C_PA31_DTXD;

// Disable all USART0 interrupt sources to begin... */
AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;

// Disable Debug Interrupts	
AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_RXRDY | AT91C_US_TXRDY;

// Reset various status bits (just in case)...
AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS | AT91C_US_RSTSTA;

// set the baud rate divisor register
 AT91C_BASE_DBGU->DBGU_BRGR = BAUD9600;
/*AT91C_BASE_DBGU->DBGU_BRGR = BAUD115200;*/

// enable transmitter
AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;

while( !(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) )
  {
  }
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : debug_message_wait
// -----------------------------------------------------------
void debug_message_wait( void )
{

while( !(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) )
  {
  }
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : debug_message
// -----------------------------------------------------------
void debug_message( unsigned char *pt )
{

while( *pt )
  {
  AT91C_BASE_DBGU->DBGU_THR = *pt++;
  while( !(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) )
    {
    }
  }
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : IntToString
// 32 bit int to hex string 0x00000000 format
// -----------------------------------------------------------
unsigned char *IntToString( unsigned long int i )
{
unsigned char *pt;

pt = (unsigned char *)buffer + 10;
*pt = 0x00; // 10
pt--;
*pt = binHex[i % 0x10]; // 9
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 8
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 7
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 6
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 5
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 4
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 3
i = i / 0x10;
pt--;
*pt = binHex[i % 0x10]; // 2
pt--;
*pt = 'x'; // 1
pt--;
*pt = '0'; // 0

return( pt );

/*
buffer[10] = 0x00;
buffer[9] = binHex[i % 0x10];
i = i / 0x10;
buffer[8] = binHex[i % 0x10];
i = i / 0x10;
buffer[7] = binHex[i % 0x10];
i = i / 0x10;
buffer[6] = binHex[i % 0x10];
i = i / 0x10;
buffer[5] = binHex[i % 0x10];
i = i / 0x10;
buffer[4] = binHex[i % 0x10];
i = i / 0x10;
buffer[3] = binHex[i % 0x10];
i = i / 0x10;
buffer[2] = binHex[i % 0x10];
buffer[1] = 'x';
buffer[0] = '0';

return( &buffer[0] );
*/
}
/*-----------------------------------------------------------*/


unsigned char const * const DebugMessage[6] = {
 "\r\nUndefined IRQ",
 "\r\nPrefetch  IRQ",
 "\r\nReserved  IRQ",
 "\r\nDataAbort IRQ",
 "\r\nDefault   FIQ",
 "\r\nDefault   IRQ"
 };

#define DEBUG_UNDEFINED_INSTRUCTION_MESSAGE 0
#define DEBUG_PREFETCH_ABORT_MESSAGE        1
#define DEBUG_RESERVED_IRQ_MESSAGE          2
#define DEBUG_DATA_ABORT_MESSAGE            3
#define DEBUG_FIQ_MESSAGE                   4
#define DEBUG_IRQ_MESSAGE                   5


// -----------------------------------------------------------
// Function Name       : DebugAbortMessage
// -----------------------------------------------------------
void DebugAbortMessage( unsigned char messageID, int *lnk_ptr )
{

// done in main, as is always debug port
//debug_serial_setup();

debug_message( "\r\n" );

debug_message( (unsigned char *)DebugMessage[messageID] );

debug_message( ", Add: " );
debug_message( IntToString( AT91C_BASE_MC->MC_AASR ) );

debug_message( " Sta: " );
debug_message( IntToString( AT91C_BASE_MC->MC_ASR ) );

debug_message( ", LP " );
debug_message( IntToString( (unsigned long int)lnk_ptr ) );

debug_message( " (" );
debug_message( IntToString( (unsigned long int)(*lnk_ptr) ) );

debug_message( ")\r\n" );

debug_message_wait(); // wait for com port ready

//vTaskList( (unsigned char *)buffer );
//vTaskSuspendAll(); // stop task switching
//debug_message( (unsigned char *)buffer );
//debug_message_wait();

// Restart
// AT91C_BASE_RSTC->RSTC_RCR = (0xA5000000 | AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_EXTRST);

// loops forever on Arm
exit( 1 );

/*
AT91C_MC_UNDADD       ((unsigned int) 0x1 <<  0) // (MC) Undefined Address Abort Status
AT91C_MC_MISADD       ((unsigned int) 0x1 <<  1) // (MC) Misaligned Address Abort Status
AT91C_MC_MPU          ((unsigned int) 0x1 <<  2) // (MC) Memory protection Unit Abort Status
AT91C_MC_ABTSZ        ((unsigned int) 0x3 <<  8) // (MC) Abort Size Status
	AT91C_MC_ABTSZ_BYTE                 ((unsigned int) 0x0 <<  8) // (MC) Byte
	AT91C_MC_ABTSZ_HWORD                ((unsigned int) 0x1 <<  8) // (MC) Half-word
	AT91C_MC_ABTSZ_WORD                 ((unsigned int) 0x2 <<  8) // (MC) Word
AT91C_MC_ABTTYP       ((unsigned int) 0x3 << 10) // (MC) Abort Type Status
	AT91C_MC_ABTTYP_DATAR                ((unsigned int) 0x0 << 10) // (MC) Data Read
	AT91C_MC_ABTTYP_DATAW                ((unsigned int) 0x1 << 10) // (MC) Data Write
	AT91C_MC_ABTTYP_FETCH                ((unsigned int) 0x2 << 10) // (MC) Code Fetch
AT91C_MC_MST0         ((unsigned int) 0x1 << 16) // (MC) Master 0 Abort Source
AT91C_MC_MST1         ((unsigned int) 0x1 << 17) // (MC) Master 1 Abort Source
AT91C_MC_SVMST0       ((unsigned int) 0x1 << 24) // (MC) Saved Master 0 Abort Source
AT91C_MC_SVMST1       ((unsigned int) 0x1 << 25) // (MC) Saved Master 1 Abort Source
*/

}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Undefined_Instruction_handler
// Undefined Interrupt will send Debug Message then Restart
// -----------------------------------------------------------
/* void AT91F_Undefined_Instruction_handler( void ) __attribute__ ((naked,noreturn)) */
void AT91F_Undefined_Instruction_handler( void )
{
register int *lnk_ptr;

__asm__ __volatile__ (
      "sub lr, lr, #4\n"
      "mov %0, lr" : "=r" (lnk_ptr)
      );
      
DebugAbortMessage( DEBUG_UNDEFINED_INSTRUCTION_MESSAGE, lnk_ptr );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Prefetch_Abort_handler
// Prefetch Abort Interrupt will send Debug Message then Restart
// -----------------------------------------------------------
/* void AT91F_Prefetch_Abort_handler( void ) __attribute__((naked,noreturn)) */
void AT91F_Prefetch_Abort_handler( void )
{
register int *lnk_ptr;

__asm__ __volatile__ (
      "sub lr, lr, #4\n"
      "mov %0, lr" : "=r" (lnk_ptr)
      );
      
DebugAbortMessage( DEBUG_PREFETCH_ABORT_MESSAGE, lnk_ptr );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Data_Abort_handler
// Data Abort Interrupt will send Debug Message then Restart
// -----------------------------------------------------------
/* void AT91F_Data_Abort_handler( void ) __attribute__((naked,noreturn)) */
void AT91F_Data_Abort_handler( void )
{
register int *lnk_ptr;

__asm__ __volatile__ (
      "sub lr, lr, #8\n"
      "mov %0, lr" : "=r" (lnk_ptr)
      );
      
DebugAbortMessage( DEBUG_DATA_ABORT_MESSAGE, lnk_ptr );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Reserved_handler
// Reserved Interrupt will send Debug Message then Restart
// -----------------------------------------------------------
/* void AT91F_Reserved_handler( void ) __attribute__((naked,noreturn)) */ 
void AT91F_Reserved_handler( void )
{
register int *lnk_ptr;

__asm__ __volatile__ (
      "sub lr, lr, #4\n"
      "mov %0, lr" : "=r" (lnk_ptr)
      );
      
DebugAbortMessage( DEBUG_RESERVED_IRQ_MESSAGE, lnk_ptr );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Default_FIQ_handler
// Default FIQ Interrupt will send Debug Message then Restart
// -----------------------------------------------------------
/* void AT91F_Default_FIQ_handler( void ) __attribute__((naked,noreturn)) */
void AT91F_Default_FIQ_handler( void )
{
register int *lnk_ptr;

__asm__ __volatile__ (
      "sub lr, lr, #4\n"
      "mov %0, lr" : "=r" (lnk_ptr)
      );
      
DebugAbortMessage( DEBUG_FIQ_MESSAGE, lnk_ptr );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Default_IRQ_handler
// Data IRQ Interrupt will send Debug Message then Restart
// -----------------------------------------------------------
/* void AT91F_Default_IRQ_handler( void ) __attribute__((naked,noreturn)) */
void AT91F_Default_IRQ_handler( void )
{
register int *lnk_ptr;

__asm__ __volatile__ (
      "sub lr, lr, #4\n"
      "mov %0, lr" : "=r" (lnk_ptr)
      );
      
DebugAbortMessage( DEBUG_IRQ_MESSAGE, lnk_ptr );
}
// -----------------------------------------------------------


// -----------------------------------------------------------
// Function Name       : AT91F_Spurious_handler
// Spurious Handler will just return to code
// -----------------------------------------------------------
/* void AT91F_Spurious_handler( void ) __attribute__((interrupt("IRQ"))) */
void AT91F_Spurious_handler( void )
{

AT91C_BASE_AIC->AIC_EOICR = AT91C_ID_FIQ;
}
// -----------------------------------------------------------
