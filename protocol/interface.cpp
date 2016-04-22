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



//----------------------------------------------------------------------------------
// setup a send message session

sManager * pSendManagerList    = NULL;
sManager * pReceiveManagerList = NULL;

void sendHeaderAndData (sTest * pTestData, BYTE * pMessage)
{
    sManager     * pManager;
    sSendManager * pSendManager;
    packets        dataPacket;
    DWORD          hashValue;
    BYTE           bytesSent;
    DWORD          messageLength;
    DEVICE_ID      apDeviceID;
    DEVICE_ID      sourceDeviceID;
    DEVICE_ID      destinationDeviceID;
    
    CopyDeviceID (& apDeviceID, pTestData->accessPointDeviceIDs[0].accessPointDeviceID);
    CopyDeviceID (& destinationDeviceID, pTestData->clientDeviceIDs[0].myDeviceID);
    CopyDeviceID (& sourceDeviceID, pTestData->clientDeviceIDs[1].myDeviceID);
    
    messageLength = strlen((const char *)pMessage);

    sendData(& dataPacket, apDeviceID, destinationDeviceID, sourceDeviceID, messageLength, &hashValue, pMessage, &bytesSent);

// NTR:  check for allocator failure
    pManager     = (sManager *)malloc(sizeof(sManager));
    pSendManager = (sSendManager *)malloc(sizeof(sSendManager));
    
    pManager->puManager = (uMessageManager *) pSendManager;

/*
    pDataPacket->standard.typeOfPacket = TYPE_DATA;
    CopyDeviceID (& pDataPacket->data.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pDataPacket->data.destinationDeviceID, destDeviceID);
    CopyDeviceID (& pDataPacket->data.sourceDeviceID,      sourceDeviceID);
    *hash = generateHash(pMessageBody, messageLength);
    pDataPacket->data.hash = *hash;
    pDataPacket->data.messageTotalLength = messageLength;
    *pMessageBytesSent = (messageLength < MAX_MESSAGE_LENGTH) ? messageLength : MAX_MESSAGE_LENGTH;
    memcpy (pDataPacket->data.messageBody, pMessageBody, *pMessageBytesSent);
*/
    
    pSendManager->sessionState = SEND_SESSION_SENT_HEADER;
    CopyDeviceID(& pSendManager->destinationDeviceID, destinationDeviceID);
    CopyDeviceID(& pSendManager->sourceDeviceID, sourceDeviceID);
    hash = generateHash(<#uint8_t *pMessageBody#>, messageLength);
    messageTotalLength;
    messageFragmentLength;
    sequenceNumber;
    * pMessageBody;
    
    listAppendNode (& pSendManagerList, pManager);
    
    if (bytesSent < messageLength)
    {
        
        
        
    }
    
    transmitPacket(pTestData, & dataPacket);

}

// process each portion of the send session state and transmit message fragments, receive acks and handle errors
void processSendSession (void)
{
    ;
}

// call Client or Access Point application code when the message has been completely sent
void sentFullMessage (void)
{
    ;
}

// setup a receive message session
void receiveHeaderAndData (sTest * pTestData)
{
    WORD apIndex;
    for (apIndex = 0; apIndex < pTestData->accessPointDeviceCount; apIndex++)
    {
        receiveAccessPointPacket(pTestData, apIndex);
    }

    removeReceivedPacketFromQueue (pTestData);
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


void listWalk (sManager * pManagerList)
{
    sManager * pTemp;
    
    pTemp = pManagerList;
    if (pTemp == NULL)
    {
        return;
    }
    while (pTemp != NULL)
    {
//        printf ("%d ", pTemp->puManager);
        
        
        pTemp = pTemp->pNextManager;
    }
    printf("\n");
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

