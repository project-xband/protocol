//
//  interface.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef interface_hpp
#define interface_hpp

#define HEARTBEAT_TIME 15000

enum deviceRoles
{
    ROLE_NULL         = 0,
    ROLE_CLIENT       = 1,
    ROLE_REPEATER     = 2,
    ROLE_ACCESS_POINT = 3
};

enum deviceState
{
    DEVICE_NULL            = 0,
    DEVICE_INITIALIZED     = 1,
    DEVICE_SEARCHING       = 2,
    DEVICE_REGISTERING     = 3,
    DEVICE_UNREGISTERING   = 4,
    DEVICE_CONNECTED       = 5,
    DEVICE_LOST_CONNECTION = 6,
    DEVICE_AP_MODE         = 7
};


typedef struct sRegionDeviceInfo_t
{
    struct sRegionDeviceInfo_t * pNextRegionDeviceInfo;
    
    DEVICE_ID deviceID;
    BYTE deviceRole;
    BYTE receiveSignalStrength;
    
} sRegionDeviceInfo;

typedef struct sDeviceInfo_t
{
    sRegionDeviceInfo * pRegionDeviceInfo;
    
    DEVICE_ID myDeviceID;
    DEVICE_ID accessPointDeviceID;
    
    DWORD     heartbeatLastTime;
    
    BYTE deviceTypeID;
    BYTE DeviceVersion;
    BYTE deviceRole;
    BYTE deviceState;
} sDeviceInfo;

typedef struct sMessage_t
{
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    WORD      messageTotalLength;
    BYTE    * pMessageBody;
} sMessage;


enum sendSessionStates
{
    SEND_SESSION_NULL                   = 0,
    SEND_SESSION_SENT_HEADER            = 1,
    SEND_SESSION_WAITING_FOR_HEADER_ACK = 2,
    SEND_SESSION_SENT_MULTI             = 3,
    SEND_SESSION_WAITING_FOR_MULTI_ACK  = 4,
    SEND_SESSION_SENT_IT_ALL            = 5,
    SEND_SESSION_CALLED_APP_CODE        = 6,
    SEND_SESSION_WAITING_TO_END         = 7
};

enum receiveSessionStates
{
    RECEIVE_SESSION_NULL            = 0,
    RECEIVE_SESSION_GOT_HEADER      = 1,
    RECEIVE_SESSION_GOT_MULTI       = 2,
    RECEIVE_SESSION_GOT_IT_ALL      = 3,
    RECEIVE_SESSION_CALLED_APP_CODE = 4,
    RECEIVE_SESSION_WAITING_TO_END  = 5
};

typedef struct sSendManager_t
{
    BYTE      sessionState;
    DEVICE_ID apDeviceID;
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
    DEVICE_ID apDeviceID;
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
int  listDeleteNode (sManager * * ppManagerList, sManager * pManagerToDelete);
void listWalk (sManager * pManagerList, sSendManager * * ppSendManager, DWORD hash, DEVICE_ID sourceID);
int  listLength (sManager * pManagerList);


#endif /* interface_hpp */


// MAX_MESSAGE_LENGTH