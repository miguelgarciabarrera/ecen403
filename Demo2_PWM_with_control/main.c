/*****************************************************
The purpose of this program is to control the duty cycle of the PWM controller designed in hardware
The PWM window size can be changed in the IP graphic interface
See documentation for reference

Terminal Settings:
   -Baud: 115200
   -Data bits: 8
   -Parity: no
   -Stop bits: 1

4/28/8
Author: Miguel Garcia-Barrera
ECEN 403 - Battlebot

Run as --> Launch on Hardware (SDB)
****************************************************/

#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"
#include <xgpio.h>
#include "platform.h"

#define MY_PWM 0x43C00000 //This value is found in the Address editor tab in Vivado (next to Diagram tab)

int main(){
    int duty_cycle=0;
    int i;

    XGpio input;
    int button_data = 0;
    int switch_data = 0;

    xil_printf("\nApplication started.... \r \n");

    XGpio_Initialize(&input, XPAR_AXI_GPIO_0_DEVICE_ID);	//initialize input XGpio variable

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

     init_platform();

    while(1){
    	// if duty_cycle reaches Max value, restart.  (Max value is the "counter" value defined in H/W)
        if(duty_cycle == 512){
        	 duty_cycle = 0;

        	 Xil_Out32(MY_PWM, duty_cycle);
        	 Xil_Out32((MY_PWM+4), duty_cycle);
        	 Xil_Out32((MY_PWM+8), duty_cycle);
        	 Xil_Out32((MY_PWM+12), duty_cycle);
        	 sleep(3);

        }
        else
        	//usleep(20);
        	duty_cycle++;

			//Xil_Out32 ~ write duty_cycle to MY_PWM memory address
			Xil_Out32(MY_PWM, duty_cycle);
			Xil_Out32((MY_PWM+4), duty_cycle);
			Xil_Out32((MY_PWM+8), duty_cycle);
			Xil_Out32((MY_PWM+12), duty_cycle);
			usleep(200000);

        // frequency = 65k. Then, to fully go over 1 period in a certain time t,
        // multiply time t in seconds times 65k.
        // example, if desired completion of cycle is 5 seconds,
        // (5s)*(65k) = 325k
        for(i=0;i<800000; i++);  // repeat "else" statement i times, thus increasing PWM LEDs brightness

        // control the speed of the increase in brightness via the usleep function, which takes microseconds
        // 10k gives loop duration approx 10seconds (why?)
        //usleep(10000);

    	}
    cleanup_platform();
    return 0;
}
