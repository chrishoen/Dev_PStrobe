#ifndef _PRUSHARET_H_
#define _PRUSHARET_H_

/*==============================================================================
This defines a memory structure that is shared between the arm and the pru.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

// Sample period is 1ms.
static const int cCyclesPerSample = 200 * 1000;

// Frame period is 50ms.
static const int cSamplesPerFrame = 50;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This defines a memory structure that is shared between the arm and the pru.

typedef struct PruShareT
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // General purpose integer variables.
   int mN1;
   int mN2;
   int mN3;
   int mN4;

   // General purpose integer variables.
   unsigned mU1;
   unsigned mU2;
   unsigned mU3;
   unsigned mU4;

   // Message variables.
   int mArmTxMsgState;
   int mArmTxMsgCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Synchronous frame double buffer select variables.
   int mMMFrameSelect;
   int mMSFrameSelect;
   int mSSFrameSelect;

   // Sample frame double buffer.
   unsigned short mSampleFrame0[cSamplesPerFrame];
   unsigned short mSampleFrame1[cSamplesPerFrame];

   // Sample frame double buffer pointers.
   unsigned short* mMMSampleFrame;
   unsigned short* mSSSampleFrame;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
} PruShareT;

//******************************************************************************
//******************************************************************************
//******************************************************************************
#endif
