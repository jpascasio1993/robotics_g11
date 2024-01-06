#include <Servo.h>

const int motorPinEnA=6; // connect motor driver EnA pin to arduino uno pin 6 (~ PWM)
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

String incomingByte; // data received from mobile app transmitted through bluetooth

int motorSpeedLeft = 0;
int motorSpeedRight = 0;

int servoDegree=0;

Servo servo;

int setMotorSpeed(int speed) {
  return constrain(speed, 0, 255);
}

void turnOffLeftMotor() {
  motorSpeedLeft = setMotorSpeed(0);
  digitalWrite(motorPinIn1, LOW);
  digitalWrite(motorPinIn2, LOW);
  analogWrite(motorPinEnA, motorSpeedLeft);
}

void turnOffRightMotor() {
  motorSpeedRight = setMotorSpeed(0);
  digitalWrite(motorPinIn3, LOW);
  digitalWrite(motorPinIn4, LOW);
  analogWrite(motorPinEnB, motorSpeedRight);
}

void turnOnLeftMotor(bool isReverse) {
  digitalWrite(motorPinIn1, isReverse ? LOW : HIGH);
  digitalWrite(motorPinIn2, isReverse ? HIGH : LOW);
  // analogWrite(motorPinEnA, 255);
}

void turnOnRightMotor(bool isReverse) {
  digitalWrite(motorPinIn3, isReverse ? LOW : HIGH);
  digitalWrite(motorPinIn4, isReverse ? HIGH : LOW);
  // analogWrite(motorPinEnB, 255);
}

void accelerateMotor(int motorPin, int speed) {
  analogWrite(motorPin, setMotorSpeed(speed));
}

void accelerateLeft(int speed) {
  accelerateMotor(motorPinEnA, speed);
}

void accelerateRight(int speed) {
  accelerateMotor(motorPinEnB, speed);
}

void turnServo(int joystickDirection){
  switch(joystickDirection) {
    case JOYSTICK_LEFT:
      servoDegree = 10;
      servo.write(servoDegree); // turn servo motor to angle 10 degree
      break;
    case JOYSTICK_RIGHT:
      servoDegree=160;
      servo.write(servoDegree); // turn servo motor to angle 160 degree
      break;
  }
}

void turnDirection(int turnJoystickDirection, int speed) {
   switch(joystickDirection) {
    case JOYSTICK_LEFT:
      accelerateLeft(0);
      accelerateRight(speed);
      break;
    case JOYSTICK_RIGHT:
      accelerateRight(0);
      accelerateLeft(speed);
      break;
  }
}

void runMotor(int joystickDirection) {
  switch(joystickDirection) {
    case BUTTON_A_FORWARD:
        turnOnLeftMotor(false); // run motor forward
        turnOnRightMotor(false); // run motor forward
      break;

    case BUTTON_B_BACKWARD:
        turnOnLeftMotor(true); // run motor direction
        turnOnRightMotor(true); // run motor direction
      break;

    case STOP:
        turnOffLeftMotor();
        turnOffRightMotor();
      break;

    default:
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

  // if no data received from bluetooth, skip process
  if(!(Serial.available() > 0)) return;

  incomingByte = Serial.readString();
  int commandCode = (int)(incomingByte.toInt() / 1000);
  int pwm = setMotorSpeed((incomingByte.toInt() % 1000));
  // turnServo(incomingByte.toInt());
  delay(400);
  runMotor(commandCode);
  delay(400);
  turnDirection(commandCode, pwm);
  delay(400);
  Serial.println(commandCode);
  Serial.write("done"); // send "done" response to the app
  Serial.flush(); // flush serial
}