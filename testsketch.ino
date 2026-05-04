#include <Arduino.h>

#define IN1 26
#define IN2 27
#define ENA 25

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION);
  Serial.println("======================");
  Serial.println("  Motor Test System   ");
  Serial.println("======================");
  Serial.println("f = forward");
  Serial.println("r = reverse");
  Serial.println("s = stop");
  Serial.println("======================");
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(ENA, 255);
  Serial.println(">> FORWARD");
}

void reverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(ENA, 255);
  Serial.println(">> REVERSE");
}

void stopMotor() {
  ledcWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  Serial.println(">> STOPPED");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    Serial.print(">> Received: ");
    Serial.println(cmd);

    if (cmd == "f") {
      forward();
    } else if (cmd == "r") {
      reverse();
    } else if (cmd == "s") {
      stopMotor();
    } else {
      Serial.print(">> Unknown command: ");
      Serial.println(cmd);
    }
  }
}