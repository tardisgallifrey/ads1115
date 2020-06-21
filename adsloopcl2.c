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
#include "ads1115.h"

int fd;

//Future Addressing from command line:
//HEX48
//HEX49
//HEX4A
//HEX4B

//Configuration Register MSB values 
//Define 1-shot read or continuous
//ONESHOT_ON
//ONESHOT_OFF

//Define Analog Inputs
//AI0
//AI1
//AI2
//AI3

//Define gain setting
//GAIN1 through GAIN8

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

    writeBuf[0] = 1;    // config register is 1
    writeBuf[1] = ONESHOT_ON + AI3 + GAIN2;
    writeBuf[2] = 0b10000101; // bits 7-0  0x85

    // No real reason to use other than 0x85 for single ended,
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
