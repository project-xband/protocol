
//
//  test.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

extern DEVICE_ID nullDeviceID;

extern sManager * pSendManagerList;
extern sManager * pReceiveManagerList;

sTest * pTestData;


//----------------------------------------------------------------------------------
void test (void)
{
    testInit();

    testRun();
}


void testInit (void)
{
    protocolInitialize ();
    sManagerInit ();
    
    pTestData = (sTest *) malloc (sizeof(sTest));
    
    pTestData->deviceStartingID = 5;
    pTestData->deviceCount      = 0;
    pTestData->testTime         = GetMilliCount();
}


//----------------------------------------------------------------------------------
void testRun (void)
{
    DEVICE_ID apDeviceID;
    DEVICE_ID cl1DeviceID;
    DEVICE_ID cl2DeviceID;
    
    CreateDeviceID (& apDeviceID, pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(nullDeviceID, apDeviceID, ROLE_ACCESS_POINT);
    pTestData->deviceCount++;

    CreateDeviceID (& cl1DeviceID, 100 + pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(apDeviceID, cl1DeviceID, ROLE_CLIENT);
    pTestData->deviceCount++;
    
    CreateDeviceID (& cl2DeviceID, 100 + pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo (apDeviceID, cl2DeviceID, ROLE_CLIENT);
    pTestData->deviceCount++;
        
// run test loop

    DWORD userTestTime = GetMilliCount();
    
    while (true)
    {
        if ((pTestData->testTime + 1000) < GetMilliCount() )
        {
            pTestData->testTime = GetMilliCount();
            
// create additional devices and scheduled events and cause various device states to occur
//-------------------------
// ADD TEST CODE HERE ...
            if ((userTestTime + 2000) < GetMilliCount())
            {
                BYTE pMessage[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-_-zyxwvutsrqponmlkjihgfedcba9876543210ZYXWVUTSRQPONMLKJIHGFEDCBA!";
                sendMessage (pTestData->deviceInfo[1], apDeviceID, pMessage);
                
                userTestTime = GetMilliCount();
            }

            
// END OF TEST CODE SECTION
//-------------------------

            testStepSimulation ();
        }
    }
}


//----------------------------------------------------------------------------------
void testStepSimulation (void)
{
// run receive until no sessions or transmitted packets are still in the transmit queue

//    receivePacket ();
    runDeviceStateMachine ();

// NTR when session managers are deleted, enable checking 
    while ((0 < pTestData->transmittedPacketQueueDepth) )
//        || (NULL != pSendManagerList) || (NULL != pReceiveManagerList) )
    {
        receivePacket ();
        runDeviceStateMachine ();
    }
}

