#include <Arduino.h>
#include <ESP32Servo.h>

// Define the pin connected to the ESC signal wire
const int escPin = 18;  // Change this as per your wiring
const int escPin2 = 19;

Servo esc;
Servo esc2;

void setup() {
  Serial.begin(115200);

  esc.attach(escPin);
  esc2.attach(escPin2);

  Serial.println("Starting ESC calibration...");

  // Step 1: Send max throttle
  esc.writeMicroseconds(2000);  // Max throttle (adjust if needed)
  esc2.writeMicroseconds(2000);
  Serial.println("Sending MAX throttle (2000us) for calibration...");
  delay(5000);  // Wait 5 seconds for ESC to enter calibration mode

  // Step 2: Send min throttle
  esc.writeMicroseconds(1000);  // Min throttle
  esc2.writeMicroseconds(1000);
  Serial.println("Sending MIN throttle (1000us) to set min throttle...");
  delay(5000);  // Wait 5 seconds for ESC to record min throttle

  Serial.println("Calibration sequence complete.");

  // Step 3: Test low-speed spin
  esc.writeMicroseconds(1100);  // Low speed (adjust based on your ESC)
  esc2.writeMicroseconds(1100);
  Serial.println("Running motor at low speed...");

  // No further code in setup
}

void loop() {
  // // Check if data is available in Serial Monitor
  // if (Serial.available()) {
  //   String input = Serial.readStringUntil('\n');  // Read input until newline
  //   input.trim();

  //   if (input.length() > 0) {
  //     int throttle = input.toInt();

  //     // Safety clamp to valid range
  //     if (throttle < 1000) throttle = 1000;
  //     if (throttle > 2000) throttle = 2000;

  //     esc2.writeMicroseconds(throttle);
  //     Serial.print("Throttle set to: ");
  //     Serial.println(throttle);
  //   }
  // }
}
