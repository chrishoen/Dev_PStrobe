#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_empty.h"
#include "pruShare.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

// Cycle counter reload value. The cycle counter will reach 
// 0xffffffff after 1ms.
static const unsigned cCycleReload = 0xffffffff - 200*1000;

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

   // Reload the cycle counter for 1ms.
   PRU0_CTRL.CYCLE = cCycleReload;

   // Update shared memory region.
   gPruShare->mN1 = 2001;
   
   // Loop to toggle the gpio pin at 1ms.
	while(1)
   {
      // Update shared memory region.
      gPruShare->mN2++;

      // Get the cycle counter.
      unsigned tCycleCount = PRU0_CTRL.CYCLE;
      gPruShare->mU1 = tCycleCount;

      // Test for 1ms.
      if (tCycleCount == 0xffffffff)
      {
         // Enable the cycle counter.
         PRU0_CTRL.CTRL_bit.CTR_EN = 1;

         // Reload the cycle counter for 1ms.
         PRU0_CTRL.CYCLE = cCycleReload;

         // Toggle the gpio pin.
   		__R30 ^= gpio;

         // Update shared memory region.
         gPruShare->mN3++;
      }
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

