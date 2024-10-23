
#include <PS4Controller.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"

unsigned long lastTimeStamp = 0;
#define EVENTS 0
#define BUTTONS 1
#define JOYSTICKS 1
#define SENSORS 0

// Constants
const int joystickThreshold = 15;  // Threshold for joystick movement
const int debounceDelay = 100;     // Delay to prevent flooding the serial output
const int LED_PIN = 2;             // LED pin (use your desired pin)

// Setup Function
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);        // Initialize LED pin
  turnOffLED();                    // Turn off LED initially
  
  PS4.attach(notify);              // Attach the notify callback
  PS4.attachOnConnect(onConnect);  // Attach the connect callback
  PS4.attachOnDisconnect(onDisConnect);  // Attach the disconnect callback
  PS4.begin();                     // Start Bluetooth PS4 controller
  removePairedDevices();           // Clear previously paired devices to fix connection issues
  
  Serial.print("This device MAC is: ");
  printDeviceAddress();            // Print the MAC address of ESP32
  Serial.println("");
}

// Main loop
void loop() {
  delay(100);  // Simple delay to keep the loop running smoothly
}

// Function to remove paired devices (to fix connection issues)
void removePairedDevices() {
  uint8_t pairedDeviceBtAddr[20][6];
  int count = esp_bt_gap_get_bond_device_num();
  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
  for (int i = 0; i < count; i++) {
    esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
  }
}

// Function to print the Bluetooth MAC address of the ESP32
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
  Serial.println();
}

// Called when the PS4 controller disconnects
void onDisConnect() {
  Serial.println("PS4 Controller Disconnected!");
  turnOffLED();  // Turn off LED on disconnect
}

// Called when the PS4 controller connects
void onConnect() {
  Serial.println("PS4 Controller Connected!");
  turnOnLED();  // Turn on LED when connected
}

// Called whenever an event (joystick or button) happens
void notify() {
  unsigned long currentTime = millis();

  // Debouncing to prevent flooding the serial output
  if (currentTime - lastTimeStamp < debounceDelay) {
    return;
  }
  lastTimeStamp = currentTime;

  // Handling joystick inputs with a threshold
  handleJoysticks();
  
  // Handling button presses
  //handleButtons();
}

// Handle joystick movements
void handleJoysticks() {
  int rStickX = PS4.RStickX();  // Right stick horizontal movement
  int rStickY = PS4.RStickY();  // Right stick vertical movement
  int lStickX = PS4.LStickX();  // Left stick horizontal movement
  int lStickY = PS4.LStickY();  // Left stick vertical movement

  // Right Stick horizontal movement (Left/Right)
  if (abs(rStickX) > joystickThreshold) {
    if (rStickX < 0) {
      Serial.println("Moving left (Baame jao bhai)");
    } else {
      Serial.println("Moving right (Daine jao bhai)");
    }
  }

  // Left Stick vertical movement (Forward/Backward)
  if (abs(lStickY) > joystickThreshold) {
    if (lStickY < 0) {
      Serial.println("Moving backward (Piiche jao bhai)");
    } else {
      Serial.println("Moving forward (Shamne jao bhai)");
    }
  }
}

// Optional: Visual feedback using an LED
void turnOnLED() {
  digitalWrite(LED_PIN, HIGH);  // Turn on the LED
}

void turnOffLED() {
  digitalWrite(LED_PIN, LOW);   // Turn off the LED
}