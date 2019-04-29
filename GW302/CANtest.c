// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    CANtest.c
// Description:      Graphic Wizard CAN File
// -----------------------------------------------------------


// Include Standard files
#include "Board.h"

#include "stdio.h"


// global variables
/*-----------------------------------------------------------*/
//xQueueHandle RxCAN_MCM, TxCAN; // Removed

// functions
#if 0 // Removed
/*-----------------------------------------------------------*/
void InitialiseCAN( void )
{
	unsigned int i;	
	
	for( i = 0; i < MAX_MCM; i++ )
	{	
		mcm[i].motor_mode = METER_AUTO;
		mcm[i].priming = 0;
		mcm[i].status = 0;
	}
	

RxCAN_MCM = xQueueCreate( CAN_MAXRX, (unsigned portBASE_TYPE)sizeof( struct CanMessage ) );
TxCAN = xQueueCreate( CAN_MAXTX, (unsigned portBASE_TYPE)sizeof( struct CanMessage ) );

// setup CAN0
AT91F_CAN0_CfgPMC();

// RDS CAN - set baud rate to 83.3'Khz, SMP = 0, BRP = 24, SJW = 3, PROPAG = 7, , PHASE1 = 6, PHASE2 = 6
AT91C_BASE_CAN0->CAN_BR =  ((unsigned int)0 << 24) | ((unsigned int)24 << 16) | ((unsigned int)3 << 12) | ((unsigned int)7 << 8) |  ((unsigned int)6 << 4) |  ((unsigned int)6 << 0);

AT91C_BASE_CAN0->CAN_MR = AT91C_CAN_CANEN;

// enable CAN0 interrupt
AT91F_AIC_ConfigureIt( AT91C_BASE_AIC, AT91C_ID_CAN0, INTERRUPT_PRIORITY2, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, CAN0_irq_handler );
AT91F_AIC_EnableIt( AT91C_BASE_AIC, AT91C_ID_CAN0 );

// set CAN0 mail box 0 to transmit
AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MCR = (unsigned int)0x0; //
AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MMR = AT91C_CAN_MOT_TX; // Mode: Tx Mode, 0 priority, no time mark
//AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MID = AT91C_CAN_MIDE | (unsigned int)0; // Message ID Type (29bit or 11bit) & ID value
//AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MDL = 0x44332211UL; // Message Data Low 		
//AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MDH = 0x88776655UL; // Message Data High
AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MCR = ((unsigned int)8 << 16); // length MDLC

// set CAN0 mail box 1 to receive
AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MCR = (unsigned int)0x0; //
AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MMR = AT91C_CAN_MOT_RXOVERWRITE; // Mode: Rx Mode - with overwrite, 0 priority, no time mark
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MAM = AT91C_CAN_MIDvA; // Type A Message Acceptance Mask  - all bits relevant
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MID = (unsigned int)xxx; // Type B Message ID 0 - ID not relevant if MAM all zeros
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MAM = AT91C_CAN_MIDE; // Type B Message Acceptance Mask - any ID
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MID = AT91C_CAN_MIDE; // Type B Message - Actual ID not relevant if MAM all zeros
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MAM = AT91C_CAN_MIDE | AT91C_CAN_MIDvA | AT91C_CAN_MIDvB; // Type B Message Acceptance Mask  - all bits relevant
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MID = AT91C_CAN_MIDE | (unsigned int)xxx; // Type B Message ID 0 - ID not relevant if MAM all zeros
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MAM = AT91C_CAN_MIDvA; // Type A Message Acceptance Mask, all bits relevant
//AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MAM = (0x787 << 18);  //0xAT91C_CAN_MIDvA; // Type A Message Acceptance Mask, all bits relevant
AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MAM = CAN_IDMASK_MCM; // all MCM ids (=0x787)
AT91C_BASE_CAN0->CAN_MB1.CAN_MB_MID = CAN_TXID_MCM; // Type A Message MCM ids only

// enable mail box 1 interrupt
AT91C_BASE_CAN0->CAN_IER = AT91C_CAN_MB1;

// could set up mail box 2 for type B message and have 2 queues of data
}
/*-----------------------------------------------------------*/
#endif

