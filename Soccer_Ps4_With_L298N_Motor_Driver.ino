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

int ENApin = 14; // motor 1 speed
int IN1pin = 27; // motor 1 dir1
int IN2pin = 26; // motor 1 dir2
int IN3pin = 25; // motor 2 dir1
int IN4pin = 33; // motor 2 dir2
int ENBpin = 32; // motor 2 speed

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
    int rightMotorSpeed, leftMotorSpeed;
  leftMotorSpeed = map( PS4.RStickX(), -127, 127, -255, 255); //Left stick  - y axis - forward/backward left motor movement
  rightMotorSpeed = map( PS4.LStickY(), -127, 127, -255, 255);  //Right stick - y axis - forward/backward right motor movement

  rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);
  leftMotorSpeed = constrain(leftMotorSpeed, -255, 255);


    

    if (PS4.RStickX() < 0) {
      Serial.println("Baame jao bhai");
          digitalWrite(IN1pin, LOW);
          digitalWrite(IN2pin, LOW);
          analogWrite(ENApin, 0);
          digitalWrite(IN3pin, HIGH);
         digitalWrite(IN4pin, LOW);
          analogWrite(ENBpin, leftMotorSpeed);
          Serial.println(leftMotorSpeed);

    
    }
    else if (PS4.RStickX() > 0) {
      Serial.println("Daine jao bhai");
          digitalWrite(IN1pin, HIGH);
          digitalWrite(IN2pin, LOW);
          analogWrite(ENApin, leftMotorSpeed);
          digitalWrite(IN3pin, LOW);
          digitalWrite(IN4pin, LOW);
          analogWrite(ENBpin, 0);
          Serial.println(leftMotorSpeed);

    
    }
    else if (PS4.LStickY() < 0) {
      Serial.println("Piiche jao bhai");
          digitalWrite(IN1pin, LOW);
          digitalWrite(IN2pin, HIGH);
          analogWrite(ENApin, rightMotorSpeed);
          digitalWrite(IN3pin, LOW);
          digitalWrite(IN4pin, HIGH);
          analogWrite(ENBpin, rightMotorSpeed);
            Serial.println(rightMotorSpeed);

    
    }
    else if (PS4.LStickY() > 0) {
      Serial.println("Shamne jao bhai");
       digitalWrite(IN1pin, HIGH);
        digitalWrite(IN2pin, LOW);
        analogWrite(ENApin, rightMotorSpeed);
        digitalWrite(IN3pin, HIGH);
        digitalWrite(IN4pin, LOW);
        analogWrite(ENBpin, rightMotorSpeed);
          Serial.println(rightMotorSpeed);

    
    }


   
}

