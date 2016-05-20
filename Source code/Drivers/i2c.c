/*
|-------------------------------------------------------------------------------
| COPYRIGHT:  CONFIDENTIAL
|
|   Copyright (c) 2016
|   McWANE Services Private Limited.
|   ALL RIGHTS RESERVED
|
|   Unauthorized access to, copying, use of or disclosure of this
|   software, or any of its features, is strictly prohibited. Your
|   access to or possession of a copy of the software is pursuant to
|   a limited license; ownership of the software and any associated
|   media remain with McWANE Services Private Limited.
|
|   Dissemination of this information or reproduction of this material
|   is strictly forbidden unless prior written permission is obtained
|   from McWANE Services Private Limited.
|-------------------------------------------------------------------------------
| Description:
|       i2c.c – I2C driver functions
|
|        Implementation:
|  Important Algorithms:
|          Dependencies:
|                  Uses:
|
|-----------------------------------------------------------------------------------------------
| Date          Changed By              Description
| YYYY-MM-DD    I. FirstName             Why was this file changed
| ==========    ====================    ===========================================================
| 2016-05-19    S. Kavya                File created, implement I2C driver functions
|--------------------------------------------------------------------------------------------------
*/

#include <msp430.h>
#include "i2c.h"
#include "api.h"

/*-----------------------------------------------------------------
NAME         : void I2C_Init( void )
DESCRIPTION  : Initialize the I2C
INPUTS       : None
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void I2C_Init( void )
{
    // select p10.1 & p10.2 as I2C
    //
    I2C_SELECT;
    // It is recomendeed to reset the serial communication module before
    // Configuring / init the registers to avoid unpredictable behaviour
    //
    // enable software reset
    //
    UCB3CTL1 |= UCSWRST;
    // select master mode, I2C mode & Synchronous mode
    //
    UCB3CTL0 = UCMST + UCMODE_3 + UCSYNC;
    // set SMCLK @ 1MHz
    //
    UCB3CTL1 = UCSSEL_2;
    // SCL 1MHz/100kHz = 10 (we need I2C Speed 100KHz)
    //
    UCB3BR0 = 10;
    UCB3BR1 = 0;
    // clear software reset
    //
    UCB3CTL1 &= ~UCSWRST;
    // enable TX, RX & NACK interrupt
    //
    UCB3IE |= UCRXIE | UCNACKIE; // | UCTXIE ;
}

/*-----------------------------------------------------------------
NAME         : void I2C_SlaveAddress( unsigned char slaveAddress )
DESCRIPTION  : send the slave address
INPUTS       : slave address
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void I2C_SlaveAddress( unsigned char slaveAddress )
{
    // slave address is 0x78
    //
    UCB3I2CSA = slaveAddress;
}

/*-----------------------------------------------------------------
NAME         : void I2C_Start( void )
DESCRIPTION  : start I2C
INPUTS       : None
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void I2C_Start( void )
{
    // Generate START condition
    //
    UCB3CTL1 |= UCTXSTT;
}

/*-----------------------------------------------------------------
NAME         : void I2C_Stop( void )
DESCRIPTION  : stop I2C
INPUTS       : None
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void I2C_Stop( void )
{
    // Generate Stop condition
    //
    UCB3CTL1 |= UCTXSTP;
}

#pragma vector = USCI_B3_VECTOR
__interrupt void USCI_B3_ISR(void)
{
    switch(__even_in_range(UCB3IV,12))
    {
        // Vector  0: No interrupts
        //
        case  0: break;
        // Vector  2: ALIFG
        //
        case  2: break;
        // Vector  4: NACKIFG
        //
        case  4:
        // If the slave does not acknowledge the transmitted data, the not-acknowledge interrupt flag UCNACKIFG is
        // set. The master must react with either a STOP condition or a repeated START condition.
        //
        __no_operation();
        // Generate I2C stop condition
        //
        UCB3CTL1 |= UCTXSTP;
        break;

        // Vector  6: STTIFG
        //
        case  6: break;
        // Vector  8: STPIFG
        //
        case  8: break;
        // Vector 10: RXIFG
        //
        case 10:
        __no_operation();
        // rxd buffer values stored in data
        //
        data[countRxdData] = UCB3RXBUF;

        // increment countRxdData value
        //
        countRxdData++;
        // if countRxdData value equals to 4?
        //
        if( PS_DATABYTES == countRxdData )
        {
            // stop communication
            //
            I2C_Stop();
            // Data is rxd
            //
            dataReady = SET;
        }

        break;
        // Vector 12: TXIFG
        //
        case 12:
        __no_operation();
        default: break;
    }
}
