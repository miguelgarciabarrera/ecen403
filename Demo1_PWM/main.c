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

#define MY_PWM 0x43C00000 //This value is found in the Address editor tab in Vivado (next to Diagram tab)

int main(){
    int duty_cycle=0;
    int i;

    while(1){
    	// if duty_cycle reaches Max value, restart.  (Max value is the "counter" value defined in H/W)
        if(duty_cycle== 1024)
        	duty_cycle = 0;
        else
        	duty_cycle++;

        //Xil_Out32 ~ write duty_cycle to MY_PWM memory address
        Xil_Out32(MY_PWM, duty_cycle);
        Xil_Out32((MY_PWM+4), duty_cycle);
        Xil_Out32((MY_PWM+8), duty_cycle);
        Xil_Out32((MY_PWM+12), 1024);

        // frequency = 65k. Then, to fully go over 1 period in a certain time t,
        // multiply time t in seconds times 65k.
        // example, if desired completion of cycle is 5 seconds,
        // (5s)*(65k) = 325k
        //for(i=0;i<325000; i++);  // repeat "else" statement i times, thus increasing PWM LEDs brightness

        // control the speed of the increase in brightness via the usleep function, which takes microseconds
        // 10k gives loop duration approx 10seconds (why?)
        usleep(10000);

    }
}
