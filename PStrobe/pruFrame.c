/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "pruShare.h"
#include "pruFrame.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize sample frame processing.

void initializeSampleFrames()
{
   gPruShare->mN1 = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a new frame.

void doStartFrame()
{
   if (gPruShare->mMMFrameSelect == 0)
   {
      gPruShare->mMMFrameSelect = 1;
      gPruShare->mMSFrameSelect = 0;
   }
   else
   {
      gPruShare->mMMFrameSelect = 0;
      gPruShare->mMSFrameSelect = 1;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Process a new frame sample.

void doProcessSample()
{

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

