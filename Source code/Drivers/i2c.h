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
|       I2C.h - I2C driver header file
|
|        Implementation:
|  Important Algorithms:
|          Dependencies:
|                  Uses:
|
|-----------------------------------------------------------------------------------------------
| Date          Changed By              Description
| YYYY-MM-DD    I. FirstName            Why was this file changed
| ==========    ====================    ===========================================================
| 2016-05-19    S. Kavya                File created, implement I2C driver header file
|--------------------------------------------------------------------------------------------------
*/
#define     SLAVE_ADDRESS   0x78

#define     I2C_SCL_PIN         BIT1
#define     I2C_SDA_PIN         BIT2
#define     I2C_SEL             P10SEL

#define     I2C_SELECT          ( I2C_SEL |= I2C_SCL_PIN + I2C_SDA_PIN )

extern unsigned char countRxdData;
extern unsigned char data[4];
extern unsigned char dataReady;

void I2C_Init( void );
void I2C_SlaveAddress( unsigned char slaveAddress );
void I2C_Start( void );
void I2C_Stop( void );
