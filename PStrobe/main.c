#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main()
{
	// GPIO bit 5.
	volatile uint32_t gpio = 0x0020;
   
   // Enable the OCP master port.
   CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

   // Counter pointer in shared memory section.
   int* tCounterPtr = (int*)0x10000;
   *tCounterPtr = 1000;
   
   // Loop to toggle the gpio pin.
	while(1){
		__R30 ^= gpio;
		__delay_cycles(200000);
	}
}

