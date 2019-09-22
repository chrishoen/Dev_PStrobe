/*==============================================================================
Program that strobes a gpio pin and communicates with the arm. 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_0.h"
#include "pruShare.h"
#include "armTxMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regional variables.

volatile register uint32_t __R30;
volatile register uint32_t __R31;

// Cycle counter limit for 1ms. 
static const unsigned cCycleLimit = 200*1000;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Main.

void main()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

	// GPIO bit 5.
	volatile uint32_t gpio = 0x0020;

   // Counters.
   int tCount1ms = 0;   
   int tCount1sec = 0;   

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Initialize the pru shared memory.
   initializePruShare();

   // Enable the OCP master port.
   CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

   // Clear the cycle counter.
   PRU0_CTRL.CTRL_bit.CTR_EN = 0;
   PRU0_CTRL.CYCLE = 0;
   PRU0_CTRL.CTRL_bit.CTR_EN = 1;

   // Initialize the arm message facility.
   initializeArmTxMsg();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to toggle the gpio pin at 1ms.

	while(1)
   {     
      // Update shared memory.
      gPruShare->mU1++;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Execute at 1ms.

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

         // Update 1ms counter.
         if (++tCount1ms % 1000 == 0)
         {
            // Update 1sec counter.
            tCount1sec++;
            sendArmTxMsg();
         }

         // Update shared memory.
         gPruShare->mU3 = tCount1ms;
         gPruShare->mU4 = tCount1sec;
      }
	}
}
