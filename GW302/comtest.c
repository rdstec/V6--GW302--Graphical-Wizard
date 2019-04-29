// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    comtest.c
// Description:      Graphic Wizard Serial Coms Routines
// -----------------------------------------------------------


// Include Standard files
#include "Board.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"


// global variables
/*-----------------------------------------------------------*/
/* Handle to the com port used by both tasks. */
xComPortHandle xPort = NULL;

// functions
/*-----------------------------------------------------------*/
static void vComTxTask( void *pvParameters )
{
portTickType xTimeToWait;

//signed portCHAR *pt;

unsigned char tstring[80];

while( 1 )
  {

 #if( configUSE_TRACE_FACILITY == 1 )
  if( key == (SW_NEW | SW_IR | SW_IL) )
    {
    while( uxQueueMessagesWaiting( TxCOM0 ) != 0 )
      {
      // wait for queue to empty
      }

    vTaskList( buffer );
      
    vTaskSuspendAll(); // stop task switching
  
    // send each character from here, rather than buffering on queue as
    //  queue may not be large enough, and queue blocking delay does not
    //  work if task are suspended 
    debug_message_wait(); // wait for com port ready
    debug_message( buffer );
    
//    // Send each character in the string, one at a time
//    pt = ( signed portCHAR * )&buffer[0];
//    while( *pt )
//      {
//      xSerialPutChar( xPort, *pt, (500 / portTICK_RATE_MS) ); // wait up to 0.5 seconds if full
//      pt++;
//      }
      
    xTaskResumeAll(); // restart task switching
    }
  else
 #endif 
    {
    // task is to send test string...
    sprintf( tstring, "TC1 %lu, EOC %lu, %02X:%02X:%02X\r\n", TC1_interrupt_count, EOC7_interrupt_count, ClockRam[2], ClockRam[1], ClockRam[0] );
    vSerialPutString( xPort, tstring, 0 );
    //vSerialPutString( xPort, UCkey, 0 ); // ' ÀÁÂÃÄÅÆABCÇDEÈÉÊËFGHIÌÍÎÏĞJKLMNÑOÒÓÔÕÖØPQRSTUÙÚÛÜVWXYİßZŞ'
    //vSerialPutString( xPort, LCkey, 0 ); // ' aàáâãäåæbcçdeèéêëfghiìíîïğjklmnñoòóôõöøpqrstuùúûüvwxyıÿzş'
    //vSerialPutString( xPort, NCkey, 0 ); // ' 0123456789.+-±!#¢£$'
    }
    
  // wait before re-sending - wait a pseudo-random time as this will provide a better test
  xTimeToWait = xTaskGetTickCount() + comOFFSET_TIME;

  // make sure we don't wait too long...
  xTimeToWait %= comTX_MAX_BLOCK_TIME;

  // ...but we do want to wait
  if( xTimeToWait < comTX_MIN_BLOCK_TIME )
    {
    xTimeToWait = comTX_MIN_BLOCK_TIME;
    }

  vTaskDelay( xTimeToWait );
  }
  
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
static void vComRxTask( void *pvParameters )
{
signed portCHAR cByteRxed;

while( 1 )
  {
  if( xSerialGetChar( xPort, &cByteRxed, comRX_BLOCK_TIME )== pdTRUE )
    {
    // byte received so do something with it!
    }
    
  }
  
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void vStartComTasks( unsigned portBASE_TYPE uxPriority, unsigned portLONG ulBaudRate )
{

/* Initialise the com port then spawn the Rx and Tx tasks. */
xSerialPortInit( ulBaudRate, comBUFFER_LEN );

/* The Tx task is spawned with a lower priority than the Rx task. */
xTaskCreate( vComTxTask, "COMTx", comTx_STACK_SIZE, NULL, uxPriority - 1, NULL );
xTaskCreate( vComRxTask, "COMRx", comRx_STACK_SIZE, NULL, uxPriority,  NULL );
}
/*-----------------------------------------------------------*/
