// C Source File:    TWI_task.c


// Include Standard files
#include "Board.h"

#include <stdlib.h>

// global variables
/*-----------------------------------------------------------*/
xSemaphoreHandle SetTime;
xTaskHandle TWItaskhandle;

// functions
/*-----------------------------------------------------------*/
static void vTWI_task( void *pvParameters )
{
unsigned char c;

// Create the semaphore to set time
vSemaphoreCreateBinary( SetTime );
    
// set control register Master Enabled / Slave Disabled
AT91C_BASE_TWI->TWI_CR = AT91C_TWI_MSEN;

while( 1 )
  {
  vTaskDelay( 500 / portTICK_RATE_MS ); // 500ms
  
  // check semaphore, if available read time, else set time
  if( xSemaphoreTake( SetTime, (portTickType)0 ) == pdTRUE )
    {
    // TWI read
    // set master mode register, Slave Address, 1 byte internal address, MREAD = 1 
    AT91C_BASE_TWI->TWI_MMR = (0x32 << 16) | AT91C_TWI_IADRSZ_1_BYTE | AT91C_TWI_MREAD;
    // set internal address to 0
    AT91C_BASE_TWI->TWI_IADR = 0x0000;
    // start transfer
    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_START;
  
    for( c = 0; c < 15; c++ )
      {
  	  while( !(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_RXRDY) )
        {
        }
      ClockRam[c] = AT91C_BASE_TWI->TWI_RHR;
      }

    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_STOP;
  
    // Wait until transfer is finished
    while( !(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXCOMP) )
      {
      }
    
    // get last byte  
    ClockRam[c] = AT91C_BASE_TWI->TWI_RHR;

    }
  else // can't take semaphore - so set time
    {
    // TWI write
    // set master mode register, Slave Address, 1 byte internal address, MREAD = 0 
    AT91C_BASE_TWI->TWI_MMR = ((0x32 << 16) | AT91C_TWI_IADRSZ_1_BYTE) & ~AT91C_TWI_MREAD;
    // set internal address to 0
    AT91C_BASE_TWI->TWI_IADR = 0x0000;
    
    // set first byte to send
    AT91C_BASE_TWI->TWI_THR = ClockRam[0]; 
    
    // start transfer
    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_START;
  
    // send remaining 15 bytes
    for( c = 1; c < 16; c++ )
      {
  	  while( !(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXRDY) )
        {
        }
      AT91C_BASE_TWI->TWI_THR = ClockRam[c];
      }

    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_STOP;
    
    // Wait until transfer is finished
    while( !(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXCOMP) )
      {
      }    
  
    }
      
  // We have finished accessing ClockRam, so release the semaphore
  xSemaphoreGive( SetTime );
  
  vTaskPrioritySet( NULL, mainTWI_TASK_READ_PRIORITY );
  }
  
}  
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void vStartTWITask( unsigned portBASE_TYPE uxPriority )
{

/* Spawn the task. */
xTaskCreate( vTWI_task, "TWI", twiSTACK_SIZE, (void *)NULL, uxPriority, (xTaskHandle *)&TWItaskhandle );
}
/*-----------------------------------------------------------*/

