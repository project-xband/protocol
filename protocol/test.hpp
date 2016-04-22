//
//  test.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//
//  The protocol utilizes client, repeater and access point devices to effect a mesh with acess points
//  providing internet connectivity, access point bridging and access to the device ID to user contact list, etc.

#ifndef test_hpp
#define test_hpp


#define MAX_CLIENT_DEVICES 24
#define MAX_AP_DEVICES 24
#define MAX_PACKETS_IN_QUEUE 100
#define HEARTBEAT_TIME 15000


enum deviceState {
    DEVICE_NULL            = 0,
    DEVICE_OFFLINE         = 1,
    DEVICE_SEARCHING       = 2,
    DEVICE_REGISTERING     = 3,
    DEVICE_UNREGISTERING   = 4,
    DEVICE_CONNECTED       = 5,
    DEVICE_LOST_CONNECTION = 6,
    DEVICE_AP_MODE         = 7
};

//----------------------------------------------------------------------------------
typedef struct clientDevice_t
{
    DEVICE_ID myDeviceID;
    DEVICE_ID accesPointDeviceID;
    BYTE      accessPointReceiveStrength;
    DWORD     HeartbeatLastTime;
    BYTE      deviceState;
} sClientDevice;

typedef struct accessPoint_t
{
    DEVICE_ID     accessPointDeviceID;
    WORD          clientDeviceCount;
    DWORD         HeartbeatLastTime;
    BYTE          deviceState;
    BYTE          clientDeviceTypes [MAX_CLIENT_DEVICES];
    sClientDevice pClientDeviceIDs [MAX_CLIENT_DEVICES];
    BYTE          clientReceiveStrengths [MAX_CLIENT_DEVICES];
} sAccessPoint;


typedef struct sTest_t
{
    BYTE          clientStartingID;
    BYTE          accessPointStartingID;
    WORD          clientDeviceCount;
    WORD          accessPointDeviceCount;
    sClientDevice clientDeviceIDs [MAX_CLIENT_DEVICES];
    sAccessPoint  accessPointDeviceIDs [MAX_AP_DEVICES];
    WORD          transmittedPacketQueueDepth;
    packets       transmittedPackets[MAX_PACKETS_IN_QUEUE];
} sTest ;

//----------------------------------------------------------------------------------
void test (void);
sTest * testInit (void);
void testRun (sTest * pTestData);

void createNewAP (sTest * pTestData, DEVICE_ID apDeviceID);
void createNewClient (sTest * pTestData, DEVICE_ID apDeviceID);

void sendHeartbeats (sTest * pTestData);

void transmitPacket (sTest * pTestData, packets * packet);
void receiveAccessPointPacket (sTest * pTestData, WORD apIndex);
void receiveClientPacket (sTest * pTestData, WORD clIndex);
void removeReceivedPacketFromQueue (sTest * pTestData);

#endif /* test_hpp */
