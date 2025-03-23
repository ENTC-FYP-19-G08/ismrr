#include "CustomServo.hpp"

CustomServo servo;

void setup()
{
 
  // Bare minimum use case, this will use default values for period and step
  servo.init(3);
  while(servo.move(40, 0.02));

  // Configure period and step when initializing, useful if speed variation is low when moving
  servo.init(3, 90, 50, 2);
  while(servo.move(40, 0.02));

  // Configure period and step when moving, useful in wide range of speed variations
  servo.init(3);
  while(servo.move(40, 40, 3)); // speed = 3/40

}

void loop()
{
  
}

