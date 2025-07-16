#include <Arduino.h>
#include "rcmonitor.h"

  #include <NimBLEDevice.h>

#define CMD_TYPE_REMOVE_ALL 0
#define CMD_TYPE_REMOVE 1
#define CMD_TYPE_ADD_INCOMPLETE 2
#define CMD_TYPE_ADD 3
#define CMD_TYPE_UPDATE_ALL 4
#define CMD_TYPE_UPDATE 5

#define CMD_RESULT_OK 0
#define CMD_RESULT_PAYLOAD_OUT_OF_SEQUENCE 1
#define CMD_RESULT_EQUATION_EXCEPTION 2

#define MAX_REMAINING_PAYLOAD 2048
#define MAX_PAYLOAD_PART 17

#define MAIN_SERVICE_NAME           (uint16_t) 0x1ff8
#define CONFIG_CHARACTERISTIC       (uint16_t) 0x05
#define NOTIFICATION_CHARACTERISTIC (uint16_t) 0x06

  static NimBLEServer* pServer;
  static NimBLECharacteristic* monitorConfigCharacteristic;
  static NimBLECharacteristic* monitorNotificationCharacteristic;

boolean wasConnected = true;
boolean monitorConfigStarted = false;
int displayUpdateCount = 0;
int nextMonitorId = 0;



boolean sendConfigCommand(int cmdType, int monitorId, const char* payload, int payloadSequence = 0) {

    Serial.println("sendConfigCommand");
    Serial.print("cmdType ");
    Serial.println(cmdType);
    Serial.print("monitorId ");
    Serial.println(monitorId);
    Serial.print("payload ");
    Serial.println(payload);
    Serial.print("payloadSequence ");
    Serial.println(payloadSequence);

    // Initially use CMD_TYPE_ADD instead of CMD_TYPE_ADD_INCOMPLETE
    cmdType = cmdType == CMD_TYPE_ADD_INCOMPLETE ? CMD_TYPE_ADD : cmdType;

    Serial.print("final cmdType ");
    Serial.println(cmdType);

    // Figure out payload
    char* remainingPayload = NULL; 
    char payloadPart[MAX_PAYLOAD_PART + 1];
    if (payload && cmdType == CMD_TYPE_ADD) {
        // Copy first 17 characters to payload
        strncpy(payloadPart, payload, MAX_PAYLOAD_PART);
        payloadPart[MAX_PAYLOAD_PART] = '\0';
        
         // If it does not fit to one payload, save the remaining part
        int payloadLen = strlen(payload);
        if (payloadLen > MAX_PAYLOAD_PART) {
            int remainingPayloadLen = payloadLen - MAX_PAYLOAD_PART;
            remainingPayload = (char*)malloc(remainingPayloadLen + 1);
            strncpy(remainingPayload, payload + MAX_PAYLOAD_PART, remainingPayloadLen);
            remainingPayload[remainingPayloadLen] = '\0';
            cmdType = CMD_TYPE_ADD_INCOMPLETE;
        }
    } else {
        payloadPart[0] = '\0';
    }
  
    // Indicate the characteristic
    byte bytes[20];
    bytes[0] = (byte)cmdType;
    bytes[1] = (byte)monitorId;
    bytes[2] = (byte)payloadSequence;
    memcpy(bytes + 3, payloadPart, strlen(payloadPart));

    monitorConfigCharacteristic->setValue(bytes, 3 + strlen(payloadPart));
    monitorConfigCharacteristic->indicate();
    delay(50); 

    // Handle remaining payload
    if (remainingPayload) {
        boolean r = sendConfigCommand(CMD_TYPE_ADD, monitorId, remainingPayload, payloadSequence + 1);
        free(remainingPayload);
        return r;
    } else {
        return true;
    }
}

boolean addMonitor(const char* monitorName, const char* filterDef, float multiplier) {
    if (nextMonitorId < MONITORS_MAX) {
        if (!sendConfigCommand(CMD_TYPE_ADD, nextMonitorId, filterDef)) {
            return false;
        }
        strncpy(monitorNames[nextMonitorId], monitorName, MONITOR_NAME_MAX);
        monitorNames[nextMonitorId][MONITOR_NAME_MAX] = '\0';
        monitorMultipliers[nextMonitorId] = multiplier;
        nextMonitorId++;
    }
    return true;
}

boolean configureMonitors() {
    // Configure monitors
    nextMonitorId = 0;
    for (uint8_t i=0; i<(sizeof monitors/sizeof monitors[0]); i++) {
        if ( ! addMonitor(monitors[i].nam, monitors[i].def, monitors[i].mul) ) {
          return false;
        }
    }
    activeMonitors = nextMonitorId;
    return true;    
}

