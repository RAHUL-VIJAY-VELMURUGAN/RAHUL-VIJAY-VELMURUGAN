#include <ESP32Servo.h>

Servo esc;  

int escPin = 18;   // Signal wire from ESC to GPIO18 (can be any PWM-capable pin)
int speedValue = 1500;  // microseconds (1000 = stop, 2000 = full speed)

void setup() {
  Serial.begin(115200);

  // Attach ESC with min=1000, max=2000 µs
  esc.attach(escPin, 1000, 2000);

  Serial.println("Arming ESC...");
  esc.writeMicroseconds(1000);  // Send min throttle
  delay(3000);                  // Wait for ESC to arm
  Serial.println("ESC armed. Motor ready.");
}

void loop() {
  // Run motor at chosen speed
  esc.writeMicroseconds(speedValue);

  // Debugging
  Serial.print("Motor running at: ");
  Serial.print(speedValue);
  Serial.println(" µs");
  delay(1000);
}