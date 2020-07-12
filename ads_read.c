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



int16_t ads_read( int address, int ain, int gain ){

   int fd;
   int16_t val;
   uint8_t writeBuf[3], readBuf[2];

   // open device on /dev/i2c-1 
   if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
      perror("Error: Couldn't open I2C device\n");
      return -1;
   }

   // connect to ads1115 as i2c slave
   if (ioctl(fd, I2C_SLAVE, address) < 0) {
      perror("Error: Couldn't find device on address!\n");
      return -1;
   }

   // set config register and start conversion

   writeBuf[0] = 1;    // config register is 1
   writeBuf[1] = ONESHOT_ON + ain + gain;
   writeBuf[2] = 0b10000101; // bits 7-0  0x85

   // begin conversion
   if (write(fd, writeBuf, 3) != 3) {
       perror("Can't write to register 1\n");
       return -1;
   }

   // wait for conversion complete
   // checking bit 15

   do{
      if (read(fd, writeBuf, 2) != 2) {
        perror("Read conversion failure-1\n");
        return -1;
      }
   }while ((writeBuf[0] & 0x80) == 0);

   // read conversion register
   // write register pointer first

   readBuf[0] = 0;   // conversion register is 0

   if (write(fd, readBuf, 1) != 1) {
      perror("Write register select failure\n");
      return -1;
   }

   // read 2 bytes
   if (read(fd, readBuf, 2) != 2) {
      perror("Read conversion failure-2\n");
      return -1;
   }

   // convert display results
   val = readBuf[0] << 8 | readBuf[1];

   if (val < 0)   val = 0;

   close(fd);

   return val;
}

