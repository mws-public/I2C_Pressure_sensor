#define     PMIN            0          //Write here the minimum pressure value (defined by sensor calibration)
#define     PMAX            1.2        //Write here the full scale pressure value (defined by sensor calibration)
//#define     P_A             0.05       //this is constant (defined by sensor calibration)
//#define     P_B             0.95       //this is constant (defined by sensor calibration)
//
#define     TMIN            -40.0      //this is constant (defined by sensor calibration)
#define     TMAX            125.0      //this is constant (defined by sensor calibration)
//#define     T_A             0.1        //this is constant (defined by sensor calibration)
//#define     T_B             0.9        //this is constant (defined by sensor calibration)

//P[bar]=(Pdec/32767)∗[(Pmax−Pmin)/(Bp−Ap)]+[Pmin−Ap((Pmax−Pmin)/(Bp−Ap))]
//T[°C]= (Tdec/32767)∗[(Tmax−Tmin)/(BT−AT)]+[Tmin−AT((Tmax−Tmin)/(BT−AT))]

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
