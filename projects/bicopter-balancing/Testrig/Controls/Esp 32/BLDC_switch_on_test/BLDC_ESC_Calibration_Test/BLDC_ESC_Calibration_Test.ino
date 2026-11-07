#include <ESP32Servo.h>

// Define the GPIO pin connected to the ESC signal line
const int ESC_PIN = 13; // Replace with the actual GPIO pin you're using

Servo esc; // Create a Servo object

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  esc.attach(ESC_PIN);  // Attach the Servo object to the specified pin

  // --- ESC Calibration Steps ---
  // These steps are crucial for the ESC to recognize its operating range.
  // Refer to your specific ESC's manual for the exact calibration procedure.

  // Step 1: Send the "maximum" signal to the ESC (e.g., 2000µs)
  esc.writeMicroseconds(2000);
  Serial.println("Sending max pulse to ESC...");
  delay(2000); // Give the ESC time to process the signal

  // Step 2: Send the "minimum" signal to the ESC (e.g., 1000µs)
  esc.writeMicroseconds(1000);
  Serial.println("Sending min pulse to ESC...");
  delay(2000); // Give the ESC time to process the signal

  // Step 3: Send the neutral signal to the ESC (e.g., 1500µs)
  esc.writeMicroseconds(1500);
  Serial.println("Motor ready, stopped at neutral.");
  delay(3000); // Allow ESC to initialize

  esc.writeMicroseconds(1400);
}

void loop() {
  // // --- Motor Control ---
  // Serial.println("Enter PWM signal value (1000-2000µs), 1500 to stop:");
  // while (Serial.available() == 0); // Wait for serial input

  // int val = Serial.parseInt(); // Read the integer value from serial

  // if (val >= 1000 && val <= 2000) {
  //   esc.writeMicroseconds(val); // Send the calibrated pulse width to the ESC
  //   Serial.print("Sending pulse width: ");
  //   Serial.println(val);
  // } else {
  //   Serial.println("Invalid input. Please enter a value between 1000 and 2000.");
  // }
  // delay(100); // Small delay to avoid overwhelming the serial port
}