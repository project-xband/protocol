//
//  device.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/24/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

extern DEVICE_ID nullDeviceID;

extern sTest * pTestData;

sDeviceInfo * pDevices[10];
WORD          deviceCount;

#ifdef DEVICE_DEBUG
extern sManager * pSendManagerList;
extern sManager * pReceiveManagerList;
#endif

//----------------------------------------------------------------------------------
// Physical and simulated device initialization, state machine operation and packet communication
sDeviceInfo * initializeDeviceInfo (DEVICE_ID accessPointDeviceID, DEVICE_ID deviceID, BYTE initialDeviceRole)
{
    sDeviceInfo * pDeviceInfo;

    if (NULL == (pDeviceInfo = (sDeviceInfo *) malloc (sizeof(sDeviceInfo)) ) )
        return (NULL);
    
    pDeviceInfo->pRegionDeviceInfo = NULL;
    
    CopyDeviceID (& pDeviceInfo->myDeviceID, deviceID);
    CopyDeviceID (& pDeviceInfo->accessPointDeviceID, accessPointDeviceID);
    
    pDeviceInfo->heartbeatLastTime = 0;
    
    pDeviceInfo->deviceTypeID  = 1;
    pDeviceInfo->DeviceVersion = 1;
    pDeviceInfo->deviceRole    = initialDeviceRole;
    pDeviceInfo->deviceState   = DEVICE_INITIALIZED;

    pDevices[deviceCount] = pDeviceInfo;
    deviceCount++;

    return (pDeviceInfo);
}


void runDeviceStateMachine (void)
{
    WORD index;
    sDeviceInfo * pDeviceInfo;
    
    packets heartbeatPacket;

    for (index = 0; index < deviceCount; index++)
    {
        pDeviceInfo = pDevices[index];

/*        if (pDeviceInfo->deviceRole == ROLE_ACCESS_POINT)
        {
            if ((pDeviceInfo->heartbeatLastTime + 15000) > GetMilliCount() )
            {
                sendHeartbeat(& heartbeatPacket, pDeviceInfo->myDeviceID, 0, nullptr);
                transmitPacket (& heartbeatPacket);
            }
        }
*/        
        ;
        
    }
}

// transmit a packet from a device to all others in range
void transmitPacket (packets * packet)
{
#ifdef DEVICE_DEBUG
    if ((MAX_PACKETS_IN_QUEUE - 1) > pTestData->transmittedPacketQueueDepth)
    {
        memcpy ((void *)(& pTestData->transmittedPackets[pTestData->transmittedPacketQueueDepth]), (const void *)packet, sizeof(packets) );
        pTestData->transmittedPacketQueueDepth++;
    } else {
        printf("ERROR: Dropping transmitted packet due to testing queue being full\n");
    }
#else
    ;
#endif
}

// receive a packet from a device
void receivePacket (void)
{
#ifdef DEVICE_DEBUG
    WORD index;
    
// iterate through all devices, allowing them to decode and act upon the packet
    if (0 < pTestData->transmittedPacketQueueDepth)
    {
        for (index = 0; index < pTestData->deviceCount; index++)
        {
            recPacket (pTestData->deviceInfo[index]->myDeviceID, & pTestData->transmittedPackets[0]);
        }
        removeFirstPacketFromQueue ();
    }
#else
    ;
#endif
}

// remove a packet after all devices have had a chance to receive it
void removeFirstPacketFromQueue (void)
{
#ifdef DEVICE_DEBUG
    if (0 == pTestData->transmittedPacketQueueDepth)
    {
        printf ("ERROR: attempting to remove transmitted packet from empty buffer\n");
        return;
    }
    
    if (1 < pTestData->transmittedPacketQueueDepth)
    {
        memcpy (& pTestData->transmittedPackets[0], & pTestData->transmittedPackets[1], sizeof(packets) * pTestData->transmittedPacketQueueDepth - 1);
    }
    pTestData->transmittedPacketQueueDepth--;
#endif
}


//----------------------------------------------------------------------------------
// DEVICE_ID helper functions
void CreateDeviceID (DEVICE_ID * newDeviceID, DWORD value)
{
    *(DWORD *) newDeviceID = value;
    *newDeviceID[4] = 0;    // just ignore the first 2 bytes
    *newDeviceID[5] = 0;
}

BYTE CompareDeviceID (DEVICE_ID aDeviceID, DEVICE_ID bDeviceID)
{
    if ((*(DWORD *) aDeviceID) == (*(DWORD *) bDeviceID) )
    {
        if ((*(WORD *) & (aDeviceID[4]) ) == (*(WORD *) & bDeviceID[4]) )
        {
            return true;
        }
    }
    return false;
}

void CopyDeviceID (DEVICE_ID * pDestination, DEVICE_ID source)
{
    memcpy (pDestination, source, DEVICE_ID_LENGTH);
}

void CopyDeviceIDs (DEVICE_ID * pDestination, DEVICE_ID * pSource, BYTE count)
{
    if (count > 0)
    {
        memcpy (pDestination, pSource, DEVICE_ID_LENGTH * count);
    }
}

