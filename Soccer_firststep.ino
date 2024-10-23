#include <PS4Controller.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"

unsigned long lastTimeStamp = 0;
#define EVENTS 0
#define BUTTONS 0
#define JOYSTICKS 1
#define SENSORS 0

void setup() {
  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  removePairedDevices(); // This helps to solve connection issues
  Serial.print("This device MAC is: ");
  printDeviceAddress();
  Serial.println("");
}

void loop() {
  delay(100);
}
void removePairedDevices() {
  uint8_t pairedDeviceBtAddr[20][6];
  int count = esp_bt_gap_get_bond_device_num();
  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
  for (int i = 0; i < count; i++) {
    esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
  }
}

void printDeviceAddress() {
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02x", (int)point[i]);
    Serial.print(str);
    if (i < 5) {
      Serial.print(":");
    }
  }
}
void onDisConnect() {
  Serial.println("Disconnected!");
}
void onConnect() {
  Serial.println("Connected!");
}

void notify() {
    if (PS4.RStickX() < 0) {
      Serial.println("Baame jao bhai");

    
    }
    else if (PS4.RStickX() > 0) {
      Serial.println("Daine jao bhai");

    
    }
    else if (PS4.LStickY() < 0) {
      Serial.println("Piiche jao bhai");

    
    }
    else if (PS4.LStickY() > 0) {
      Serial.println("Shamne jao bhai");

    
    }


   
}

