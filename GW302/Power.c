// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    Power.c
// Description:      Graphic Wizard Power Task File
// -----------------------------------------------------------
 

// Include Standard files
#include "Board.h"

#include "string.h"


// functions
/*-----------------------------------------------------------*/
void SPI1Setup( void )
{

// enable spi
// enable the clock of the SPI1
AT91F_SPI1_CfgPMC();
AT91F_SPI_Reset( AT91C_BASE_SPI1 );

// set SPI master mode before configuring CS (errata 40.4.4 clock can toggle before first transfer)
AT91F_SPI_CfgMode( AT91C_BASE_SPI1, AT91C_SPI_MSTR );

// set configuration for CS0 - FLASH
AT91F_SPI_CfgCs( AT91C_BASE_SPI1,
  0, // CS0
  AT91C_SPI_CPOL |
  AT91C_SPI_BITS_8 | 
  (AT91C_SPI_DLYBS & (10 << 16)) | // 10 / mck, 10 / 47923200 = 209nS
  (AT91C_SPI_SCBR & (3 << 8)) ); // mck / 3, 47.923200 / 3 = 15.9744MHz

// set configuration for CS1 - ATOD
AT91F_SPI_CfgCs( AT91C_BASE_SPI1,
  1, // CS1
  /*AT91C_SPI_CPOL |*/ 
  /*AT91C_SPI_NCPHA |*/
  // because of errata 40.2.6.3 now use cs manually for reads
  /*AT91C_SPI_CSAAT |*/ // leave chip select low
  AT91C_SPI_BITS_8 |
  /*(AT91C_SPI_DLYBCT &(10 << 24)) |*/  // 32 * 10 / mck, 320 / 47923200 = 6.68uS
  (AT91C_SPI_DLYBS & (10 << 16)) | // 10 / mck, 10 / 47923200 = 209nS
  (AT91C_SPI_SCBR & (25 << 8)) ); // mck / 25, 47.923200 / 25 = 1.916928MHz
 
AT91F_SPI_Enable( AT91C_BASE_SPI1 );

// clear PDC buffers of SPI1
AT91F_PDC_SetNextTx( AT91C_BASE_PDC_SPI1, (char *) 0, 0);
AT91F_PDC_SetNextRx( AT91C_BASE_PDC_SPI1, (char *) 0, 0);
AT91F_PDC_SetTx( AT91C_BASE_PDC_SPI1, (char *) 0, 0);
AT91F_PDC_SetRx( AT91C_BASE_PDC_SPI1, (char *) 0, 0);
  
//AT91F_PDC_EnableTx( AT91C_BASE_PDC_SPI1 ); // enable DMA transfers
//AT91F_PDC_EnableRx( AT91C_BASE_PDC_SPI1 ); // enable DMA transfers
AT91C_BASE_PDC_SPI1->PDC_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN; // Enable RX/TX DMA Transfers

}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void SPI1TransferData( unsigned int device, unsigned char *data, unsigned short int length )
{

// wait until SPI PDC ready
while( !(AT91C_BASE_SPI1->SPI_SR & AT91C_SPI_TXBUFE) ) // set when TCR & TNCR both 0
  {
  }

AT91C_BASE_SPI1->SPI_MR = device;
  
// receive data
AT91F_PDC_SetRx( AT91C_BASE_PDC_SPI1, data, length );
// send data
AT91F_PDC_SetTx( AT91C_BASE_PDC_SPI1, data, length );

// wait until SPI PDC ready
while( !(AT91C_BASE_SPI1->SPI_SR & AT91C_SPI_RXBUFF) ) // set when RCR & RNCR both 0
  {
  }
  
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void SPI1FlashRead( unsigned long int address, unsigned char *data, unsigned short int length )
{
unsigned char ReadMemoryAllocationFailed;
unsigned char *pts;
unsigned int ui;

ReadMemoryAllocationFailed = 0;

#ifdef DEVELOP // Testing memory
TestOutputReadMemAllocFail = 0; // Test output
#endif

pts = (unsigned char *)pvPortMalloc( length + 4 );
if( pts == NULL )
  {
  pts = buffer; // use display buffer if allocation fails
  ReadMemoryAllocationFailed = 1;
  
#ifdef DEVELOP // Testing memory
  TestOutputReadMemAllocFail = 1; // Test output
#endif
  
  }
	
pts[0] = SFC_READ;
ui = address / 0x010000;
pts[1] = ui % 0x100;
ui = address % 0x010000;
pts[2] = ui / 0x100;
pts[3] = ui % 0x100;

// set SPI1 chip select for Flash Chip
SPI1TransferData( SPI1_FLASH_MODE, pts, length + 4 ); // Read All

if( data != (unsigned char *)NULL )
  {
  memcpy( data, &pts[4], length );  
  }

if(!ReadMemoryAllocationFailed)
  vPortFree( (void *)pts );
	
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void SPI1FlashWrite( unsigned char command, unsigned long int address, unsigned char *data, unsigned short int length )
{
unsigned char WriteMemoryAllocationFailed;
unsigned char *ptd;
unsigned int ui, towrite;

WriteMemoryAllocationFailed = 0;

#ifdef DEVELOP // Testing memory
TestOutputWriteMemAllocFail = 0; // Test output
#endif

ptd = (unsigned char *)pvPortMalloc( length + 4 );
if( ptd == NULL )
  {
  ptd = buffer; // use display buffer if allocation fails
  WriteMemoryAllocationFailed = 1;
  
#ifdef DEVELOP // Testing memory
  TestOutputWriteMemAllocFail = 1; // Test output
#endif
  
  }

// loop for multiples of 256 bytes if data pointer passed to function and length > 256
do
  {
  // send write enable as single byte, cs needs to return high
  ptd[0] = SFC_WREN; // Write Enable
  // set SPI1 chip select for Flash Chip
  SPI1TransferData( SPI1_FLASH_MODE, ptd, 1 ); // send 1 byte
	
  ptd[0] = command;
  ui = address / 0x010000;
  ptd[1] = ui % 0x100;
  ui = address % 0x010000;
  ptd[2] = ui / 0x100;
  ptd[3] = ui % 0x100;

  towrite = 4;
  
  // no data to send?
  if( data != (unsigned char *)NULL )
    {
    if( length > 256 )
      {
	  towrite += 256;
      }
    else
      {
      towrite += length;
      }
	
    for( ui = 4; ui < towrite; ui++ )
      {
      ptd[ui] = *data++;
      }
    }

  // set SPI1 chip select for Flash Chip
  SPI1TransferData( SPI1_FLASH_MODE, ptd, towrite ); // Write Command & Page
  
  if( length > 256 )
    {
    length -= 256;
    address += 256;
    }
  else
    {
    length = 0;
    }
  
  do
    {
    ptd[0] = SFC_RDSR; // Read Status Register
    ptd[1] = 0xff;
    // set SPI1 chip select for Flash Chip
    SPI1TransferData( SPI1_FLASH_MODE, ptd, 2 );
    }
  while( ptd[1] & SFS_WIP ); // check WIP bit (write in progress) and loop until clear
  
  }
while( length != 0 );

if(!WriteMemoryAllocationFailed)
  vPortFree( (void *)ptd );	
	
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
//* Function Name       : vPowerTask
//* Object              : Save Totals to SPI Flash at power down
//* Output Parameters   :
/*-----------------------------------------------------------*/
static void vPowerTask( void *pvParameters )
{

	vTaskSuspendAll(); // stop task switching

	// test
	AT91C_BASE_PIOA->PIO_SODR = TESTPIN;

	// read factors from SPI1 Flash @ 0x000000
	SPI1FlashRead( pFac_BASEADDRESS, &(pFac.ch[0]), sizeof(pFac.pf) );

	// checksum fail on main block ?
	if( pFac.pf.CheckSum != CheckSumIt( &(pFac.ch[0]), (unsigned char *)&(pFac.pf.CheckSum) ) )
	{
		// first block checksum fail, then try backup block
		RunFactorStatus = RFS_CS1FAIL;

		// read factors from backup block
		SPI1FlashRead( pFac_BASEADDRESS + 0x800, &(pFac.ch[0]), sizeof(pFac.pf) );

		// checksum backup block
		if( pFac.pf.CheckSum != CheckSumIt( &(pFac.ch[0]), (unsigned char *)&(pFac.pf.CheckSum) ) )
		{
			// second set fail...
			RunFactorStatus = RFS_CS2FAIL;
		}
		else
		{
			// second set ok
			RunFactorStatus = RFS_CLEAR;
		}
	}
	else // checksum ok, then has software changed 
	{
		if( pFac.pf.software_type != SOFTWARE )
		{
			RunFactorStatus = RFS_SW1FAIL;
		}
		else if( pFac.pf.software_issue != ISSUE )
		{
			RunFactorStatus = RFS_IS1FAIL;
		}
		else if( pFac.pf.software_revision != REVISION )
		{
			RunFactorStatus = RFS_RV1FAIL;
		}
		else
		{
			RunFactorStatus = RFS_CLEAR;
		}
		
		// software seems to have changed, so check backup
		if( RunFactorStatus != RFS_CLEAR )
		{
			// read factors from backup block
			SPI1FlashRead( pFac_BASEADDRESS + 0x800, &(pFac.ch[0]), sizeof(pFac.pf) );
			
			// software changed?
			if( pFac.pf.software_type != SOFTWARE )
			{
				RunFactorStatus = RFS_SW2FAIL; // software type changed 
			}
			else if( pFac.pf.software_issue != ISSUE )
			{
				RunFactorStatus = RFS_IS2FAIL;
			}
			else if( pFac.pf.software_revision != REVISION )
			{
				RunFactorStatus = RFS_RV2FAIL;
			}
			else
			{
				RunFactorStatus = RFS_CLEAR;
			}
			
		}
	}
	
	// set defaults
	if( RunFactorStatus != RFS_CLEAR ) 	// Preserved factors changed
	{
		// set default program factors & write to flash
		SetDefaultFactors(); // checksum calculated here for the first time
		SetDefaultTotals();
	}
	else //Preserved factors not changed so check Runtime
	{
		// read runtime factors and totals from SPI1 Flash @ 0x001000
		SPI1FlashRead( pRun_BASEADDRESS, &(pRun.ch[0]), sizeof(pRun.pr) );

		// checksum fail on first block ?
		if( pRun.pr.CheckSum != CheckSumIt( &(pRun.ch[0]), (unsigned char *)&(pRun.pr.CheckSum) ) )
		{
			// first block checksum fail, then try second block
			RunFactorStatus = 0x08;

			// read runtime factors and totals from SPI1 Flash @ 0x001800
			SPI1FlashRead( pRun_BASEADDRESS + 0x800, &(pRun.ch[0]), sizeof(pRun.pr) );

			if( pRun.pr.CheckSum != CheckSumIt( &(pRun.ch[0]), (unsigned char *)&(pRun.pr.CheckSum) ) )
			{
				// second set fail...
				RunFactorStatus = 0x09;
				// set default runtime factors and reset totals	
				SetDefaultTotals();
				// factors are erased at this location for faster write at power down!
				// so no need to write them
				// write runtime factors and totals to SPI1 Flash @ 0x001000
				// SPI1FlashWrite( SFC_PW, pRun_BASEADDRESS, &(pRun.ch[0]), sizeof(pRun.pr) );
			}  

		}
	}

	AT91C_BASE_PIOA->PIO_CODR = BUZZER;
//	AT91C_BASE_PIOA->PIO_CODR = OUT5;

	// write runtime factors and totals to SPI1 Flash @ 0x001800
	SPI1FlashWrite( SFC_PW, pRun_BASEADDRESS + 0x800, &(pRun.ch[0]), sizeof(pRun.pr) );

	// erase runtime factors and totals from SPI1 Flash @ 0x001000
	SPI1FlashWrite( SFC_PE, pRun_BASEADDRESS, (unsigned char *)NULL, sizeof(pRun.pr) );

	AT91C_BASE_PIOA->PIO_SODR = BUZZER;
//	AT91C_BASE_PIOA->PIO_SODR = OUT5;

  //SetFactorPointers();
	SetStartupDefaults();

	// test
	AT91C_BASE_PIOA->PIO_CODR = TESTPIN;

	pRun.pr.PowerCount++;

	// turn on wakeup on WKUP1 low to high level, set maximum slow clock cycles event should last
	// AT91C_BASE_SHDWC->SHDWC_SHMR = AT91C_SHDWC_CPTWK1 | AT91C_SHDWC_WKMODE1_LOW;

	// turn on supply voltage check atod interrupt & PIOB interrupts
	/* AT91F_AIC_EnableIt( AT91C_BASE_AIC, AT91C_ID_ADC0 ); */
	AT91C_BASE_AIC->AIC_IECR = (0x1 << AT91C_ID_ADC0) | (0x1 << AT91C_ID_PIOB);

	// timer1 - enable clock and start timer
	AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;

	xTaskResumeAll(); // restart task switching

	vTaskSuspend( NULL ); // suspend current task - task is woken by atod interrupt

	vTaskSuspendAll(); // stop task switching

	// Disable Interrupts
	//AT91F_AIC_DisableIt( AT91C_BASE_AIC, AT91C_ID_TC0 );
	//AT91F_AIC_DisableIt( AT91C_BASE_AIC, AT91C_ID_ADC0 );

	// Disable all interrupts on the interrupt controller except PIT 
	AT91C_BASE_AIC->AIC_IDCR = ~((unsigned int)1 << AT91C_ID_SYS);
	// Clear pending interrupts on the Interrupt Controller except PIT
	AT91C_BASE_AIC->AIC_ICCR = ~((unsigned int)1 << AT91C_ID_SYS);

	// Stop PWM
	AT91C_BASE_PWMC->PWMC_DIS = (AT91C_PWMC_CHID0 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID2 | AT91C_PWMC_CHID3 | AT91C_PWMC_CHID4 | AT91C_PWMC_CHID5 | AT91C_PWMC_CHID6 | AT91C_PWMC_CHID7);

	// set outputs to PIO mode then turn off - some may be in PWM mode
	AT91C_BASE_PIOA->PIO_PER = OUTPUT_MASK_PIOA; // Set in PIO mode
	AT91C_BASE_PIOA->PIO_OER = OUTPUT_MASK_PIOA; // Configure as Output
	AT91C_BASE_PIOA->PIO_SODR = OUTPUT_MASK_PIOA; // Set Outputs - OFF if set

	AT91C_BASE_PIOB->PIO_PER = OUTPUT_MASK_PIOB | PFAIL; // Set in PIO mode
	AT91C_BASE_PIOB->PIO_OER = OUTPUT_MASK_PIOB; // Configure as Output
	AT91C_BASE_PIOB->PIO_SODR = OUTPUT_MASK_PIOB; // Set Outputs - OFF if set

	// Turn Pullups Off
	//AT91C_BASE_PIOA->PIO_PPUDR 
	//AT91C_BASE_PIOB->PIO_PPUDR = SW_MASK;

	// Disable Serial Debug Interrupt (DEBUG unit is on AT91C_ID_SYS)
	AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_RXRDY | AT91C_US_TXRDY;

	AT91C_BASE_CAN0->CAN_MR = 0; // turn off CAN0
	AT91C_BASE_CAN1->CAN_MR = 0; // turn off CAN1

	// turn all io clocks off but leave PIT & PIOA
	AT91C_BASE_PMC->PMC_PCDR = ~( ((unsigned int)1 << AT91C_ID_SYS) | ((unsigned int)1 << AT91C_ID_PIOA) );    

	// turn off wakeup on WKUP1/0
	AT91C_BASE_SHDWC->SHDWC_SHMR = AT91C_SHDWC_WKMODE1_NONE | AT91C_SHDWC_WKMODE0_NONE;

	// wait for SPI to finish if running
	while( !(AT91C_BASE_SPI1->SPI_SR & AT91C_SPI_TXBUFE) )
	{
	}
	AT91C_BASE_PIOA->PIO_SODR = AT91C_PA5_SPI1_NPCS1; // set output
	AT91C_BASE_PIOA->PIO_PDR = AT91C_PA5_SPI1_NPCS1; // disable PIO

	// turn off all peripherals clocks
	AT91C_BASE_PMC->PMC_PCDR = AT91C_BASE_PMC->PMC_PCSR;

	// disable reset pin - leave delay
	AT91F_RSTSetMode( AT91C_BASE_RSTC, (0x4 << 8) ); // 0.5 sec == 2^(13 + 1) / 32768hz

	// test
	AT91C_BASE_PIOA->PIO_SODR = TESTPIN;

	// setup SPI1
	SPI1Setup();

	pRun.pr.CheckSum = CheckSumIt( &(pRun.ch[0]), (unsigned char *)&(pRun.pr.CheckSum) );

	// write runtime factors and totals to SPI1 Flash @ 0x001000
	SPI1FlashWrite( SFC_PP, pRun_BASEADDRESS, &(pRun.ch[0]), sizeof(pRun.pr) );

	// setup SPI0 for display
	SPI0Setup();

	// then reset display
	adm[0] = 0xe2;
	SendToDisplay( &adm[0], 1, DM_COMMAND );

	// wait for SPI to finish writing to display
	while( !(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXBUFE) )
	{
	}

	// test
	AT91C_BASE_PIOA->PIO_CODR = TESTPIN;

	if( PowerSwitchedOff == 1 )	
	{
		// set /PFAIL input to output mode, and set low to 
		// indicate to PIC16F506 that power should remain off
		// if removed then restored
		AT91C_BASE_PIOB->PIO_OER = PFAIL; // Enable Output
		AT91C_BASE_PIOB->PIO_CODR = PFAIL; // clear output
	}

	// save all data then shutdown	
	AT91C_BASE_SHDWC->SHDWC_SHCR = SHUTDOWN_KEY | AT91C_SHDWC_SHDW;

	// all done so stay here until power goes off
	while( 1 ) 
	{
		// restart watchdog counter
		AT91C_BASE_WDTC->WDTC_WDCR = WATCHDOG_KEY | AT91C_WDTC_WDRSTT;
	} // end of while( 1 )

}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void vStartPowerTask( unsigned portBASE_TYPE uxPriority )
{

	/* Spawn the task. */
	xTaskCreate( vPowerTask, "POWER", configPOWER_STACK_SIZE, (void *)NULL, uxPriority, (xTaskHandle *)&POWERtaskhandle );
}
/*-----------------------------------------------------------*/


