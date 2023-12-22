#include "CustomServo.hpp"

String inputString = "";    // a String to hold incoming data
bool inputComplete = false; // whether the string is complete

const int SERVO_SPEED_MAX = 15; // maximum speed of the servos
const int SERVO_COUNT = 1;

         // maximum number of angles to load
const int servoPins[] = {3, 5, 6, 9, 11}; // pins for the servos
CustomServo servos[SERVO_COUNT];                   // array of VarSpeedServo objects
int angles[SERVO_COUNT+2];                             // array to store the loaded angles
char sepChar = ' ';                                  // character used to separate the angles
CustomServo servo;
void setup()
{
  Serial.begin(9600); 
  // Serial.println("Ready");
  servo.init(3);
  servo.trajInit(40, 1000);
  while(servo.trajFollow())
    ;
  // servo.trajInit(120, 1000);
  // while(servo.trajFollow())
  //   ;
  // while(servo.move(40,0.02));
}

void loop()
{
  
}

