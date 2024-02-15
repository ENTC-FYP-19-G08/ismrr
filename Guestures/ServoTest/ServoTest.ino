#include "CustomServo.hpp"

String inputString = "";    // a String to hold incoming data
bool inputComplete = false; // whether the string is complete

const int SERVO_SPEED_MAX = 15; // maximum speed of the servos
const int SERVO_COUNT = 1;

// maximum number of angles to load
const int servoPins[] = {3, 5, 6, 9, 11}; // pins for the servos
const int servoEnablePins[] = {2, 4, 7, 8, 10};
CustomServo servos[SERVO_COUNT]; // array of VarSpeedServo objects
int angles[SERVO_COUNT + 2];     // array to store the loaded angles
char sepChar = ' ';              // character used to separate the angles

void setup()
{
  Serial.begin(9600);
  inputString.reserve(200);
  delay(2000);

  for (int i = 0; i < SERVO_COUNT; i++)
  {
    servos[i].init(servoPins[i]);
    servos[i].move(90);
    pinMode(servoEnablePins[i], OUTPUT);
    digitalWrite(servoEnablePins[i], HIGH);
  }

  Serial.println("Ready");

  handShake();
}

void loop()
{
  if (inputComplete)
  {
    updateServos();
    inputString = "";
    inputComplete = false;
  }
}

void handShake()
{
  while (true)
  {

    while (servos[0].move(70, 15, 1))
      ;
    delay(200);

    while (servos[0].move(130, 15, 1))
      ;
    delay(200);

  }
}

void updateServos()
{
  int startIndex = 0;
  int endIndex = inputString.indexOf(sepChar);
  for (int i = 0; i < SERVO_COUNT + 2; i++)
  {
    if (endIndex == -1)
    {
      angles[i] = inputString.substring(startIndex).toInt();
      break;
    }
    else
    {
      angles[i] = inputString.substring(startIndex, endIndex).toInt();
      startIndex = endIndex + 1;
      endIndex = inputString.indexOf(sepChar, startIndex);
    }
  }

  for (int i = 0; i < SERVO_COUNT + 2; i++)
  {
    Serial.print(angles[i]);
    Serial.print(' ');
  }

  while (servos[0].move(angles[0], angles[1], angles[2]))
    ;

  //   bool wait = true;
  //   int time = 0;
  //   // while (wait && time <5000)
  //   while (wait)

  //   {
  // //    break;
  //     wait = false;
  //     for (int i = 0; i < SERVO_COUNT; i++)
  //     {
  //       if (servos[i].move(angles[i]))
  //         wait = true;
  //     }
  //     delay(2);
  //     time += 2;
  //   }

  Serial.println();
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar != '\n')
      inputString += inChar;
    else
      inputComplete = true;
  }
}
