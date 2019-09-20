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
   gPruShare->mN1 = 1001;
   gPruShare->mN2 = 1002;
   gPruShare->mN3 = 1003;
   gPruShare->mN4 = 1004;
}