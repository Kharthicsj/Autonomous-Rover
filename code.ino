#include <Servo.h>
#include <AFMotor.h>

// Define motors on the motor shield
AF_DCMotor motor1(1);  // Left side front
AF_DCMotor motor2(2);  // Left side back
AF_DCMotor motor3(3);  // Right side front
AF_DCMotor motor4(4);  // Right side back

#define Echo A0
#define Trig A1
#define motorPin 10
#define Speed 170
#define spoint 90
#define THRESHOLD 30  // Distance threshold in cm for stopping

int distance;
int LeftDistance;
int RightDistance;

Servo servo;

void setup() {
  Serial.begin(9600);  
  servo.attach(motorPin);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  Serial.println("Ultrasonic Obstacle Avoiding Robot Initialized");
  
  forward(); // Start moving forward by default
}

void loop() {
  distance = readUltrasonic();
  
  if (distance > 0) {  // Only print valid distances
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  if (distance <= THRESHOLD && distance > 0) { // Obstacle detected within 30 cm
    Stop();
    delay(100); // Stabilize stop
    scanAndMove();
  } 
}

int readUltrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long duration = pulseIn(Echo, HIGH);

  if (duration == 0 || duration > 38000) { // Check for timeout
    return -1;
  } else {
    int cm = duration * 0.034 / 2;  // Convert time to distance in cm
    return cm;
  }
}

void scanAndMove() {
  LeftDistance = checkLeft();
  RightDistance = checkRight();
  
  if (LeftDistance > RightDistance && LeftDistance > THRESHOLD) {
    left();
    delay(500); 
  } else if (RightDistance > LeftDistance && RightDistance > THRESHOLD) {
    right();
    delay(500); 
  } else { // Both sides blocked or too close, move backward
    backward();
    delay(500);
  }
  
  Stop();
  delay(200);
  forward(); // Resume forward movement
}

void forward() {
  motor1.setSpeed(Speed);
  motor2.setSpeed(Speed);
  motor3.setSpeed(Speed);
  motor4.setSpeed(Speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void backward() {
  motor1.setSpeed(Speed);
  motor2.setSpeed(Speed);
  motor3.setSpeed(Speed);
  motor4.setSpeed(Speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void right() {
  motor1.setSpeed(Speed);
  motor2.setSpeed(Speed);
  motor3.setSpeed(Speed);
  motor4.setSpeed(Speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void left() {
  motor1.setSpeed(Speed);
  motor2.setSpeed(Speed);
  motor3.setSpeed(Speed);
  motor4.setSpeed(Speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void Stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

int checkRight() {
  servo.write(20); // Look right
  delay(500);
  int rightDistance = readUltrasonic();
  if (rightDistance > 0) {
    Serial.print("Right side distance: ");
    Serial.println(rightDistance);
  }
  servo.write(spoint); // Reset position
  delay(200);
  return rightDistance;
}

int checkLeft() {
  servo.write(160); // Look left
  delay(500);
  int leftDistance = readUltrasonic();
  if (leftDistance > 0) {
    Serial.print("Left side distance: ");
    Serial.println(leftDistance);
  }
  servo.write(spoint); // Reset position
  delay(200);
  return leftDistance;
}
