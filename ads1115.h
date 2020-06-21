//Header file for fixed variables
//to allow easier configuration of 
//ADS1115 register calls


#include <inttypes.h>  // uint8_t, etc

//ADS1115 possible addresses
#define HEX48 0b1001000  //Hex 48
#define HEX49 0b1001001  //Hex 49
#define HEX4A 0b1001010  //Hex 4A
#define HEX4B 0b1001011  //Hex 4B


//Configuration Register MSB values
//These three are added together to
//form MSB of config register

//Define 1-shot read or continuous
#define ONESHOT_ON  0b10000001
#define ONESHOT_OFF 0

//Define Analog Inputs
#define AI0 0b01000000
#define AI1 0b01010000
#define AI2 0b01100000
#define AI3 0b01110000

//Define gain setting
#define GAIN1 0b00000000  //Full Scale Voltage is: 6.144 + or -
#define GAIN2 0b00000010  //FS 4.096
#define GAIN3 0b00000100  //FS 2.048
#define GAIN4 0b00000110  //FS 1.024
#define GAIN5 0b00001000  //FS  .512
#define GAIN6 0b00001010  //FS  .256
#define GAIN7 0b00001100  //FS  .256
#define GAIN8 0b00001110  //FS  .256

int16_t ads_read( int address, int ain, int gain );

