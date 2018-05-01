/*****************************************************
The purpose of this program is to use the on board LEDs, switches and push buttons
based on the HW design in Vivado.

The program will control the LEDs via the switches
and will also print a message to the SDK terminal
when a button is pressed.

Terminal Settings:
   -Baud: 115200
   -Data bits: 8
   -Parity: no
   -Stop bits: 1

4/2/8
Author: Miguel Garcia-Barrera
ECEN 403 - Battlebot

Run as --> Launch on Hardware (SDB)
****************************************************/

#include <stdio.h>
#include "platform.h"
#include <xgpio.h>
#include "xparameters.h"
#include "sleep.h"

int main()
{
   // instantiate XGpio variable for every GPIO device in the system
   // later, a pointer to such GPIO variable is passed to the API driver
   XGpio input, leds;	//input = button or switch
   int button_data = 0;
   int switch_data = 0;

   int speed = 0;
   int normalized_speed = 0;
   int direction = 1;
   int counter = 0;
   int i = 0;

   xil_printf("\nApplication started.... \r \n");

   XGpio_Initialize(&input, XPAR_AXI_GPIO_0_DEVICE_ID);	//initialize input XGpio variable
   XGpio_Initialize(&leds, XPAR_AXI_GPIO_1_DEVICE_ID);	//initialize leds XGpio variable

   /*
    * XGpio_SetDataDirection(XGpio *InstancePtr, unsigned Channel, u32 DirectionMask);
    *  "InstancePtr" is a pointer to an XGpio instance to be worked
    *  "unsigned Channel" contains the channel of the GPIO (1 or 2) to operate on
    *  "u32 DirectionMask" is a bitmask specifying which discretes are input and which are output.
    *   Bits set to 0 are output and bits set to 1 are input.
    *   See Vivado AXI_GPIO blocks to identify channel:
    *   Channel 1 = buttons
    *   Channel 2 = switches
    *   0xF means input
    */
   XGpio_SetDataDirection(&input, 1, 0xF);
   XGpio_SetDataDirection(&input, 2, 0xF);

   // channel 1 of axi_gpio_1 is leds. Ox0 means output
   XGpio_SetDataDirection(&leds, 1, 0x0);

   init_platform();

   while(1){
      // read input from channel 1 of axi_gpio_0 (buttons) and pass that data to "button_data" variable
      button_data = XGpio_DiscreteRead(&input, 1);
      switch_data = XGpio_DiscreteRead(&input, 2);


      // Messages to be printed to SDK terminal
      //if(button_data == 0b0000){} // no input, then do nothing

      if(button_data == 0b0001){ // button 0 (R18) pressed
         xil_printf("button 0 pressed\n\r");
      	 speed=300000;
      	 direction = 1;
      }

      else if(button_data == 0b0010){ // button 1 (P16) pressed
         xil_printf("button 1 pressed\n\r");
         speed=200000;
      	 direction = 1;
      }

      else if(button_data == 0b0100){ // button 2 (V16) pressed
         xil_printf("button 2 pressed\n\r");
         speed=100000;
      	 direction = 1;
      }
      else if(button_data == 0b1000){ // button 3 (Y16) pressed
         xil_printf("button 3 pressed\n\r");
         speed=50000;
      	 direction = 1;
      }

      // switches control
      else if (switch_data == 0b0001){
         xil_printf("switch 0 pressed\n\r");
         speed=300000;
       	 direction = -1;
                    }
      else if (switch_data == 0b0010){
         xil_printf("switch 1 pressed\n\r");
         speed=200000;
       	 direction = -1;
                    }
      else if (switch_data == 0b0100){
         xil_printf("switch 2 pressed\n\r");
         speed=100000;
       	 direction = -1;
                    }
      else if (switch_data == 0b1000){
         xil_printf("switch 3 pressed\n\r");
         speed=50000;
      	 direction = -1;
          	        }
      else
         xil_printf("last command running or multiple buttons pressed\n\r");

      normalized_speed = (1000000-speed)/10000;
      xil_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \r \n");
      xil_printf("Value of direction: %d \r \n",direction);
      xil_printf("Value of speed: %d \r \n",normalized_speed);
      xil_printf("Value of counter: %d \r \n",counter);

      counter = counter+direction;

      if(counter>15){
    	  counter=0;
      }

      if(counter<0){
    	  counter=15;
      }

      // write data stored in "switch_data" to channel 1 of axi_gpio_1 which is leds.
      XGpio_DiscreteWrite(&leds, 1, counter);

      // control responsiveness of the board via the sleep or usleep function
      // sleep function delays operations in seconds in parenthesis
      // usleep function delays operations in microseconds. 200K is a good value for 0.2 seconds. 0.5 lags
      // both functions take only integers
      usleep(speed);
      //for(i=0; i<speed; i++);		// lag

   }
   cleanup_platform();
   return 0;
}
