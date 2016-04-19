//
//  interface.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

void processHeartbeat (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, BYTE receiveSignalStrength, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs)
{
    packets heartBeatReplyPacket;
    
    if ( ! CompareDeviceID (receivingDeviceID, apDeviceID) )
    {
        sendHeartbeatReply (& heartBeatReplyPacket, apDeviceID, receivingDeviceID, rand() % 10);
        transmitPacket(pTestData, & heartBeatReplyPacket);
    }
}

void processHeartbeatReply (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE receiveSignalStrength)
{
    if ( CompareDeviceID (receivingDeviceID, apDeviceID) )
    {
// maintain the APs client ID list of info and broadcast it in heartbeat packets
        ;
    }
}

void processRegistration (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID)
{
    ;
}

void processRegistrationReply (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE internetConnected)
{
    ;
}

void processData (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody)
{
    // deal with AP target data versus other client data target
    if ( CompareDeviceID (receivingDeviceID, apDeviceID) )
    {
        printf("message received ==> %s\n", pMessageBody);
    }
}

void processMulti (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody)
{
    ;
}

void processAck (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD hash, WORD sequenceNumber)
{
    ;
}

