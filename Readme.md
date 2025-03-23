# Servo Control Library

This library can control servo speeds with smooth motion by allowing separate fine-tuning of the stepping period and stepping resolution, effectively addressing the smoothness issues at lower speed

## Examples
 
- Bare minimum use case, this will use default values for period and step
  ```
  servo.init(3);
  while(servo.move(40, 0.02));
  ```

- Configure period and step when initializing, useful if speed variation is low when moving
  ```
  servo.init(3, 90, 50, 2);
  while(servo.move(40, 0.02));
  ```

- Configure period and step when moving, useful in wide range of speed variations
  ```
  servo.init(3);
  while(servo.move(40, 40, 3)); // speed = 3/40
  ```
