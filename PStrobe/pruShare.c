/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#define  _PRUSHARE_C_
#include "pruShare.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the pru shared memory pointer and contents.

void initializePruShare()
{
   // Initialize the pointer.
   gPruShare = (PruShareT*)0x10000;

   // Initialize members.
   gPruShare->mN1 = 0;
   gPruShare->mN2 = 0;
   gPruShare->mN3 = 0;
   gPruShare->mN4 = 0;

   // Initialize members.
   gPruShare->mU1 = 0;
   gPruShare->mU2 = 0;
   gPruShare->mU3 = 0;
   gPruShare->mU4 = 0;

   // Initialize members.
   gPruShare->mArmTxMsgState = 0;
   gPruShare->mArmTxMsgCount = 0;
}

