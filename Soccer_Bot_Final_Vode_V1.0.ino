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

/*FOR RIGHT MOTOR*/
int R1PWM = 19;
int R2PWM = 21;

/*FOR LEFT MOTOR*/
int L1PWM = 23;
int L2PWM = 22;

//declare channel for pwm Output
#define R1 0
#define R2 1
#define L1 2
#define L2 3


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
  //output pin declare
  pinMode(R1PWM, OUTPUT);
  pinMode(R2PWM, OUTPUT);
  pinMode(L1PWM, OUTPUT);
  pinMode(L2PWM, OUTPUT);
  // Setup PWM channels
  ledcSetup(R1, 5000, 8);  // Channel 0 for Motor A, 5 kHz frequency, 8-bit resolution
  ledcAttachPin(R1PWM, R1);
  ledcSetup(R2, 5000, 8);  // Channel 1 for Motor B, 5 kHz frequency, 8-bit resolution
  ledcAttachPin(R2PWM, R2);
  ledcSetup(L1, 5000, 8);  // Channel 0 for Motor A, 5 kHz frequency, 8-bit resolution
  ledcAttachPin(L1PWM, L1);
  ledcSetup(L2, 5000, 8);  // Channel 1 for Motor B, 5 kHz frequency, 8-bit resolution
  ledcAttachPin(L2PWM, L2);
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
      ledcWrite(R1, 0);
      ledcWrite(R2, -leftMotorSpeed);

      ledcWrite(L1, -leftMotorSpeed);
      ledcWrite(L2, 0);
    }

    else if (PS4.RStickX() > 0) {
      Serial.println("Daine jao bhai");
      ledcWrite(R1, leftMotorSpeed);
      ledcWrite(L1, 0);

      ledcWrite(R2, 0);
      ledcWrite(L2, leftMotorSpeed);  
    }

    else if (PS4.LStickY() < 0) {
      Serial.println("Piiche jao bhai");
      Serial.println(-rightMotorSpeed);
      ledcWrite(R1, 0);
      ledcWrite(R2, -rightMotorSpeed);

      ledcWrite(L1, 0);
      ledcWrite(L2, -rightMotorSpeed);   
    }

    
    
    else if (PS4.LStickY() > 0) {
      Serial.println("Shamnee jao bhai");
      Serial.println(rightMotorSpeed);
      ledcWrite(R1, rightMotorSpeed);
      ledcWrite(R2, 0);
      ledcWrite(L1, rightMotorSpeed);
      ledcWrite(L2, 0);

    
    }
  else if (PS4.LStickY() == 0 && PS4.RStickX() == 0) {
      ledcWrite(R1, 0);
      ledcWrite(R2, 0);
      ledcWrite(L1, 0);
      ledcWrite(L2, 0);


   
}
