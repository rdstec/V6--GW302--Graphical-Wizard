/*
	FreeRTOS.org V4.1.0 - Copyright (C) 2003-2006 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	FreeRTOS.org is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FreeRTOS.org; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	A special exception to the GPL can be applied should you wish to distribute
	a combined work that includes FreeRTOS.org, without being obliged to provide
	the source code for any proprietary components.  See the licensing section 
	of http://www.FreeRTOS.org for full details of how and when the exception
	can be applied.

	***************************************************************************
	See http://www.FreeRTOS.org for documentation, latest information, license 
	and contact details.  Please ensure to read the configuration and relevant 
	port sections of the online documentation.
	***************************************************************************
*/

/* 
	BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR USART0. 

	This file contains all the serial port components that can be compiled to
	either ARM or THUMB mode.  Components that must be compiled to ARM mode are
	contained in serialISR.c.
*/

// Include Standard files
#include "Board.h"

#include <stdlib.h>

// global variables
/*-----------------------------------------------------------*/
xQueueHandle RxCOM0;
xQueueHandle TxCOM0;

// functions
/*-----------------------------------------------------------*/
void vSerialISRCreateQueues( unsigned portBASE_TYPE uxQueueLength, xQueueHandle *pRxCOM0, xQueueHandle *pTxCOM0 )
{
	
/* Create the queues used to hold Rx and Tx characters. */
RxCOM0 = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
TxCOM0 = xQueueCreate( uxQueueLength + 1, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );

/* Pass back a reference to the queues so the serial API file can post/receive characters. */
*pRxCOM0 = RxCOM0;
*pTxCOM0 = TxCOM0;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
xComPortHandle xSerialPortInit( unsigned portLONG ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
unsigned portLONG ulSpeed;
unsigned portLONG ulCD;
xComPortHandle xReturn = serHANDLE;
extern void ( vUART_ISR )( void );

/* The queues are used in the serial ISR routine, so are created from
	serialISR.c (which is always compiled to ARM mode. */
vSerialISRCreateQueues( uxQueueLength, &RxCOM0, &TxCOM0 );

if( (RxCOM0 != serINVALID_QUEUE) && (TxCOM0 != serINVALID_QUEUE) && 
	(ulWantedBaud != (unsigned portLONG)0) )
  {
  portENTER_CRITICAL();
    {
	// Enable clock
	AT91F_DBGU_CfgPMC();

	/* Disable all USART0 interrupt sources to begin... */
	AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;
	
    // pio bits set in main for now

	/* Reset various status bits (just in case)... */
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS | AT91C_US_RSTSTA;

	/* Clear Transmit and Receive Counters */
	AT91C_BASE_DBGU->DBGU_RCR = 0;
	AT91C_BASE_DBGU->DBGU_TCR = 0;

	/* Input clock to baud rate generator is MCK */
	ulSpeed = ((configCPU_CLOCK_HZ * 10) / 16) / ulWantedBaud; 
			
	/* compute the error */
	ulCD = ulSpeed / 10;
	if( (ulSpeed - (ulCD * 10)) >= 5 )
	  {
  	  ulCD++;
	  }

    /* set the baud rate divisor register */
	AT91C_BASE_DBGU->DBGU_BRGR = ulCD;

	/* Define the USART mode */
	AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE;

    // DBGU uses SYS interrupt and is configured by FreeRTOS prvSetupTimerInterrupt
	/* Setup the interrupt for DBGU, Store interrupt handler function address in DBGU vector register... */
	//AT91C_BASE_AIC->AIC_SVR[ AT91C_ID_SYS ] = (unsigned long)vSYSIRQ_ISR;
	/* USART0 interrupt level-sensitive, priority 1... */
	//AT91C_BASE_AIC->AIC_SMR[ AT91C_ID_SYS ] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 1;
			
	/* Clear some pending DBGU interrupts (just in case)... */
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTSTA;

	/* Enable DBGU interrupt sources (but not Tx for now)... */
	AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXRDY;

    // DBGU uses SYS interrupt and SYS interrupt is called vPreemptiveTick by FreeRTOS
    // DBGU uses SYS interrupt and SYS interrupt is enabled by FreeRTOS prvSetupTimerInterrupt
	/* Enable SYSIRQ interrupts in the AIC... */
	//AT91C_BASE_AIC->AIC_IECR = (0x01 << AT91C_ID_SYS);

	/* Enable receiver and transmitter... */
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	}
  portEXIT_CRITICAL();
  }
else
  {
  xReturn = (xComPortHandle)0;
  }

return xReturn;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed portCHAR *pcRxedChar, portTickType xBlockTime )
{

/* The port handle is not required as this driver only supports UART0. */
( void )pxPort;

/* Get the next character from the buffer.  Return false if no characters
   are available, or arrive before xBlockTime expires. */
if( xQueueReceive( RxCOM0, pcRxedChar, xBlockTime ) )
  {
  return pdTRUE;
  }
else
  {
  return pdFALSE;
  }
}
/*-----------------------------------------------------------*/

void vSerialPutString( xComPortHandle pxPort, const signed portCHAR * const pcString, unsigned portSHORT usStringLength )
{
signed portCHAR *pxNext;

/* NOTE: This implementation does not handle the queue being full as no	block time is used! */

/* The port handle is not required as this driver only supports UART0. */
( void )pxPort;

/* Send each character in the string, one at a time. */
pxNext = ( signed portCHAR * ) pcString;
while( *pxNext )
  {
  xSerialPutChar( pxPort, *pxNext, serBLOCK_TIME );
  pxNext++;
  }
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed portCHAR cOutChar, portTickType xBlockTime )
{

/* The port handle is not required as this driver only supports UART0. */
( void )pxPort;

/* Place the character in the queue of characters to be transmitted. */

if( xQueueSend( TxCOM0, &cOutChar, xBlockTime ) != pdPASS )
  {
  return pdFAIL;
  }

/* Turn on the Tx interrupt so the ISR will remove the character from the
  queue and send it. This does not need to be in a critical section as
  if the interrupt has already removed the character the next interrupt
  will simply turn off the Tx interrupt again. */
AT91C_BASE_DBGU->DBGU_IER = AT91C_US_TXRDY;

return pdPASS;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void vSerialClose( xComPortHandle pxPort )
{

/* The port handle is not required as this driver only supports UART0. */
( void )pxPort;

/* Not supported as not required by the demo application. */

}
/*-----------------------------------------------------------*/