#if 0 // Removed
/*-----------------------------------------------------------*/
static void vCANTxTask( void *pvParameters )
{
	unsigned int status;
	struct CanMessage CanM;
	
	while( 1 )
  	{
		// get message from queue - block indefinitely
		// should unblock when message posted to queue from operate task
		if( xQueueReceive( TxCAN, (void *)&CanM, portMAX_DELAY ) == pdTRUE )
		{
			status = AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MSR;
			if( status & AT91C_CAN_MRDY ) // mailbox ready
			{	
				// setup data 
				AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MID = CanM.Id;        // Message ID
				AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MDL = CanM.Data.l[0]; // Message Data Low 		
				AT91C_BASE_CAN0->CAN_MB0.CAN_MB_MDH = CanM.Data.l[1]; // Message Data High

				// transmit CAN0 mail box 0
				AT91C_BASE_CAN0->CAN_TCR = AT91C_CAN_MB0;
			}
			else // wait a bit then try again, don't loop in high priority tasks
			{
				vTaskDelay( 100 / portTICK_RATE_MS );
			}
		}  

  	} // while( 1 )
}
/*-----------------------------------------------------------*/
#endif

#if 0 // Removed
/*-----------------------------------------------------------*/
static void vCANRxTask( void *pvParameters )
{
	struct CanMessage CanM_Rx, CanM_Tx;
	struct mcm_struct *ptm;

	int msd, id;
	unsigned long int speed_value, temp_ul;
	unsigned short int temp_us;
	//char tstring[16];

	while( 1 )
  	{
  		// get message from queue - block indefinitely
		// should unblock when interrupt receives MCM_frame and posts to queue
  		if( xQueueReceive( RxCAN_MCM, (void *)&CanM_Rx, portMAX_DELAY ) == pdTRUE )
    	{
    		// MCM CAN message received so do something with it!
  		    
  			// get module id
  			id = CanM_Rx.Id & ((unsigned int)0x0f << 21);
            CanM_Tx.Id = CAN_RXID_MCM | id;
  			id = id >> 21;
  			
  			// is this message for us?
  			for( msd = 0; (msd < MAX_MCM) && (mcm[msd].pF->send_to_id != id); msd++ )
  			{
  			}
  			// id matches one of our MCM modules
  			if( msd < MAX_MCM )
  			{
  				ptm = &mcm[msd];
	    		
  				switch( CanM_Rx.Data.c[0] )
  				{
  				case 1:
  					// Reset_Variables();
      					
  					// transmitted to module - reset module
  					// CanM_Tx.Data.c[0] = 0x01;
  					// CanM_Tx.Data.c[1] = 'R';
  					// CanM_Tx.Data.c[2] = 'E';
  					// CanM_Tx.Data.c[3] = 'S';
  					// CanM_Tx.Data.c[4] = 'E';
  					// CanM_Tx.Data.c[5] = 'T';
  					// CanM_Tx.Data.c[6] = 1;
  					// CanM_Tx.Data.c[7] = 1;
  					// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );
      					
  					// received from module
      				// CanM_Rx.Data.c[0] = 0x01;
      				// tstring[0] = CanM_Rx.Data.c[1]; // 'R'
      				// tstring[1] = CanM_Rx.Data.c[2]; // 'E'
      				// tstring[2] = CanM_Rx.Data.c[3]; // 'S'
      				// tstring[3] = CanM_Rx.Data.c[4]; // 'E'
      				// tstring[4] = CanM_Rx.Data.c[5]; // 'T'
      				// tstring[5] = CanM_Rx.Data.c[6]; // '1'
      				// tstring[6] = CanM_Rx.Data.c[7]; // '1'
      				// tstring[7] = 0x00;
        			break;
      
      			case 2:
      				// transmitted to module - set motor speed and mode
        			// CanM_Tx.Data.c[0] = 0x02;
        			// CanM_Tx.Data.c[1] = Ratio_D;
        			// CanM_Tx.Data.c[2] = Ratio_R / 256;
        			// CanM_Tx.Data.c[3] = Ratio_R % 256;
        			// CanM_Tx.Data.c[4] = Motor_Mode;
        			// CanM_Tx.Data.c[5] = 0;
        			// CanM_Tx.Data.c[6] = 0;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );
        				
      				// received from module
        			// CanM_Rx.Data.c[0] = 0x02;
      				ptm->motor_frequency = ((unsigned short int)CanM_Rx.Data.c[1] * 256) + CanM_Rx.Data.c[2];
      				ptm->motor_frequency_error = (signed char)CanM_Rx.Data.c[5]; 
      				
      				ptm->controller_voltage = CanM_Rx.Data.c[6];
      				ptm->controller_pwm = CanM_Rx.Data.c[7];
      				
      				temp_us = ((unsigned short int)CanM_Rx.Data.c[3] * 256) + CanM_Rx.Data.c[4];
  					ptm->calibration_motor_pulse_accumulator += temp_us;
      				if( ptm->motor_mode != METER_CALIBRATION )
      				{
      					ptm->motor_pulse_accumulator += temp_us;
      				}
        			break;
        
   				case 3:
   					// transmitted to module - get travel information
   					// CanM_Tx.Data.c[0] = 0x03;
      				// CanM_Tx.Data.c[1] = speed_frequency  / 256;
      				// CanM_Tx.Data.c[2] = speed_frequency % 256;
      				// CanM_Tx.Data.c[3] = speed_pulses / 256;
      				// CanM_Tx.Data.c[4] = speed_pulses % 256;
      				// CanM_Tx.Data.c[5] = 0;
      				// CanM_Tx.Data.c[6] = 0;
   					// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );
      					
      				// received from module
        			// CanM_Rx.Data.c[0] = 0x03;
   					ptm->speed_frequency = ((unsigned short int)CanM_Rx.Data.c[1] * 256) + CanM_Rx.Data.c[2];
      				ptm->speed_frequency_error = (signed char)CanM_Rx.Data.c[5];
      				
      				// speed_value is 200 times speed frequency
      				speed_value = (ptm->speed_frequency * 200) + ptm->speed_frequency_error;

   					ptm->speed_pulse_accumulator += ((unsigned short int)CanM_Rx.Data.c[3] * 256) + CanM_Rx.Data.c[4];
   					
   					if( msd == 0 )
   					{
   						if( speed_value > ptm->zero_speed_limit ) // speed_limit is 200 times limit frequency
   						{
   							if( use_simulated_speed || prestart_status )
   							{
   								use_simulated_speed = 0;
   								prestart_status = 0;
   								mcm[0].motor_mode = METER_AUTO;
   							}
   						}
   							
						if( use_simulated_speed ) // use simulated speed
						{
							//mcm_forward_speed = pRun.pr.simulated_speed;
							mcm_forward_speed = 0.0; // simulated_speed removed from pRun.pr 
						}
						else 
   						{
   							forward_speed_buffer[0] = forward_speed_buffer[1];
   							temp_ul = forward_speed_buffer[0];
   							forward_speed_buffer[1] = forward_speed_buffer[2];
   							temp_ul += forward_speed_buffer[1];
   							forward_speed_buffer[2] = forward_speed_buffer[3];
   							temp_ul += forward_speed_buffer[2];
   							forward_speed_buffer[3] = forward_speed_buffer[4];
   							temp_ul += forward_speed_buffer[3];
   							forward_speed_buffer[4] = forward_speed_buffer[5];
   							temp_ul += forward_speed_buffer[4];
   							forward_speed_buffer[5] = speed_value;
   							temp_ul += forward_speed_buffer[5];
   						
   							// buffer contains speed_frequency * 200 * 6
   							// if > 1.5 times or < 0.5 times current frequency, reset buffer
   							if( (speed_value > (temp_ul / 800)) ||
   								(speed_value < (temp_ul / 2400)) )					
   							{
   								//forward_speed_buffer[5] = speed_value; - is already set above!
   								forward_speed_buffer[4] = speed_value;
   								forward_speed_buffer[3] = speed_value;
   								forward_speed_buffer[2] = speed_value;
   								forward_speed_buffer[1] = speed_value;
   								forward_speed_buffer[0] = speed_value;
   								temp_ul = speed_value * 6; // set to unaveraged buffer sum
   							}
                          //mcm_forward_speed = (float)temp_ul * pFac.pf.SpeedCalFac * (3.6000 / 1200.0);
   							mcm_forward_speed = (float)temp_ul * 0.0 * (3.6000 / 1200.0); // SpeedCalFac removed from pFac.pf
   						}
   					}

        			break;
      
      			case 4:
  					// transmitted to module - start auto cal
   					// CanM_Tx.Data.c[0] = 0x04;
      				// CanM_Tx.Data.c[1] = 0;
       				// CanM_Tx.Data.c[2] = 0;
       				// CanM_Tx.Data.c[3] = 0;
       				// CanM_Tx.Data.c[4] = 0;
       				// CanM_Tx.Data.c[5] = 0;
       				// CanM_Tx.Data.c[6] = 0;
        			// CanM_Tx.Data.c[7] = 0;
   	    			// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );
      				
   					// received from module
       				// CanM_Rx.Data.c[0] = 0x04;
      				
        			break;
	        
   	  			case 5:
   					// transmitted to module - auto cal progress
   	  				// CanM_Tx.Data.c[0] = 0x05;
        			// CanM_Tx.Data.c[1] = 0;
        			// CanM_Tx.Data.c[2] = 0;
        			// CanM_Tx.Data.c[3] = 0;
        			// CanM_Tx.Data.c[4] = 0;
        			// CanM_Tx.Data.c[5] = 0;
        			// CanM_Tx.Data.c[6] = 0;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

      				// received from module
        			// CanM_Rx.Data.c[0] = 0x05;
      				// motor_frequency = ((unsigned short int)CanM_Rx.Data.c[1] * 256) + CanM_Rx.Data.c[2];
      				// motor_pulses =((unsigned short int)CanM_Rx.Data.c[3] * 256) + CanM_Rx.Data.c[4];
    	  			// data_valid = CanM_Rx.Data.c[4];

        			break;
        
      			case 6:
   					// transmitted to module - stop auto cal
      				// CanM_Tx.Data.c[0] = 0x06;
        			// CanM_Tx.Data.c[1] = 0;
        			// CanM_Tx.Data.c[2] = 0;
        			// CanM_Tx.Data.c[3] = 0;
        			// CanM_Tx.Data.c[4] = 0;
        			// CanM_Tx.Data.c[5] = 0;
        			// CanM_Tx.Data.c[6] = 0;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

      				// received from module
        			// CanM_Rx.Data.c[0] = 0x06;
      				// motor_frequency = ((unsigned short int)CanM_Rx.Data.c[1] * 256) + CanM_Rx.Data.c[2];
      				// motor_pulses =((unsigned short int)CanM_Rx.Data.c[3] * 256) + CanM_Rx.Data.c[4];
    	  			// data_valid = CanM_Rx.Data.c[4];

        			break;
        
				case 7:
      				// transmitted to module - get auxiliary inputs
      				// CanM_Tx.Data.c[0] = 0x07;
        			// CanM_Tx.Data.c[1] = 0;
        			// CanM_Tx.Data.c[2] = 0;
        			// CanM_Tx.Data.c[3] = 0;
        			// CanM_Tx.Data.c[4] = 0;
        			// CanM_Tx.Data.c[5] = 0;
        			// CanM_Tx.Data.c[6] = 0;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

      				// received from module
        			// CanM_Rx.Data.c[0] = 0x07;
      				// motor_temp = CanM_Rx.Data.c[1];
      				// motor_current = ;
      				// drive_temp = CanM_Rx.Data.c[3]; // always 0
      				// input_status = CanM_Rx.Data.c[4];
					
					ptm->motor_current = (unsigned char)((float)CanM_Rx.Data.c[2] / 2.55); 
      				ptm->controller_temperature = (unsigned char)((unsigned short int)CanM_Rx.Data.c[3] * 150 / 255);
      				ptm->dip_status = CanM_Rx.Data.c[4];
        			break;
                                                     
	      		case 8:
      				// transmitted to module - get auxiliary inputs
      				// CanM_Tx.Data.c[0] = 0x08;
        			// CanM_Tx.Data.c[1] = 0;
        			// CanM_Tx.Data.c[2] = 0;
        			// CanM_Tx.Data.c[3] = 0;
        			// CanM_Tx.Data.c[4] = 0;
        			// CanM_Tx.Data.c[5] = 0;
        			// CanM_Tx.Data.c[6] = 0;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

      				// received from module
        			// CanM_Rx.Data.c[0] = 0x08;
      				ptm->dip_status = CanM_Rx.Data.c[1];
	    		    ptm->dip_time_since_last_pulse[0] = CanM_Rx.Data.c[2];
	    		    ptm->dip_time_since_last_pulse[1] = CanM_Rx.Data.c[3];
	    		    ptm->dip_time_since_last_pulse[2] = CanM_Rx.Data.c[4];
	    		    ptm->dip_time_since_last_pulse[3] = CanM_Rx.Data.c[5];
	    		    ptm->dip_time_since_last_pulse[4] = CanM_Rx.Data.c[6];
	    		    ptm->dip_time_since_last_pulse[5] = CanM_Rx.Data.c[7];
        			break;
                                                     
      			case 9:
    				// transmitted to module - send PID configuration
      				// CanM_Tx.Data.c[0] = 0x09;
        			// CanM_Tx.Data.c[1] = con_PI / 256;
        			// CanM_Tx.Data.c[2] = con_PI % 256;
        			// CanM_Tx.Data.c[3] = con_D / 256;
        			// CanM_Tx.Data.c[4] = con_D % 256;
        			// CanM_Tx.Data.c[5] = con_Frequency / 256;
        			// CanM_Tx.Data.c[6] = con_Frequency % 256;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

      				// received from module
        			// CanM_Rx.Data.c[0] = 0x09;
        			break;

				case 10:
      				// transmitted to module - send aux frequency
      				// CanM_Tx.Data.c[0] = 0x0a;
        			// CanM_Tx.Data.c[1] = aux_frequency / 256;
        			// CanM_Tx.Data.c[2] = aux_frequency % 256;
        			// CanM_Tx.Data.c[3] = aux_pulses / 256;
        			// CanM_Tx.Data.c[4] = aux_pulses % 256;
        			// CanM_Tx.Data.c[5] = 0;
        			// CanM_Tx.Data.c[6] = 0;
	        		// CanM_Tx.Data.c[7] = 0;
    	    		// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );
					
      				// received from module
        			// CanM_Rx.Data.c[0] = 0x0a;
					break;

				case 0xff:
   					// transmitted to module - get module info
   					// CanM_Tx.Data.c[0] = 0xff;
     				// CanM_Tx.Data.c[1] = software_type / 256;
       				// CanM_Tx.Data.c[2] = software_type % 256;
       				// CanM_Tx.Data.c[3] = software_issue / 256;
       				// CanM_Tx.Data.c[4] = software_issue % 256;
       				// CanM_Tx.Data.c[5] = inputs9to12_status;
       				// CanM_Tx.Data.c[6] = reset_fault;
        			// CanM_Tx.Data.c[7] = 0;
   	    			// xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

   					// received from module
       				// CanM_Rx.Data.c[0] = 0xff;
      			    ptm->software_type =  ((unsigned short int)CanM_Rx.Data.c[1] * 256) + CanM_Rx.Data.c[2];
      			    ptm->software_issue = ((unsigned short int)CanM_Rx.Data.c[3] * 256) + CanM_Rx.Data.c[4];
      			    ptm->dil_setting = CanM_Rx.Data.c[5];
      			    ptm->reset_fault = CanM_Rx.Data.c[6];
        			break;
        
   				default:
   					CanM_Tx.Data.c[0] = 0xaa;
    	    		CanM_Tx.Data.c[1] = 0xbb;
        			CanM_Tx.Data.c[2] = 0xcc;
        			CanM_Tx.Data.c[3] = 0xdd;
        			CanM_Tx.Data.c[4] = 0;
        			CanM_Tx.Data.c[5] = 0;
	        		CanM_Tx.Data.c[6] = 0;
    	    		CanM_Tx.Data.c[7] = CanM_Rx.Data.c[0];
        			xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );
        			break;         
				}
  				
			ptm->status |= CAN_MODULE_MESSAGE_RECEIVED;
  			}   
    	}
  	}
}
/*-----------------------------------------------------------*/
#endif

#if 0 // Removed
/*-----------------------------------------------------------*/
void vStartCANTasks( unsigned portBASE_TYPE uxPriority )
{
	// Initialise CAN peripheral
	InitialiseCAN();

	/* The Tx task is spawned with a lower priority than the Rx task. */
	//xTaskCreate( vCANTxTask, "CAN0Tx", canTx_STACK_SIZE, NULL, uxPriority - 1, NULL );
	xTaskCreate( vCANTxTask, "CAN0Tx", canTx_STACK_SIZE, NULL, uxPriority, NULL );
	xTaskCreate( vCANRxTask, "CAN0Rx", canRx_STACK_SIZE, NULL, uxPriority, NULL );
}
/*-----------------------------------------------------------*/
#endif
