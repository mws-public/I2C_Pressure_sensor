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