class monitorConfigWriteCallback: public NimBLECharacteristicCallbacks {

    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo &connInfo) {
        if (pCharacteristic->getValue().length() >= 1) {
            int result = pCharacteristic->getValue()[0];
            int monitorId = pCharacteristic->getValue()[1];
            switch (result) {
                case CMD_RESULT_PAYLOAD_OUT_OF_SEQUENCE:
                    Serial.println("");
                    Serial.print(monitorId);
                    Serial.println(" out-of-sequence");
                    break;
                case CMD_RESULT_EQUATION_EXCEPTION:
                    // Notice exception details available in the data
                    Serial.println("");
                    Serial.print(pCharacteristic->getValue()[1]);
                    Serial.println(" exception");
                    break;
                default:
                    break;
            }
        }
    };
};

class monitorNotificationWriteCallback: public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo &connInfo) {
        int dataPos = 0;
        while (dataPos + 5 <= pCharacteristic->getValue().length()) {
            int monitorId = (int)pCharacteristic->getValue()[dataPos];
            int32_t value = pCharacteristic->getValue()[dataPos + 1] << 24 | pCharacteristic->getValue()[dataPos + 2] << 16 | pCharacteristic->getValue()[dataPos + 3] << 8 | pCharacteristic->getValue()[dataPos + 4];
            if (monitorId < nextMonitorId) {
                monitorValues[monitorId] = value;
            }
            dataPos += 5;
        }
    };
};

void handleConnected() {
    // Reset state
    for (int i = 0; i < MONITORS_MAX; i++) {
        monitorValues[i] = INVALID_VALUE;
    }
    monitorConfigStarted = false;
    displayUpdateCount = 0;
    Serial.println("BLE connected!");
    delay(1500);
}


void handleDisconnected() {
    Serial.println("No BLE connection");
}

boolean handleConfigure() {
    // Start configuring when inidicate is enabled for the first time 
    if (!monitorConfigStarted) {
        boolean isIndicating = pServer->getConnectedCount();     
        if (isIndicating) {
            // Configure in loop until success or disconnected
            monitorConfigStarted = true;
            while (1) {
                Serial.print("Configuring ... ");    
                if (pServer->getConnectedCount()>0 && configureMonitors()) {
                    Serial.println("Done");    
                    return true;
                }
                Serial.println("Fail");    
            }
        }
        return false;
    }
    return true;
}

static monitorConfigWriteCallback cbmonitorConfigWriteCallback;
static monitorNotificationWriteCallback cbmonitorNotificationWriteCallback;

void bluetoothStart(void) {

    NimBLEDevice::init("RC DIY");
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT); 
    pServer = NimBLEDevice::createServer();
    NimBLEService* pMainService = pServer->createService(MAIN_SERVICE_NAME);
  
    monitorConfigCharacteristic = pMainService->createCharacteristic( 
                CONFIG_CHARACTERISTIC, 
                NIMBLE_PROPERTY::READ | 
                NIMBLE_PROPERTY::WRITE | 
                NIMBLE_PROPERTY::INDICATE 
                );
    monitorConfigCharacteristic->setCallbacks(&cbmonitorConfigWriteCallback);

    monitorNotificationCharacteristic = pMainService->createCharacteristic( 
                NOTIFICATION_CHARACTERISTIC, 
                NIMBLE_PROPERTY::WRITE_NR
                );
    monitorNotificationCharacteristic->setCallbacks(&cbmonitorNotificationWriteCallback);
    
    pMainService->start();
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    String str = std::string(NimBLEAddress(NimBLEDevice::getAddress())).c_str();
    char name[255];
    snprintf(name, sizeof(name), "RC DIY #%02X%02X", str[4], str[5]);
    pAdvertising->setName(name);
    pAdvertising->setMinInterval(160);
    pAdvertising->setMaxInterval(160);
    pAdvertising->addServiceUUID(pMainService->getUUID());

    /*NimBLEAdvertisementData scanResponseData;
    scanResponseData.setName(name);  // mismo nombre en respuesta al escaneo, opcional
    pAdvertising->setScanResponseData(scanResponseData);*/

    pAdvertising->start();
}

void rcmonitorstart() {
    bluetoothStart();
}

boolean rcmonitor() {
    // Monitor change in Bluetooth connection status
    boolean isConnected = pServer->getConnectedCount();

    if (wasConnected != isConnected) {
        wasConnected = isConnected;

        // Print Bluetooth connection status
        if (isConnected) {
            handleConnected();       
        } else {
            handleDisconnected();
        }
    }
    if (isConnected) {
        // Try configuring
        return handleConfigure();
    }
	return 0;
}


// end 
