/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include "resource_table_0.h"

volatile register uint32_t __R31;

/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT			((uint32_t) 1 << 30)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST			16
#define FROM_ARM_HOST      17

/*
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
#define CHAN_NAME			"rpmsg-pru"
#define CHAN_DESC			"Channel 30"
#define CHAN_PORT			30

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4


#define RPMSG_BUF_HEADER_SIZE           16
uint8_t payload[RPMSG_BUF_SIZE - RPMSG_BUF_HEADER_SIZE];

struct pru_rpmsg_transport rTransport;
uint16_t rSource, rDestin, rLength;
unsigned rSeqNum = 0;
volatile uint8_t *status;

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "pruShare.h"
#include "armTxMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the arm tx message facility.

void initializeArmTxMsg()
{
   // Update status.
   gPruShare->mArmTxMsgState = 1;

	// Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us.
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Make sure the Linux drivers are ready for RPMsg communication */
   status = &resourceTable.rpmsg_vdev.status;
   while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

   // Update status.
   gPruShare->mArmTxMsgState = 2;

   /* Initialize the RPMsg transport structure */
   pru_rpmsg_init(&rTransport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

   // Update status.
   gPruShare->mArmTxMsgState = 3;

   // Create the RPMsg channel between the PRU and ARM user space using the transport structure.
   while (pru_rpmsg_channel(RPMSG_NS_CREATE, &rTransport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);

   // Update status.
   gPruShare->mArmTxMsgState = 4;

   // Loop until the first message is received from the arm.
   // The first message establishes the source and destinations used in 
   // subsequent message send calls.
   while (1)
   {
      // Check bit 30 of register R31 to see if the ARM has kicked us.
      if (__R31 & HOST_INT)
      {
         // Update status.
         gPruShare->mArmTxMsgState = 5;

         // Clear the event status.
         CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

         // Receive the first message from the arm. This will establish the channel
         // source and destination info.
         if (pru_rpmsg_receive(&rTransport, &rSource, &rDestin, &rSeqNum, &rLength) == PRU_RPMSG_SUCCESS) 
         {
            // Update status.
            gPruShare->mArmTxMsgState = 8;
            // The first message was received, so exit.
            return;
         }
         else
         {
            // Update status.
            gPruShare->mArmTxMsgState = 7;
         }
      }
	}

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message to the arm.

void sendArmTxMsg()
{
   // Send the message and update the sequence number.
   pru_rpmsg_send(&rTransport, rDestin, rSource, &rSeqNum, 4);
   rSeqNum++;

   // Update status.
   gPruShare->mArmTxMsgCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************






#if 0
         // Receive all available messages, multiple messages can be sent per kick.
         while (pru_rpmsg_receive(&rTransport, &rSource, &rDestin, &rSeqNum, &rLength) == PRU_RPMSG_SUCCESS) 
         {
            // Update shared memory.
            gPruShare->mArmTxMsgState = 7;
            return;

            // Echo the message back to the same address from which we just received.
            pru_rpmsg_send(&rTransport, rDestin, rSource, &rSeqNum, 4);
            gPruShare->mArmTxMsgCount++;
            rSeqNum++;
            pru_rpmsg_send(&rTransport, rDestin, rSource, &rSeqNum, 4);
            rSeqNum++;
            // Update shared memory.
            gPruShare->mArmTxMsgCount++;

            // Update shared memory.
            gPruShare->mArmTxMsgState = 7;
            return;
         }
#endif
