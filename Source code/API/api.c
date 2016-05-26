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
|       api.c - API functions to read data from I2C based Pressure sensor
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
| 2016-05-19    S. Kavya                File created, implement API's to access drivers
|--------------------------------------------------------------------------------------------------
*/
#include <msp430.h>
#include "api.h"
#include "i2c.h"

// define structure for pressure
//
struct tag_pressure
{
    int rawValue;
    float bar;
    float psi;
}pressure;

// define structure for temperature
//
struct tag_temp
{
    int rawValue;
    float celcius;
    float farenheit;
}temperature;

/*-----------------------------------------------------------------
NAME         : void API_SysInit( void )
DESCRIPTION  : initialize the system
INPUTS       : slave address
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void API_SysInit( unsigned char slaveAddress )
{
    // Initialization of I2C
    //
    I2C_Init();
    // passing the slave address(0x78)
    //
    I2C_SlaveAddress( slaveAddress );
}

/*-----------------------------------------------------------------
NAME         : void API_SysStart( void )
DESCRIPTION  : start the system
INPUTS       : None
OUTPUTS      : None
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
void API_SysStart( void )
{
    // start communication through I2C
    //
    I2C_Start();
}

/*-----------------------------------------------------------------
NAME         : void API_ReadPressureTemp( void )
DESCRIPTION  : read the pressure & temperature value
INPUTS       : None
OUTPUTS      : None
PROCESS      : get data, calculate pressure & temperature values,
               again start communication to read data
NOTES        :
-----------------------------------------------------------------*/
void API_ReadPressureTemp( void )
{
    // Read the pressure & temperature values
    //
    // if data is rxd?
    //
    if( SET == dataReady )
    {
        // call a function to calculate pressure
        //
        API_CalcPressureValues();

        // call a function to calculate temperature
        //
        API_CalcTempValues();

        //after getting 4bytes of data countRxdData initialize to zero
        //
        countRxdData = CLEAR;
        // Data is not rxd
        //
        dataReady = CLEAR;
        // again start I2C
        //
        I2C_Start();
    }
}

/*-----------------------------------------------------------------
NAME         : void API_CalcPressureValues( void )
DESCRIPTION  : calculate the pressure value
INPUTS       : None
OUTPUTS      : None
PROCESS      : converting rxd 2bytes pressure value into integer
NOTES        :
-----------------------------------------------------------------*/
void API_CalcPressureValues( void )
{
    // Shift MSB and Concatenate LSB (pressure)
    //
    pressure.rawValue = data[0]<<8;
    pressure.rawValue += data[1];

    // calling function of rawPressure
    //
    // calculate bar value
    //
    pressure.bar = API_ConvRaw2bar( pressure.rawValue );
    // calculate psi value
    //
    pressure.psi = API_ConvPSI( pressure.bar );
}

/*-----------------------------------------------------------------
NAME         : void API_CalcTempValues( void )
DESCRIPTION  : calculate the temperature value
INPUTS       : None
OUTPUTS      : None
PROCESS      : converting rxd 2bytes pressure value into integer
NOTES        :
-----------------------------------------------------------------*/
void API_CalcTempValues( void )
{
    // Shift MSB and Concatenate LSB (temperature)
    //
    temperature.rawValue = data[2]<<8;
    temperature.rawValue += data[3];

    // calling function of rawtemperature
    //
    // calculate celcius value
    //
    temperature.celcius = API_ConvRaw2celsius( temperature.rawValue );
    // calculate Fahrenheit value
    //
    temperature.farenheit = API_ConvFahrenheit( temperature.celcius );
}

/*-----------------------------------------------------------------
NAME         : void API_ConvRaw2bar( void )
DESCRIPTION  : Calculate the bar value using formula given by manufacturer
INPUTS       : rawPressure
OUTPUTS      : data in bar value
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
float API_ConvRaw2bar( unsigned int rawPressure )
{
    // converting bar value
    //
    //P[bar]=(Pdec/32767)∗[(Pmax−Pmin)/(Bp−Ap)]+[Pmin−Ap((Pmax−Pmin)/(Bp−Ap))]
    //
    return ( ( rawPressure / 32767.0 ) * (( PMAX - PMIN )/ 0.90 ) + ( PMIN - ( ( PMAX / 0.90 ) * 0.05) ) );
}

/*-----------------------------------------------------------------
NAME         : void API_ConvPSI( void )
DESCRIPTION  : Calculate the PSI value using formula given by manufacturer
INPUTS       : bar value
OUTPUTS      : get data in PSI value
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
float API_ConvPSI( float bar )
{
    // converting to psi value
    //
	return ( bar * 14.5037738 );
}

/*-----------------------------------------------------------------
NAME         : void API_ConvRaw2celsius( void )
DESCRIPTION  : Calculate the celsius value using formula given by manufacturer
INPUTS       : raw temperature
OUTPUTS      : get data in celsius value
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
float API_ConvRaw2celsius( unsigned int rawTemperature )
{
    // converting celsius value
    //
    //T[°C]= (Tdec/32767)∗[(Tmax−Tmin)/(BT−AT)]+[Tmin−AT((Tmax−Tmin)/(BT−AT))]
    //
    return ( ( rawTemperature / 32767.0 ) * ( ( TMAX - TMIN ) / 0.8 )  + ( TMIN - ( ( TMAX / 0.8 ) * 0.10 ) ) );
}

/*-----------------------------------------------------------------
NAME         : void API_ReadPressureTemp( void )
DESCRIPTION  : Calculate the farenheit value using formula given by manufacturer
INPUTS       : celsius value
OUTPUTS      : get data in farenheit
PROCESS      :
NOTES        :
-----------------------------------------------------------------*/
float API_ConvFahrenheit( float tempCelsius )
{
    // converting to fahrenheit value
    //
	return ( tempCelsius * 1.8 + 32 );          //conversion in °F
}
