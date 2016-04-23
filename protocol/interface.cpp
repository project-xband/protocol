//
//  interface.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

sManager * pSendManagerList;
sManager * pReceiveManagerList;


void processHeartbeat (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, BYTE receiveSignalStrength, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs)
{
    packets heartBeatReplyPacket;
    
    if ( ! CompareDeviceID (receivingDeviceID, apDeviceID) )
    {
        sendHeartbeatReply (& heartBeatReplyPacket, apDeviceID, receivingDeviceID, rand() % 10); // Make a packet
        transmitPacket(pTestData, & heartBeatReplyPacket); // Then send
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
        receiveHeaderAndData (pTestData, apDeviceID, destDeviceID, sourceDeviceID, messageTotalLength, messageFragmentLength, hash, pMessageBody);
    }
}

void processMulti (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody)
{
    ;
}

void processAck (sTest * pTestData, DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD hash, WORD sequenceNumber)
{
// once receive ack, then send multi if needed
    sSendManager * pSendManager;
    
    listWalk (pSendManagerList, & pSendManager, hash, sourceDeviceID);
    if (pSendManager != NULL)
    {
        pSendManager->sessionState = SEND_SESSION_SENT_MULTI;
    }
}



//----------------------------------------------------------------------------------
// setup and process send and receive message sessions

void sManagerInit (void)
{
    pSendManagerList    = (sManager *) malloc (sizeof(sManager));
    pReceiveManagerList = (sManager *) malloc (sizeof(sManager));
    
    pSendManagerList->pNextManager    = NULL;
    pSendManagerList->puManager       = NULL;
    pReceiveManagerList->pNextManager = NULL;
    pReceiveManagerList->puManager    = NULL;
}

// setup a send message session
void sendHeaderAndData (sTest * pTestData, BYTE * pMessage)
{
    sManager     * pManager;
    sSendManager * pSendManager;
    packets        dataPacket;
    DEVICE_ID      apDeviceID;
    DEVICE_ID      destinationDeviceID;
    DEVICE_ID      sourceDeviceID;
    DWORD          hashValue;
    BYTE           bytesSent;
    DWORD          messageLength;
    
    CopyDeviceID (& apDeviceID, pTestData->accessPointDeviceIDs[0].accessPointDeviceID);
    CopyDeviceID (& destinationDeviceID, pTestData->clientDeviceIDs[0].myDeviceID);
    CopyDeviceID (& sourceDeviceID, pTestData->clientDeviceIDs[1].myDeviceID);
    
    messageLength = strlen((const char *)pMessage);

    sendData(& dataPacket, apDeviceID, destinationDeviceID, sourceDeviceID, messageLength, &hashValue, pMessage, &bytesSent);

// NTR:  check for allocator failure
    
    if (messageLength > bytesSent)
    {
        pManager     = (sManager *)malloc(sizeof(sManager));
        pSendManager = (sSendManager *)malloc(sizeof(sSendManager));
    
        pManager->puManager = (uMessageManager *) pSendManager;
    
        listAppendNode (& pSendManagerList, pManager);
    
        pSendManager->sessionState          = SEND_SESSION_SENT_HEADER;
        CopyDeviceID (& pSendManager->apDeviceID,          apDeviceID);
        CopyDeviceID (& pSendManager->destinationDeviceID, destinationDeviceID);
        CopyDeviceID (& pSendManager->sourceDeviceID,      sourceDeviceID);
        pSendManager->hash                  = hashValue;
        pSendManager->messageTotalLength    = messageLength;
        pSendManager->messageFragmentLength = bytesSent;
        pSendManager->sequenceNumber        = 0;
        pSendManager->pMessageBody          = pMessage;
    }
    else
    {
        ;   // full message sent in a single packet
    }
    
    transmitPacket(pTestData, & dataPacket);
}

