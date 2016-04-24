
//
//  test.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

extern DEVICE_ID nullDeviceID;

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
    while (true)
    {
        if ((pTestData->testTime + 1000) < GetMilliCount() )
        {
            testStepSimulation ();

// create additional devices and scheduled events and cause various device states to occur

// ADD CODE HERE ...
            
            BYTE pMessage[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 
            sendHeaderAndData (pTestData->deviceInfo[1], apDeviceID, pMessage);


            
            pTestData->testTime = GetMilliCount();
        }
    }
}


//----------------------------------------------------------------------------------
void testStepSimulation (void)
{

    receivePacket ();
    receivePacket ();
}

