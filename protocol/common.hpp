//
//  common.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef common_h
#define common_h

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <string.h>
//#include <time.h>
#include <sys/timeb.h>


#define DEVICE_ID_LENGTH 6
#define DEVICE_LIST_MAX 6
#define MAX_MESSAGE_LENGTH 48

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t


#include "protocol.hpp"
#include "interface.hpp"
#include "test.hpp"


//----------------------------------------------------------------------------------
// protocol.cpp function prototypes

void protocolInitialize (void);

void sendHeartbeat (packets * pHeartbeatPacket, DEVICE_ID myDeviceID, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs);
void sendHeartbeatReply (packets * pHeartbeatReplyPacket, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID, BYTE apRSSI);
void sendRegistration (packets * pRegistration, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID);
void sendRegistrationReply (packets * pRegistrationPacket, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID);
void sendData (packets * pDataPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, WORD messageLength, DWORD * hash, BYTE * pMessageBody, BYTE * pMessageBytesSent);
void sendMulti (packets * pMultiPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, WORD messageLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody, BYTE * pMessageBytesSent);
void sendDataAck (packets * pAckPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD hash, WORD sequenceNumber);

void recPacket (sTest * pTestData, packets * newPacket, DEVICE_ID receivingDeviceID);

void recHeartbeat  (packets * pHeartbeatPacket, DEVICE_ID * apDeviceID, BYTE * pReceiveSignalStrength, BYTE * deviceCount, DEVICE_ID * pArrayOfDeviceIDs);
void recHeartbeatReply (packets * pHeartbeatReplyPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID, BYTE * pReceiveSignalStrength);
void recRegistration (packets * pRegistrationPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID);
void recRegistrationReply (packets * pRegistrationReplyPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID, BYTE * internetConnected);
void recData (packets * pDataPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, DWORD * messageTotalLength, BYTE * messageFragmentLength, DWORD * hash, BYTE * pMessageBody);
void recMultiData (packets * pMultiPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, BYTE * messageFragmentLength, DWORD * hash, WORD * sequenceNumber, BYTE * pMessageBody);
void recDataAck (packets * pAckPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, DWORD * hash, WORD * sequenceNumber);

void CreateDeviceID (DEVICE_ID * newDeviceID, DWORD value);
BYTE CompareDeviceID (DEVICE_ID aDeviceID, DEVICE_ID bDeviceID);
void CopyDeviceID (DEVICE_ID * pDestination, DEVICE_ID source);
void CopyDeviceIDs (DEVICE_ID * pDestination, DEVICE_ID * pSource, BYTE count);
DWORD generateHash (BYTE * pMessageBody, WORD messageLength);

// from https://rosettacode.org/wiki/CRC-32#Implementation
uint32_t rc_crc32 (uint32_t crc, const char *buf, size_t len);

// From: http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
uint32_t GetMilliCount (void);
uint32_t GetMilliSpan (uint32_t nTimeStart);

//----------------------------------------------------------------------------------
// interface.cpp function prototypes


void getDeviceInfo (sDeviceInfo * pDeviceInfo);
void getRegionInfo (sRegionInfo * pRegionInfo);
void registerWithAp (void);
void unregisterWithAp (void);
void sendMessageCallBack (void);
void sendMessage (sMessage * pMessage, void (* pCallBackFunction)() );
void checkForMessages (sMessage * pMessage);


void processHeartbeat (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, BYTE receiveSignalStrength, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs);
void processHeartbeatReply (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE receiveSignalStrength);
void processRegistration (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID);
void processRegistrationReply (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE internetConnected);
void processData (sTest* pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody);
void processMulti (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody);
void processAck (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD hash, WORD sequenceNumber);

void sManagerInit (void);  // Initialize send and receive message session subsystem
void sendHeaderAndData (sTest * pTestData, BYTE * pMessage);  // setup a send message session
void processSendSession (void);  // process each portion of the send session state and transmit message fragments, receive acks and handle errors
void sentFullMessage (void);  // call Client or Access Point application code when the message has been completely sent
void receiveHeaderAndData (sTest * pTestData, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody);
void processReceiveSession (void);  // process each portion of the receive session state and receive message fragments, send acks and handle timeout errors waiting for new message fragments
void receivedFullMessage (void);  // call Client or Access Point application code when the message has been completely received

#endif /* common_h */
