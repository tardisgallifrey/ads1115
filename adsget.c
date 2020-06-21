// ads1115b.c read pot on ANC1
// output value in volts exit program.
// uses one-shot mode
// pull up resistors in module caused problems
// used level translator - operated ADS1115 at 5V
// by Lewis Loflin lewis@bvu.net
// www.bristolwatch.com
// http://www.bristolwatch.com/rpi/ads1115.html

#include <stdio.h>
#include <unistd.h>    // needed for atio()
#include <stdlib.h>    // exit
#include "ads1115.h"   // needed for ads1115 system variables

//Future Addressing from command line:
//HEX48
//HEX49
//HEX4A
//HEX4B

//Define Analog Inputs
//AI0
//AI1
//AI2
//AI3

//Define gain setting
//GAIN1 through GAIN8


int main(int argc, char *argv[])
{

/* program call must have correct number of arguments */
/* if not, print Usage: statment and quit             */
  if( argc != 4 ){

      printf("Usage: ads_get addr AI Gain\n");
      printf("     where addr = unit address (1 , 2, 3, or 4)\n");
      printf("           AI = which analog input (0 , 1, 2, or 3)\n");
      printf("           Gain = voltage gain (1 through 8, 2 recommended)\n");
      exit (1);
   }


/* variables needed for program */
   int address, ain, gain;
   int16_t analog_in;

/* Convert arguments on command line to numbers and */
/* change variable to correct DEF'ed value.         */

/* address argument */
   switch( atoi(argv[1]) ){
   case 1:
      address = HEX48;
      break;
   case 2:
      address = HEX49;
      break;
   case 3:
      address = HEX4A;
      break;
   case 4:
      address = HEX4B;
      break;
   default:
     printf("Input number doesn't match any address.\n");
     address = 0;
     break;
   }

/* AI line argument */
   switch( atoi(argv[2]) ){
   case 0:
      ain = AI0;
      break;
   case 1:
      ain = AI1;
      break;
   case 2:
      ain = AI2;
      break;
   case 3:
      ain = AI3;
      break;
   default:
     printf("Input number doesn't match any ai line.\n");
     ain = 0;
     break;
   }

   gain = GAIN2;

if( address != 0 && ain != 0 && gain != 0){

   analog_in = ads_read( address, ain, gain );
   if( analog_in != -1 ){

      printf("%d\n", analog_in);

   }else{

      printf("ADS Device returned an error.\n");
      exit( -1 );
   }

}else{

   exit(0);

}

return 1;

}


