//
//  interface.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef interface_hpp
#define interface_hpp

enum receiveSessionStates {
    RECEIVE_SESSION_NULL            = 0,
    RECEIVE_SESSION_GOT_HEADER      = 1,
    RECEIVE_SESSION_GOT_MULTI       = 2,
    RECEIVE_SESSION_GOT_IT_ALL      = 3,
    RECEIVE_SESSION_CALLED_APP_CODE = 4,
    RECEIVE_SESSION_WAITING_TO_END  = 5
};

enum sendSessionStates {
    
    SEND_SESSION_NULL            = 0,
    SEND_SESSION_SENT_HEADER     = 1,
    SEND_SESSION_SENT_MULTI      = 2,
    SEND_SESSION_WAITING_FOR_ACK = 3,
    SEND_SESSION_SENT_IT_ALL     = 4,
    SEND_SESSION_CALLED_APP_CODE = 5,
    SEND_SESSION_WAITING_TO_END  = 6
};

typedef struct sSendManager_t
{
    BYTE      sessionState;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      messageTotalLength;
    WORD      messageFragmentLength;
    WORD      sequenceNumber;
    BYTE    * pMessageBody;
}sSendManager;


typedef struct sReceiveManager_t
{
    BYTE      sessionState;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      messageTotalLength;
    WORD      messageFragmentLength;
    WORD      sequenceNumber;
    BYTE    * pMessageBody;
}sReceiveManager;

typedef union uMessageManager_t
{
    sSendManager    pSendManagerList;
    sReceiveManager pReceiveManagerList;
}uMessageManager;

typedef struct sManager_t
{
    struct sManager_t * pNextManager;
    uMessageManager * puManager;
}sManager;


void listAppendNode (sManager * * ppManagerList, sManager * pNewManager);
int listDeleteNode (sManager * * ppManagerList, sManager * pManagerToDelete);
void listWalk (sManager * pManagerList);
int listLength (sManager * pManagerList);


#endif /* interface_hpp */


// MAX_MESSAGE_LENGTH