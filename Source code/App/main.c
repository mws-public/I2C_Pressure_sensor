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
|       main.c – read data from I2C based Pressure sensor
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
| 2016-05-19    S. Kavya                File created, implement application to read data
|                                       from pressure sensor
|--------------------------------------------------------------------------------------------------
*/
#include <msp430.h> 
#include "i2c.h"
#include "api.h"

#define I2C_CODE_VERSION    0.01

void enableGlobalIntrpt( void );

unsigned char countRxdData = CLEAR;
unsigned char data[4];
unsigned char dataReady = CLEAR;


int main(void)
{
    // Stop watchdog timer
    //
    WDTCTL = WDTPW | WDTHOLD;

    // API system initialization
    //
    API_SysInit( SLAVE_ADDRESS );
    // API system start
    //
    API_SysStart();
    // enable the interrupt
    //
    enableGlobalIntrpt();

    // continuous loop
    //
    while ( 1 )
    {
        // read the pressure & temperature values
        //
        API_ReadPressureTemp();
    }
}

/*-----------------------------------------------------------------
NAME         : void enableGlobalIntrpt( void )
DESCRIPTION  : enable the global interrupt
INPUTS       : None
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void enableGlobalIntrpt( void )
{
    // enable the global interrupt
    __bis_SR_register( GIE );
}
