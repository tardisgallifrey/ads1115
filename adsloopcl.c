// ads1115b.c read pot on ANC1
// output value in volts exit program.
// uses one-shot mode
// pull up resistors in module caused problems
// used level translator - operated ADS1115 at 5V
// by Lewis Loflin lewis@bvu.net
// www.bristolwatch.com
// http://www.bristolwatch.com/rpi/ads1115.html

#include <stdio.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <sys/ioctl.h>

int fd;

//Future Addressing from command line:
#define HEX48 0b1001000  //Hex 48
#define HEX49 0b1001001  //Hex 49
#define HEX4A 0b1001010  //Hex 4A
#define HEX4B 0b1001011  //Hex 4B


//Configuration Register MSB values 
//Define 1-shot read or continuous
#define ONESHOT_ON  0b10000001
#define ONESHOT_OFF 0

//Define Analog Inputs
#define AI0 0b01000000
#define AI1 0b01010000
#define AI2 0b01100000
#define AI3 0b01110000

//Define gain setting
#define GAIN1 0b00000000
#define GAIN2 0b00000010
#define GAIN3 0b00000100
#define GAIN4 0b00000110
#define GAIN5 0b00001000
#define GAIN6 0b00001010
#define GAIN7 0b00001100
#define GAIN8 0b00001110

//16 bit integer exactly
int16_t val;

//2 8-bit integers which become a 16-bit word
uint8_t writeBuf[3];
uint8_t readBuf[2];

//Float needed to return voltage from input being read
float myfloat;

//ADS1115 is a 16 bit A/D (0 - 32768 count)
//Max voltage is 4.096
//So, volt per step (or count) is this formula
const float VPS = 4.096 / 32768.0; //volts per step

/*
The resolution of the ADC in single ended 
mode we have 15 bit rather than 16 bit resolution, 
the 16th bit being the sign of the differential reading.
*/

int main(int argc, char *argv[])
{

//Set address (prefer from CL)
int ads_address = HEX48;

  // open device on /dev/i2c-1 
  // Rapberry Pi really only has 1 i2c bus
  if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
    printf("Error: Couldn't open device! %d\n", fd);
    exit (1);
  }

  // connect to ads1115 as i2c slave
  if (ioctl(fd, I2C_SLAVE, ads_address) < 0) {
    printf("Error: Couldn't find device on address!\n");
    exit (1);
  }



  while(1){

    // set config register and start conversion
    // A1 and GND, 4.096v, 128s/s

    writeBuf[0] = 1;    // config register is 1
    writeBuf[1] = ONESHOT_ON + AI3 + GAIN2;

    // Reading from the left, after the 0b:
    // bit 15 flag bit for single shot
    // Bits 14-12 input selection:
    // 100 A0
    // 101 A1
    // 110 A2
    // 111 A3
    // Bits 11-9 gain setting. Here 001
    // Full table:
    // 000 : 6.144V Full scale (+ or -)
    // 001 : 4.096
    // 010 : 2.048 (default)
    // 011 : 1.024
    // 100 :  .512
    // 101 :  .256
    // 110 :  .256
    // 111 :  .256
    // Bit 8 Operational mode of the ADS1115.
    // 0 : Continuous conversion mode
    // 1 : Power-down single-shot mode (default)

    writeBuf[2] = 0b10000101; // bits 7-0  0x85

    // Testing defines for config regiter
    printf("writeBuf[1] = %X\n", writeBuf[1]);

    // Bits 7-5 data rate default to 100 for 128SPS
    // Bits 4-0  comparator functions see spec sheet.
    // No real reason to change these for single ended,
    // resistive sensors (NTC, PTC, etc.)

    // begin conversion
    if (write(fd, writeBuf, 3) != 3) {
      perror("Write to register 1");
      exit(-1);
    }

    // wait for conversion complete
    // checking bit 15

    do {
      if (read(fd, writeBuf, 2) != 2) {
        perror("Read conversion");
        exit(-1);
      }
    }while ((writeBuf[0] & 0x80) == 0);

    // read conversion register
    // write register pointer first

    readBuf[0] = 0;   // conversion register is 0

    if (write(fd, readBuf, 1) != 1) {
      perror("Write register select");
      exit(-1);
    }

    // read 2 bytes
    if (read(fd, readBuf, 2) != 2) {
      perror("Read conversion");
      exit(-1);
    }

    // convert display results
    val = readBuf[0] << 8 | readBuf[1];

    if (val < 0)   val = 0;

    myfloat = val * VPS; // convert to voltage

    printf("Values: HEX 0x%02x DEC %d reading %4.3f volts.\n", val, val, myfloat);

    sleep(2);

  }

    close(fd);

    return 0;
}
