#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_empty.h"
#include "pruShare.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main()
{
   // Initialize the pru shared memory region.
   initializePruShare();

	// GPIO bit 5.
	volatile uint32_t gpio = 0x0020;
   
   // Enable the OCP master port.
   CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

   // Enable the cycle counter.
   PRU0_CTRL.CTRL_bit.CTR_EN = 1;

   // Update shared memory region.
   gPruShare->mN1 = 2001;
   
   // Loop to toggle the gpio pin.
	while(1)
   {
      unsigned tBeginCycleCount = PRU0_CTRL.CYCLE;

      // Update shared memory region with the cycle counter.
      gPruShare->mU1 = PRU0_CTRL.CYCLE;

      while(1)
      {
         gPruShare->mN2++;
         unsigned tCycleCount = PRU0_CTRL.CYCLE;
         unsigned tDeltaCycleCount = tCycleCount - tBeginCycleCount;
         gPruShare->mU2 = tCycleCount;
         gPruShare->mU3 = tDeltaCycleCount;
         if (tDeltaCycleCount > 200*1000) break;
      }
      gPruShare->mN3++;
 
		__R30 ^= gpio;

	}
}

#if 0
   // Loop to toggle the gpio pin.
	while(1)
   {
		__R30 ^= gpio;
		__delay_cycles(200000);
      // Update shared memory region with the cycle counter.
      gPruShare->mU1 = PRU0_CTRL.CYCLE;
	}
#endif

