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

// Cycle counter limit for 10Hz. 
static const unsigned cCycleLimit10Hz = (200*1000*1000)/10;

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
   int tCount10Hz = 0;
   int tCount1Hz = 0;
   int tFirstFrame = 1;   

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
   // Main loop to process at 10Hz.

	while(1)
   {     
      // Update status.
      gPruShare->mU1++;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Wait to execute at 10Hz.

      // Wait for the cycle counter to reach 100ms.
      while (PRU0_CTRL.CYCLE < cCycleLimit10Hz) {}

      // Clear the cycle counter. This will cause this section of code to
      // execute again in 100ms.
      PRU0_CTRL.CTRL_bit.CTR_EN = 0;
      PRU0_CTRL.CYCLE = 0;
      PRU0_CTRL.CTRL_bit.CTR_EN = 1;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Execute at 10Hz.

      // Toggle the gpio pin.
      __R30 ^= gpio;

      // Update status.
      gPruShare->mU2++;

      // Update 100ms counter.
      if (++tCount10Hz % 10 == 0)
      {
         // Update 1sec counter.
         tCount1Hz++;
         // Start a new frame.
         doStartFrame();

         // Send a message to the arm at 1hz, but not for the first frame.
         if (tFirstFrame == 1)
         {
            tFirstFrame = 0;
         }
         else
         {
            sendArmTxMsg();
         }
      }

      // Update status.
      gPruShare->mU3 = tCount10Hz;
      gPruShare->mU4 = tCount1Hz;
	}
}
