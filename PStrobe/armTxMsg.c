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
#define FROM_ARM_HOST			17

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

struct pru_rpmsg_transport transport;
uint16_t src, dst, len;
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
   // Update shared memory.
   gPruShare->mArmTxMsgState = 1;

	/* Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Make sure the Linux drivers are ready for RPMsg communication */
   status = &resourceTable.rpmsg_vdev.status;
   while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

   // Update shared memory.
   gPruShare->mArmTxMsgState = 2;

   /* Initialize the RPMsg transport structure */
   pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

   // Update shared memory.
   gPruShare->mArmTxMsgState = 3;

   /* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
   while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);

   // Update shared memory.
   gPruShare->mArmTxMsgState = 4;

   while (1)
   {
      // Check bit 30 of register R31 to see if the ARM has kicked us.
      if (__R31 & HOST_INT)
      {
         // Update shared memory.
         gPruShare->mArmTxMsgState = 5;
         // Clear the event status.
         CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

         // Sequence number variable for receive.
         unsigned tSeqNum = 0;

         // Receive all available messages, multiple messages can be sent per kick.
         while (pru_rpmsg_receive(&transport, &src, &dst, &tSeqNum, &len) == PRU_RPMSG_SUCCESS) 
         {
            // Echo the message back to the same address from which we just received.
            pru_rpmsg_send(&transport, dst, src, &tSeqNum, len);
            tSeqNum++;
            pru_rpmsg_send(&transport, dst, src, &tSeqNum, len);
            // Update shared memory.
            gPruShare->mArmTxMsgCount++;
         }
      }
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message to the arm.

void sendArmTxMsg(int aCode)
{
	pru_rpmsg_send(&transport, dst, src, payload, len);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