// process each portion of the send session state and transmit message fragments, receive acks and handle errors
void processSendSession (sTest * pTestData, packets * packet, sSendManager * pSendManager)
{
    packets        dataPacket;
    DEVICE_ID      apDeviceID;
    DEVICE_ID      destinationDeviceID;
    DEVICE_ID      sourceDeviceID;
    DWORD          hashValue;
    BYTE           bytesSent;
    WORD           sequenceNumber;
    DWORD          messageLength;
    BYTE         * pMessage;
    
    switch (pSendManager->sessionState) {
        case SEND_SESSION_NULL:
            ;
            break;
            
        case SEND_SESSION_SENT_HEADER:
            ;
            break;
            
        case SEND_SESSION_SENT_MULTI:
            
            CopyDeviceID (& apDeviceID, pSendManager->apDeviceID);
            CopyDeviceID (& destinationDeviceID, pSendManager->destinationDeviceID);
            CopyDeviceID (& sourceDeviceID, pSendManager->sourceDeviceID);
            hashValue      = pSendManager->hash;
            messageLength  = pSendManager->messageTotalLength;
            bytesSent      = pSendManager->messageFragmentLength;
            sequenceNumber = pSendManager->sequenceNumber++;
            pMessage       = pSendManager->pMessageBody;
            
            pMessage = pMessage + (sequenceNumber * MAX_MESSAGE_LENGTH);
            
            sendData(& dataPacket, apDeviceID, destinationDeviceID, sourceDeviceID, messageLength, &hashValue, pMessage, &bytesSent);
            
            transmitPacket(pTestData, & dataPacket);
            break;
            
        case SEND_SESSION_WAITING_FOR_ACK:
            ;
            break;
            
        case SEND_SESSION_SENT_IT_ALL:
            ;
            break;
            
        case SEND_SESSION_CALLED_APP_CODE:
            ;
            break;
            
        case SEND_SESSION_WAITING_TO_END:
            ;
            break;
            
        default:
            break;
    }
}

// call Client or Access Point application code when the message has been completely sent
void sentFullMessage (void)
{
    ;
}

// setup a receive message session
void receiveHeaderAndData (sTest * pTestData, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody)
{
    packets AckPacket;
    
    sendDataAck (& AckPacket, apDeviceID, destDeviceID, sourceDeviceID, hash, 0);
    
    transmitPacket(pTestData, & AckPacket);
}


// process each portion of the receive session state and receive message fragments, send acks and handle timeout errors waiting for new message fragments
void processReceiveSession (void)
{
    ;
}

// call Client or Access Point application code when the message has been completely received
void receivedFullMessage (void)
{
    
    ;
}



//----------------------------------------------------------------------------------
// from: http://www.cprogramming.com/snippets/source-code/singly-linked-list-insert-remove-add-count

void listAppendNode (sManager * * ppManagerList, sManager * pNewManager)
{
    sManager * pRight;
    
    pRight = * ppManagerList;
    
    while ( pRight->pNextManager != NULL )
        pRight = pRight->pNextManager;
    
    pRight->pNextManager = pNewManager;
    pRight = pNewManager;
    pRight->pNextManager = NULL;
}

int listDeleteNode (sManager * * ppManagerList, sManager * pManagerToDelete)
{
    sManager * pTemp;
    sManager * pPrev;
    
    pTemp = * ppManagerList;
    
    while ( pTemp != NULL)
    {
        if ( pTemp == pManagerToDelete)
        {
            if (  pTemp == * ppManagerList)
            {
                * ppManagerList = pTemp->pNextManager;
                free ( pTemp );
                return 1;
            }
            else
            {
                pPrev->pNextManager = pTemp->pNextManager;
                free( pTemp );
                return 1;
            }
        }
        else
        {
            pPrev = pTemp;
            pTemp = pTemp->pNextManager;
        }
    }
    return 0;
}


void listWalk (sManager * pManagerList, sSendManager * * ppSendManager, DWORD hash, DEVICE_ID sourceID)
{
    sManager * pTemp;
    
    * ppSendManager = NULL;
    
    pTemp = pManagerList;
    if (pTemp == NULL)
    {
        return;
    }
    while (pTemp != NULL)
    {
        if (hash == pTemp->puManager->pSendManagerList.hash)
        {
            if (CompareDeviceID(sourceID, pTemp->puManager->pSendManagerList.sourceDeviceID))
            {
                * ppSendManager = & pTemp->puManager->pSendManagerList;
                return;
            }
        }
        pTemp = pTemp->pNextManager;
    }
}


int listLength (sManager * pManagerList)
{
    sManager * pTemp;
    int count = 0;
    
    pTemp = pManagerList;
    while (pTemp != NULL)
    {
        pTemp = pTemp->pNextManager;
        count++;
    }
    return count;
}

