#include <Servo.h>
/*
  RC Car Control System v1
  Controls:
  w = forward
  a = turn left
  s = reverse
  d = turn right
  e = stop

  Hardware:
  Arduino + L298N Motor Driver + 2 DC Motors + MG996R Servo
*/

// Front motor pins
const int ENA = 5;  // PWM speed pin
const int IN1 = 8;
const int IN2 = 9;

// Back motor pins
const int ENB = 6;  // PWM speed pin
const int IN3 = 10;
const int IN4 = 11;

// Servo
Servo steeringServo;
const int SERVO_PIN = 3;

// Steering angles
const int CENTER_ANGLE = 90;
const int LEFT_ANGLE = 50;
const int RIGHT_ANGLE = 130;
const int ANGLE_CHANGE = 5;

int currentSteering = CENTER_ANGLE;

// LED Dashboard
const int GREEN_LED = 2;
const int YELLOW_LED = 4;
const int BLUE_LED = 7;
const int RED_LED = 12;

// Car state
int speedValue = 0;  // -255 to 255

const int MIN_SPEED = -255;
const int MAX_SPEED = 255;
const int ACCELERATION = 10;

void setup() {
  Serial.begin(9600);

  // Motors
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Servo
  steeringServo.attach(SERVO_PIN);
  steeringServo.write(CENTER_ANGLE);

  // LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  stopCar();
  allLEDsOff();

  Serial.println("=== Arduino RC Car Ready ===");
  Serial.println("Controls:");
  Serial.println("w = forward");
  Serial.println("a = left");
  Serial.println("s = reverse");
  Serial.println("d = right");
  Serial.println("e = stop");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case 'w':
        moveForward();
        break;
      case 'a':
        turnLeft();
        break;
      case 'd':
        turnRight();
        break;
      case 's':
        moveBackward();
        break;
      case 'e':
        stopCar();
        break;
      default:
        break;
    }
    printStatus();
  }
}

void moveForward() {
  if (speedValue < 0) speedValue = 0;
  speedValue = constrain(speedValue + ACCELERATION, MIN_SPEED, MAX_SPEED);

  allLEDsOff();
  digitalWrite(GREEN_LED, HIGH);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  updatePWM();
}

void moveBackward() {
  if (speedValue > 0) speedValue = 0;
  speedValue = constrain(speedValue - ACCELERATION, MIN_SPEED, MAX_SPEED);

  allLEDsOff();
  digitalWrite(RED_LED, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  updatePWM();
}

void turnLeft() {
  currentSteering = constrain(currentSteering - ANGLE_CHANGE, LEFT_ANGLE, RIGHT_ANGLE);
  steeringServo.write(currentSteering);

  allLEDsOff();
  digitalWrite(YELLOW_LED, HIGH);
}

void turnRight() {
  currentSteering = constrain(currentSteering + ANGLE_CHANGE, LEFT_ANGLE, RIGHT_ANGLE);
  steeringServo.write(currentSteering);

  allLEDsOff();
  digitalWrite(BLUE_LED, HIGH);
}

void stopCar() {
  speedValue = 0;
  currentSteering = CENTER_ANGLE;

  allLEDsOff();

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  steeringServo.write(currentSteering);

  updatePWM();
}

void allLEDsOff() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void updatePWM() {
  analogWrite(ENA, abs(speedValue));
  analogWrite(ENB, abs(speedValue));
}

void printStatus() {
  Serial.println("----- RC Car Status -----");
  Serial.print("Direction: ");
  if (speedValue < 0)
    Serial.print("Backward ");
  else if (speedValue > 0)
    Serial.print("Forward ");
  else
    Serial.print("Stopped ");

  if (currentSteering < CENTER_ANGLE)
    Serial.println("Left");
  else if (currentSteering > CENTER_ANGLE)
    Serial.println("Right");
  else
    Serial.println("Center");


  Serial.print("Speed PWM: ");
  Serial.println(speedValue);

  Serial.print("Turn Angle: ");
  Serial.println(currentSteering);

  Serial.println("-------------------------");
}