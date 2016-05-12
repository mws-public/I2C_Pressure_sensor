#include <msp430.h> 

#define     SLAVE_ADDRESS   0x78

//#define     PMIN            0           //Write here the minimum pressure value (defined by sensor calibration)
//#define     PMAX            1.2;        //Write here the full scale pressure value (defined by sensor calibration)
//#define     P_A             0.05;       //this is constant (defined by sensor calibration)
//#define     P_B             0.95;       //this is constant (defined by sensor calibration)
//
//#define     TMIN            -40.0;      //this is constant (defined by sensor calibration)
//#define     TMAX            125.0;      //this is constant (defined by sensor calibration)
//#define     T_A             0.1;        //this is constant (defined by sensor calibration)
//#define     T_B             0.9;        //this is constant (defined by sensor calibration)

//P[bar]=(Pdec/32767)∗[(Pmax−Pmin)/(Bp−Ap)]+[Pmin−Ap((Pmax−Pmin)/(Bp−Ap))]
//T[°C]= (Tdec/32767)∗[(Tmax−Tmin)/(BT−AT)]+[Tmin−AT((Tmax−Tmin)/(BT−AT))]

#define     I2C_SCL_PIN         BIT1
#define     I2C_SDA_PIN         BIT2
#define     I2C_SEL             P10SEL

#define     I2C_SELECT          ( I2C_SEL |= I2C_SCL_PIN + I2C_SDA_PIN )

void initI2C( void );
void configI2C( unsigned char slaveAddress );
void startI2C( void );
float raw2bar( unsigned int raw_pressure );
float convPSI( float bar );
float raw2celsius( unsigned int raw_temperature );
float convFahrenheit( float tempCelsius );
void stopI2C( void );

unsigned char i=0;
unsigned char data[4] ={ 0 };
unsigned char P_MSB;
unsigned char P_LSB;
unsigned char T_MSB;
unsigned char T_LSB;

float bar;
unsigned int rawPressure;
float psi;

float tempCelsius;
unsigned int rawTemperature;
float tempFahrenheit;

unsigned char dataReady = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                           // Stop watchdog timer
    I2C_SELECT;                                         // select p10.1 & p10.2 as I2C

    initI2C();
    configI2C( SLAVE_ADDRESS );                         // passing argument
    startI2C();
    __bis_SR_register( GIE );                           // enable interrupts
    while ( 1 )
    {

        if( 1 == dataReady )                            // if data is rxd?
        {
            rawPressure = data[0]<<8;                   // Shift MSB and Concatenate LSB (pressure)
            rawPressure += data[1];

            rawTemperature = data[2]<<8;                // Shift MSB and Concatenate LSB (temperature)
            rawTemperature += data[3];

            /* calling function of rawpressure */
            bar = raw2bar( rawPressure );
            psi = convPSI( bar );

            /* calling function of rawtemperature */
            tempCelsius = raw2celsius( rawTemperature );
            tempFahrenheit = convFahrenheit( tempCelsius );

            __delay_cycles(60000);                      // 60ms delay
            __delay_cycles(60000);                      // 60ms delay
            __delay_cycles(60000);                      // 60ms delay
            __delay_cycles(60000);                      // 60ms delay
            __delay_cycles(60000);                      // 60ms delay
            i = 0;

            startI2C();                                 // start I2C
            dataReady = 0;                              // Data is not rxd
        }

        __delay_cycles(60000);                          // 60ms delay
    }
}

#pragma vector = USCI_B3_VECTOR
__interrupt void USCI_B3_ISR(void)
{
    switch(__even_in_range(UCB3IV,12))
    {
    case  0: break;                           // Vector  0: No interrupts
    case  2: break;                           // Vector  2: ALIFG
    case  4:                                  // Vector  4: NACKIFG
        // If the slave does not acknowledge the transmitted data, the not-acknowledge interrupt flag UCNACKIFG is
        // set. The master must react with either a STOP condition or a repeated START condition.
        //
      __no_operation();
      /* Generate I2C stop condition */
       UCB3CTL1 |= UCTXSTP;
      break;

    case  6: break;                           // Vector  6: STTIFG
    case  8: break;                           // Vector  8: STPIFG
    case 10:                                  // Vector 10: RXIFG
    __no_operation();
    data[i] = UCB3RXBUF;                      // rxd buffer values stored in data

    i++;                                      // increment i value
    if( 4 == i )                              // if i value equals to 4?
    {
        // i = 0;
        stopI2C();                            // stop I2C
        dataReady = 1;                        // Data is rxd
    }

    break;
    case 12:                                  // Vector 12: TXIFG
    __no_operation();
    default: break;
    }
}


/* Initializing I2C */
void initI2C( void )
{
    // It is recomendeed to reset the serial communication module before
    // Configuring / init the registers to avoid unpredictable behaviour
    //
    UCB3CTL1 |= UCSWRST;                            // enable software reset
    UCB3CTL0 = UCMST + UCMODE_3 + UCSYNC;           // select master mode, I2C mode & Synchronous mode
    UCB3CTL1 = UCSSEL_2; // + UCSWRST;              // set SMCLK @ 1MHz
    UCB3BR0 = 10;                                   // SCL 1MHz/100kHz = 10 (we need I2C Speed 100KHz)
    UCB3BR1 = 0;
    UCB3CTL1 &= ~UCSWRST;                           // clear software reset
    UCB3IE |= UCRXIE | UCNACKIE; // | UCTXIE ;      // enable TX, RX & NACK interrupt
}

/* Configuring I2C */
void configI2C( unsigned char slaveAddress )
{
    UCB3I2CSA = slaveAddress;                       // slave address is 0x78
}

/* Generate I2C start condition */
void startI2C( void )
{
    UCB3CTL1 |= UCTXSTT;                            // Generate START condition
}
/* function definition of rawpressure */
float raw2bar( unsigned int raw_pressure )
{
    return ( ( raw_pressure / 32767.0 ) * ( 1.2 / 0.90 ) + ( 0 - ( ( 1.2 / 0.90 ) * 0.05) ) );
}
float convPSI( float bar )
{
    return ( bar * 14.5037738 );                     // Conversion to psi
}
/* function definition of rawtemperature */
float raw2celsius( unsigned int raw_temperature )
{
    return ( ( raw_temperature / 32767.0 ) * ( 165 / 0.8 )  + ( -40 - ( ( 165 / 0.8 ) * 0.10 ) ) );
}
float convFahrenheit( float tempCelsius )
{
    return ( tempCelsius * 1.8 + 32 );              //conversion in °F
}
/* Generate I2C stop condition */
void stopI2C( void )
{
    UCB3CTL1 |= UCTXSTP;                            // Generate Stop condition
}
