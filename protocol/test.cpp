//
//  test.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//


#include "common.hpp"


extern DEVICE_ID nullDeviceID;


//----------------------------------------------------------------------------------
void test (void)
{
    sTest * pTestData;
    
    pTestData = testInit();

    testRun(pTestData);
}

sTest * testInit (void)
{
    sTest * pTestData;
    protocolInitialize ();
    
    pTestData = (sTest *) malloc (sizeof(sTest));
    
    pTestData->clientStartingID      = 5;     // client devices
    pTestData->accessPointStartingID = 105;
    
    pTestData->clientDeviceCount      = 0;
    pTestData->accessPointDeviceCount = 0;
    
    return (pTestData);
}

void testRun (sTest * pTestData)
{
    int testTime = GetMilliCount();
    WORD apIndex;
    WORD clIndex;
    WORD repeat = 10000;
    
    createNewAP (pTestData, nullDeviceID);
    
    createNewClient(pTestData, nullDeviceID);
    
// run test loop
    while (repeat)
    {
        if ((testTime + 1000) < GetMilliCount() )
        {
        // broadcast heartbeat to all clients, repeaters and APs
            sendHeartbeats (pTestData);
        
        // allow each client to receive the heartbeat
            for (clIndex = 0; clIndex < pTestData->clientDeviceCount; clIndex++)
            {
                receiveClientPacket(pTestData, clIndex);
            }
            
            for (apIndex = 0; apIndex < pTestData->accessPointDeviceCount; apIndex++)
            {
                receiveAccessPointPacket(pTestData, apIndex);
            }
            
       // reset packet queue to empty
            pTestData->transmittedPacketQueueDepth = 0;
        
// create additional devices and scheduled events and cause various device states to occur

// ADD CODE HERE ...


//            removeReceivedPacketFromQueue (pTestData);
            
            testTime = GetMilliCount();
        }
    }
}

//----------------------------------------------------------------------------------
void createNewAP (sTest * pTestData, DEVICE_ID apDeviceID)
{
    if (false == CompareDeviceID (apDeviceID, nullDeviceID) )
    {
        CopyDeviceID (& pTestData->accessPointDeviceIDs[pTestData->accessPointDeviceCount].accessPointDeviceID, apDeviceID);
    } else {
        CreateDeviceID (& pTestData->accessPointDeviceIDs[pTestData->accessPointDeviceCount].accessPointDeviceID, pTestData->accessPointStartingID);
    }
    
    pTestData->accessPointDeviceIDs[pTestData->accessPointDeviceCount].HeartbeatLastTime = GetMilliCount();
    
    pTestData->accessPointDeviceIDs[pTestData->accessPointDeviceCount].deviceState  = DEVICE_AP_MODE;
    
    pTestData->accessPointStartingID++;
    pTestData->accessPointDeviceCount++;
}


void createNewClient (sTest * pTestData, DEVICE_ID myDeviceID)
{
    if (false == CompareDeviceID (myDeviceID, nullDeviceID) )
    {
        CopyDeviceID (& pTestData->clientDeviceIDs[pTestData->clientDeviceCount].myDeviceID, myDeviceID);
    } else {
        CreateDeviceID (& pTestData->clientDeviceIDs[pTestData->clientDeviceCount].myDeviceID, pTestData->clientStartingID);
    }

    pTestData->accessPointDeviceIDs[pTestData->clientDeviceCount].HeartbeatLastTime = GetMilliCount();
    
    pTestData->clientDeviceIDs[pTestData->clientDeviceCount].deviceState = DEVICE_OFFLINE;

    pTestData->clientStartingID++;
    pTestData->clientDeviceCount++;
}

// from each AP, send a heartbeat packet periodically
void sendHeartbeats (sTest * pTestData)
{
    int apIndex;
    packets hbPacket;
    
    for (apIndex = 0; apIndex < pTestData->accessPointDeviceCount; apIndex++)
    {
        if (GetMilliCount() > (pTestData->accessPointDeviceIDs[apIndex].HeartbeatLastTime + HEARTBEAT_TIME) )
        {
            sendHeartbeat(& hbPacket, pTestData->accessPointDeviceIDs[apIndex].accessPointDeviceID, 0, nullptr);
            transmitPacket (pTestData, & hbPacket);
        }
    }
}

// transmit a packet from a device
// for testing, using a short last in first out queue of packets to simulate multiple radio transmissions
// in partially overlapping regions with possible reordering
void transmitPacket (sTest * pTestData, packets * packet)
{
    if ((MAX_PACKETS_IN_QUEUE - 1) > pTestData->transmittedPacketQueueDepth)
    {
        memcpy ((void *)(& pTestData->transmittedPackets[pTestData->transmittedPacketQueueDepth]), (const void *)packet, sizeof(packets) );
        pTestData->transmittedPacketQueueDepth++;
    } else {
        printf("ERROR: Dropping transmitted packet due to testing queue being full\n");
    }
}

void receiveAccessPointPacket (sTest * pTestData, WORD apIndex)
{
    WORD index;
// iterate through all packets for each receiving AP
    for (index = 0; index < pTestData->transmittedPacketQueueDepth; index++)
    {
        recPacket(pTestData, & pTestData->transmittedPackets[index], pTestData->accessPointDeviceIDs[apIndex].accessPointDeviceID );
    }
}

void receiveClientPacket (sTest * pTestData, WORD clIndex)
{
    WORD index;
// iterate through all packets for each receiving client
    for (index = 0; index < pTestData->transmittedPacketQueueDepth; index++)
    {
        recPacket(pTestData, & pTestData->transmittedPackets[index], pTestData->clientDeviceIDs[clIndex].myDeviceID );
    }
}

void removeReceivedPacketFromQueue (sTest * pTestData)
{
    if (1 <= pTestData->transmittedPacketQueueDepth)
    {
        pTestData->transmittedPacketQueueDepth--;
    }
}


