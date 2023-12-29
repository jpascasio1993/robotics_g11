#include <Servo.h>

const int motorPinEnA=6 // connect motor driver EnA pin to arduino uno pin 6 (~ PWM)
const int motorPinIn1=7; // connect motor driver in1 pin to arduino uno pin 7
const int motorPinIn2=8; // connect motor driver in2 pin to arduino uno pin 8

const int motorPinEnB=11; // connect motor driver EnA pin to arduino uno pin 11 (~ PWM)
const int motorPinIn3=12; // connect motor driver in3 pin to arduino uno pin 12
const int motorPinIn4=13; // connect motor driver in4 pin to arduino uno pin 13

const int servoPin=9; // connect servo motor PWM pin (ORANGE WIRE) to arduino uno pin 9
const int servoMinPWM=544; // 544 is equals to 0 degree
const int servoMaxPWM=2400; // 2400 is equals to 180 degree;
const int servoMidPWM=1200; // 1200 is equals to 90 degree;

const int serialBaudRate=9600;

const int JOYSTICK_LEFT=1;
const int JOYSTICK_RIGHT=2;

const int BUTTON_A_FORWARD=50;
const int BUTTON_B_BACKWARD=51;
const int STOP=52;


int incomingByte; // data received from mobile app transmitted through bluetooth

Servo servo;

void turnOffLeftMotor() {
  digitalWrite(motorPinIn1, LOW);
  digitalWrite(motorPinIn2, LOW);
}

void turnOffRightMotor() {
  digitalWrite(motorPinIn3, LOW);
  digitalWrite(motorPinIn4, LOW);
}

void turnOnLeftMotor(bool isReverse) {
  digitalWrite(motorPinIn1, isReverse ? LOW : HIGH);
  digitalWrite(motorPinIn2, isReverse ? HIGH : LOW);
}

void turnOnRightMotor(bool isReverse) {
  digitalWrite(motorPinIn3, isReverse ? LOW : HIGH);
  digitalWrite(motorPinIn4, isReverse ? HIGH : LOW);
}

void turnServo(int joystickDirection){
  switch(joystickDirection) {
    case JOYSTICK_LEFT:
      servo.write(10); // turn servo motor to angle 10 degree
      break;
    case JOYSTICK_RIGHT:
      servo.write(160) // turn servo motor to angle 160 degree
      break;
  }
}

void runMotor(int joystickDirection) {
  switch(joystickDirection) {
    case BUTTON_A_FORWARD:
        turnOnLeftMotor(false);
        turnOnRightMotor(false);
      break;

    case BUTTON_B_BACKWARD:
        turnOnLeftMotor(true);
        turnOnRightMotor(true);
      break;

    default:
        turnOffLeftMotor();
        turnOffRIghtMotor();
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin, servoMinPWM, servoMaxPWM);
  // initialize the servo to position at 90 degree
  servo.writeMicroseconds(servoMidPWM);

  // initialize bluetooth
  Serial.begin(serialBaudRate);

  // initialize pin mode
  pinMode(motorPinEnA, OUTPUT);
  pinMode(motorPinIn1, OUTPUT);
  pinMode(motorPinIn2, OUTPUT);

  pinMode(motorPinEnB, OUTPUT);
  pinMode(motorPinIn3, OUTPUT);
  pinMode(motorPinIn4, OUTPUT);

  turnOffLeftMotor();
  turnOffRightMotor();
}

void loop() {
  // put your main code here, to run repeatedly:

  // if no data received, skip process
  if(!(Serial.available() > 0)) return;

  incomingByte = Serial.parseInt();

  turnServo(incomingByte);
  runMotor(incomingByte);
}
