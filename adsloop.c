// ads1115b.c read pot on ANC1
// output value in volts exit program.
// uses one-shot mode
// pull up resistors in module caused problems
// used level translator - operated ADS1115 at 5V
// by Lewis Loflin lewis@bvu.net
// www.bristolwatch.com
// http://www.bristolwatch.com/rpi/ads1115.html

#include <stdio.h>
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include "ads1115.h"   // needed for ads1115 system variables

int16_t ads_read( int address, int shot1, int ain, int gain );


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


int main(int argc, char *argv[])
{

   int16_t analog_in;
   const float VPS = 4.096 / 32768; 	/* volts per step */

   while(1){

      analog_in = ads_read( HEX48, ONESHOT_ON, AI3, GAIN2 );
      if( analog_in != -1 ){

         printf("ADS voltage is: %4.3f\n", analog_in * VPS);
         sleep(2);

      }else{

        printf("ADS Device returned an error.\n");
        exit( -1 );
      }
   }

}


