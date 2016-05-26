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
|       api.h - API functions to read data from I2C based Pressure sensor
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
| 2016-05-26    S. Kavya                Modified Pmax macros to converting psi to bars
|--------------------------------------------------------------------------------------------------
*/
#define     PMIN                0           //Write here the minimum pressure value (defined by sensor calibration)
#define     SENSOR_MAX_PSI      450         // Maximum PSI value in pressure sensor
// In formula Pmax value is in bars. so coverting psi to bars divided by 14.503
//
#define     PMAX                (SENSOR_MAX_PSI/14.503)
// #define     P_A               0.05       //this is constant (defined by sensor calibration)
// #define     P_B               0.95       //this is constant (defined by sensor calibration)

#define     TMIN                -40.0      //this is constant (defined by sensor calibration)
#define     TMAX                125.0      //this is constant (defined by sensor calibration)
// #define     T_A               0.1        //this is constant (defined by sensor calibration)
// #define     T_B               0.9        //this is constant (defined by sensor calibration)

// P[bar]=(Pdec/32767)∗[(Pmax−Pmin)/(Bp−Ap)]+[Pmin−Ap((Pmax−Pmin)/(Bp−Ap))]
// T[°C]= (Tdec/32767)∗[(Tmax−Tmin)/(BT−AT)]+[Tmin−AT((Tmax−Tmin)/(BT−AT))]

#define         PS_DATABYTES        4
#define         SET                 1
#define         CLEAR               0


void API_SysInit( unsigned char slaveAddress );
void API_SysStart( void );


float API_ConvRaw2bar( unsigned int rawPressure );
float API_ConvPSI( float bar );
float API_ConvRaw2celsius( unsigned int rawTemperature );
float API_ConvFahrenheit( float tempCelsius );

void API_CalcPressureValues( void );
void API_CalcTempValues( void );
void delayCycles( void );
void API_ReadPressureTemp( void );

extern unsigned char countRxdData;
extern unsigned char data[4];
extern unsigned char dataReady;
