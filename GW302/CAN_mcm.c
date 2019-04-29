// -----------------------------------------------------------
// Software:         GW302 - McHale V6 Fixed Chamber Round Baler
// C Source File:    CAN_mcm.c
// Description:      Graphic MCM functions
// -----------------------------------------------------------
 

// Include Standard files
#include "Board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if 0 // Removed
/*-----------------------------------------------------------*/
void can_mcm_initialise( unsigned char msd )
{
    struct CanMessage CanM_Tx;
    struct mcm_struct *ptm;
    
    // reset runtime registers
    forward_speed_buffer[0] = 0;
    forward_speed_buffer[1] = 0;
    forward_speed_buffer[2] = 0;
    forward_speed_buffer[3] = 0;
    forward_speed_buffer[4] = 0;
    forward_speed_buffer[5] = 0;
    
    ptm = &mcm[msd];
    
    ptm->motor_mode = METER_AUTO;
    ptm->factor_D = 0;
    ptm->factor_R = 0.0;
    
    ptm->motor_frequency = 0;
    ptm->motor_pulse_accumulator = 0;
    ptm->calibration_motor_pulse_accumulator = 0;
    ptm->motor_current = 0; 
    ptm->motor_speed = 0; // calculated if motor frequency
      
    // speed limt is pre calculated to save some time when speed message received
    //ptm->zero_speed_limit = (unsigned short int)((pFac.pf.minimum_speed * 200.0) / (pFac.pf.SpeedCalFac * 3.6)); 
    ptm->zero_speed_limit = (unsigned short int)((0.0 * 200.0) / (0.0 * 3.6)); // minimum_speed & SpeedCalFac removed from pFac.pf
    ptm->speed_frequency = 0;
    ptm->speed_pulse_accumulator = 0;

    ptm->dip_status = 0;
    ptm->dip_time_since_last_pulse[0] = 0;
    ptm->dip_time_since_last_pulse[1] = 0;
    ptm->dip_time_since_last_pulse[2] = 0;
    ptm->dip_time_since_last_pulse[3] = 0;
    ptm->dip_time_since_last_pulse[4] = 0;
    ptm->dip_time_since_last_pulse[5] = 0;
      
    ptm->controller_temperature = 0;
    ptm->controller_voltage = 0;
    ptm->controller_pwm = 0;
      
    ptm->motor_frequency_error = 0;
    ptm->speed_frequency_error = 0;

    ptm->priming = 0;
    ptm->target_motor_rpm = 0.0;
    ptm->target_motor_freq = 0.0;
    
    //ptm->software_type = 0;
    //ptm->software_issue = 0;
    
    ptm->dil_setting = 0;
    ptm->reset_fault = 0;
    
    ptm->status = 0; // reset all bits for initialisation
    ptm->timeout = 0;
    
    // transmit to module - reset module
    CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
    CanM_Tx.Data.c[0] = 0x01;
    CanM_Tx.Data.c[1] = 'R';
    CanM_Tx.Data.c[2] = 'E';
    CanM_Tx.Data.c[3] = 'S';
    CanM_Tx.Data.c[4] = 'E';
    CanM_Tx.Data.c[5] = 'T';
    CanM_Tx.Data.c[6] = 1;
    CanM_Tx.Data.c[7] = 1;

    xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

    /* wait for response */
    while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
    {
    }
    if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
    {
        ptm->status = CAN_MODULE_PRESENT;
    }

    if( ptm->status & CAN_MODULE_PRESENT )
    {
        ptm->timeout = 0;
        
        // transmit to module
        CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
        CanM_Tx.Data.c[0] = 0xff;
        CanM_Tx.Data.c[1] = 0;
        CanM_Tx.Data.c[2] = 0;
        CanM_Tx.Data.c[3] = 0;
        CanM_Tx.Data.c[4] = 0;
        CanM_Tx.Data.c[5] = 0;
        CanM_Tx.Data.c[6] = 0;
        CanM_Tx.Data.c[7] = 0;

        xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

        /* wait for response */
        while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
        {
        }
        if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED )
        {
            ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED;
        }
        else
        {
            ptm->status &= ~CAN_MODULE_PRESENT;
        }
    }
}
/*-----------------------------------------------------------*/
#endif    

#if 0 // Removed
/*-----------------------------------------------------------*/
void can_mcm_restart_autocal( unsigned char msd )
{
    struct CanMessage CanM_Tx;
    struct mcm_struct *ptm;
        
    ptm = &mcm[msd];
    ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED;
    ptm->timeout = 0;
        
    // transmit to module
    CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
    CanM_Tx.Data.c[0] = 0x04;
    CanM_Tx.Data.c[1] = 0;
    CanM_Tx.Data.c[2] = 0;
    CanM_Tx.Data.c[3] = 0;
    CanM_Tx.Data.c[4] = 0;
    CanM_Tx.Data.c[5] = 0;
    CanM_Tx.Data.c[6] = 0;
    CanM_Tx.Data.c[7] = 0;

    xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

    /* wait for response */
    while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
    {
    }
    if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
    {
        ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED;
    }
    else
    {
        ptm->status &= ~CAN_MODULE_PRESENT;    
    }
}
/*-----------------------------------------------------------*/
#endif

