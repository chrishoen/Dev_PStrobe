#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_empty.h"
#include "pruShare.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

// Cycle counter limit for 1ms. 
static const unsigned cCycleLimit = 200*1000;

void main()
{
   // Local variables.

	// GPIO bit 5.
	volatile uint32_t gpio = 0x0020;

   // Counters.
   int tCount1ms = 0;   
   int tCount1sec = 0;   

   // Initialize the pru shared memory region.
   initializePruShare();

   // Enable the OCP master port.
   CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

   // Clear the cycle counter.
   PRU0_CTRL.CTRL_bit.CTR_EN = 0;
   PRU0_CTRL.CYCLE = 0;
   PRU0_CTRL.CTRL_bit.CTR_EN = 1;

   // Loop to toggle the gpio pin at 1ms.
	while(1)
   {     
      // Update shared memory.
      gPruShare->mU1++;

      // Test if the cycle counter has reached 1ms.
      if (PRU0_CTRL.CYCLE > cCycleLimit)
      {
         // Clear the cycle counter. This will cause this section
         // of code to execute again in 1ms.
         PRU0_CTRL.CTRL_bit.CTR_EN = 0;
         PRU0_CTRL.CYCLE = 0;
         PRU0_CTRL.CTRL_bit.CTR_EN = 1;

         // Toggle the gpio pin.
   		__R30 ^= gpio;

         // Update shared memory.
         gPruShare->mU2++;

         // Update counters.
         if (++tCount1ms % 1000 == 0)
         {
            tCount1sec++;
         }
         gPruShare->mU3 = tCount1ms;
         gPruShare->mU4 = tCount1sec;
      }
	}
}