#if 0 // Removed
/*-----------------------------------------------------------*/
void can_mcm_setPID_info( unsigned char msd )
{
    struct CanMessage CanM_Tx;
    struct mcm_struct *ptm;
    
    ptm = &mcm[msd];
    
    // message only valid for CMA101 
    if( ptm->software_type != 101 )
    {
        return;
    }

    ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED;
    ptm->timeout = 0;

    // transmitted to module - send PID configuration
    CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
    CanM_Tx.Data.c[0] = 0x09;
    CanM_Tx.Data.c[1] = ptm->pF->factor_PI / 256;
    CanM_Tx.Data.c[2] = ptm->pF->factor_PI % 256;
    CanM_Tx.Data.c[3] = ptm->pF->factor_PD / 256;
    CanM_Tx.Data.c[4] = ptm->pF->factor_PD % 256;
    CanM_Tx.Data.c[5] = ptm->pF->PWM_frequency / 256;
    CanM_Tx.Data.c[6] = ptm->pF->PWM_frequency % 256;
    CanM_Tx.Data.c[7] = 0;

    xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

    /* wait for response */
    while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
    {
    }
    if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
    {
        ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED;
    }
    else
    {
        ptm->status &= ~CAN_MODULE_PRESENT;    
    }
}
/*-----------------------------------------------------------*/
#endif

#if 0 // Removed
/*-----------------------------------------------------------*/
void can_mcm_routines( unsigned char msd )
{
    struct CanMessage CanM_Tx;
    struct mcm_struct *ptm;
    
    ptm = &mcm[msd];
    
    if( ptm->status & CAN_MODULE_PRESENT )
    {
        ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        ptm->timeout = 0;
    
        // transmit to module
        CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
        CanM_Tx.Data.c[0] = 0x02;
        CanM_Tx.Data.c[1] = ptm->factor_D;
        CanM_Tx.Data.c[2] = ptm->factor_R / 256;
        CanM_Tx.Data.c[3] = ptm->factor_R % 256;
        CanM_Tx.Data.c[4] = ptm->motor_mode;
        CanM_Tx.Data.c[5] = 0;
        CanM_Tx.Data.c[6] = 0;
        CanM_Tx.Data.c[7] = 0;

        xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

        /* wait for response */
        while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
        {
        }
    
        if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
        {
            ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        }
        else
        {
            ptm->status &= ~CAN_MODULE_PRESENT;
            if( msd == 0 )
            {
                mcm_forward_speed = local_forward_speed;
            }
        }
    }

    if( ptm->status & CAN_MODULE_PRESENT )
    {
        ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        ptm->timeout = 0;

        // transmit to module
        CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
        CanM_Tx.Data.c[0] = 0x03;
        CanM_Tx.Data.c[1] = 0;
        CanM_Tx.Data.c[2] = 0;
        CanM_Tx.Data.c[3] = 0;
        CanM_Tx.Data.c[4] = 0;
        CanM_Tx.Data.c[5] = 0;
        CanM_Tx.Data.c[6] = 0;
        CanM_Tx.Data.c[7] = 0;

        xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

        /* wait for response */
        while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
        {
        }
        
        if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
        {
            ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        }
        else
        {
            ptm->status &= ~CAN_MODULE_PRESENT;    
        }
    }    
        
    if( ptm->status & CAN_MODULE_PRESENT )
    {
        ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        ptm->timeout = 0;

        // transmit to module - reset module
        CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
        CanM_Tx.Data.c[0] = 0x07;
        CanM_Tx.Data.c[1] = 0;
        CanM_Tx.Data.c[2] = 0;
        CanM_Tx.Data.c[3] = 0;
        CanM_Tx.Data.c[4] = 0;
        CanM_Tx.Data.c[5] = 0;
        CanM_Tx.Data.c[6] = 0;
        CanM_Tx.Data.c[7] = 0;

        xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

        /* wait for response */
        while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
        {
        }
        
        if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
        {
            ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        }
        else
        {
            ptm->status &= ~CAN_MODULE_PRESENT;    
        }
    }
    
    if( ptm->status & CAN_MODULE_PRESENT )
    {
        ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        ptm->timeout = 0;

        // transmit to module - reset module
        CanM_Tx.Id = CAN_RXID_MCM | ((unsigned int)ptm->pF->send_to_id << 21);
        CanM_Tx.Data.c[0] = 0x08;
        CanM_Tx.Data.c[1] = 0;
        CanM_Tx.Data.c[2] = 0;
        CanM_Tx.Data.c[3] = 0;
        CanM_Tx.Data.c[4] = 0;
        CanM_Tx.Data.c[5] = 0;
        CanM_Tx.Data.c[6] = 0;
        CanM_Tx.Data.c[7] = 0;

        xQueueSend( TxCAN, (void *)&CanM_Tx, (portTickType)100 );

        /* wait for response */
        while( !(ptm->status & CAN_MODULE_MESSAGE_RECEIVED) && (ptm->timeout < CAN_TIMEOUT) )
        {
        }
        
        if( ptm->status & CAN_MODULE_MESSAGE_RECEIVED ) 
        {
            ptm->status &= ~CAN_MODULE_MESSAGE_RECEIVED; 
        }
        else
        {
            ptm->status &= ~CAN_MODULE_PRESENT;    
        }
    }    
}
/*-----------------------------------------------------------*/
#endif
