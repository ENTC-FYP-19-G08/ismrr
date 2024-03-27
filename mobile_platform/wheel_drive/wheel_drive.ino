#include "src/HubWheel/HubWheel.h"
#include "src/CustomServo/CustomServo.hpp"

// Pins for the right motor
#define right_SIGNAL 3 
#define right_VR 13    
#define right_ZF 12
#define right_EL 11

// Pins for the left motor
#define left_SIGNAL 2
#define left_VR 10 
#define left_ZF 9
#define left_EL 8

// wheel indexes
#define RIGHT_WHEEL 0
#define LEFT_WHEEL 1

// timeout
#define TIME_OUT 500

// Servo motors
#define head_servo_PIN 46
#define STEP 1
#define STEP_DELAY 50

double timeout_t = 0;

// Power state variable
int power_state = 0;

// Head servo angle
int head_servo_angle = 90;
int previous_head_servo_angle = 90;

//wheel objects
HubWheel right_wheel(right_SIGNAL,right_ZF,right_VR,right_EL,RIGHT_WHEEL);
HubWheel left_wheel(left_SIGNAL,left_ZF,left_VR,left_EL,LEFT_WHEEL);

//Head servo object
CustomServo headServo;

void right_encoder(){

    /*
        interrupt function for right motor encoder
    */

    if(right_wheel.direction == FORWARD){right_wheel.count += 1;}
    else if(right_wheel.direction == BACKWARD){right_wheel.count -= 1;}

    //right_wheel.velocity = (right_wheel.count - right_wheel.pre_count)*MILIMETERS_PER_TICK/(millis()-right_wheel.t);
    
    if(abs(right_wheel.count)>MAX_COUNT){
        right_wheel.count = 0;
    }
    
    //right_wheel.pre_count = right_wheel.count;
    //right_wheel.t = millis();
}

void left_encoder(){

    /*
        interrupt function for left motor encoder
    */

    if(left_wheel.direction == FORWARD){left_wheel.count += 1;}
    else if (left_wheel.direction == BACKWARD){left_wheel.count -= 1;}

    // left_wheel.velocity = (left_wheel.count - left_wheel.pre_count)*MILIMETERS_PER_TICK/(millis()-left_wheel.t);
    
    if(abs(left_wheel.count)>MAX_COUNT){
        left_wheel.count = 0;
    }
    
    // left_wheel.pre_count = left_wheel.count;
    // left_wheel.t = millis();
}

void moveServo(){
  if(head_servo_angle != previous_head_servo_angle){
        while(headServo.move(head_servo_angle,STEP_DELAY,STEP));
        previous_head_servo_angle = head_servo_angle;
    }
}

void readMsg(){
  /*
   * This function reads velocities, head servo angle and power state from serial when available
   * Message is sent as (float,float,int,int)=>(left_wheel_vel, right_wheel_vel,head_servo_angle,power_state)
  */
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');

    // Parse the data using strtok function
    char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

    // Loop through the tokens and convert them 
    for (int i = 0; i < 4 && ptr != NULL; i++){
      if(i==0){left_wheel.target_velocity = atof(ptr);}
      if(i==1){right_wheel.target_velocity = atof(ptr);}   
      if(i==2){head_servo_angle = atoi(ptr);}
      if(i==3){power_state = atoi(ptr);}

      ptr = strtok(NULL,",");
   }
   Serial.println(String(left_wheel.count)+" "+String(right_wheel.count));
   timeout_t = millis();
  }  
}

// void readPWM(){
//   /*
//    * Can be used for testings
//    * This function reads pwm values from serial when available
//    * pwm values are sent as (int,int)=>(left_wheel_pwm, right_wheel_pwm)
//   */
//   if(Serial.available()>0){
//     String data = Serial.readStringUntil('\n');

//     // Parse the data using strtok function
//     char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

//     // Loop through the tokens and convert them to integers
//     for (int i = 0; i < 2 && ptr != NULL; i++){
//       if(i==1){right_wheel.pwm_vel = atoi(ptr);}
//       if(i==0){left_wheel.pwm_vel = atoi(ptr);}
// //      Serial.println(velocity[i]);
//       ptr = strtok(NULL,",");
//    }
//    Serial.println(String(left_wheel.count)+" "+String(right_wheel.count));//delay(5);
//   }  
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(5);

  attachInterrupt(digitalPinToInterrupt(right_SIGNAL),right_encoder,CHANGE);
  attachInterrupt(digitalPinToInterrupt(left_SIGNAL),left_encoder,CHANGE);

  headServo.attach(head_servo_PIN);
  moveServo();

  right_wheel.t = millis();
  left_wheel.t = millis();
  right_wheel.direction = STOP;
  left_wheel.direction = STOP;
  
  
}

void loop() {

    readMsg();
    // readPWM();
    right_wheel.calPWM();
    right_wheel.drive();
    left_wheel.calPWM();
    left_wheel.drive();
    moveServo();
    
    
    
//   if((millis()-timeout_t) >= TIME_OUT){
//     right_wheel.target_velocity = 0;
//     left_wheel.target_velocity = 0;
//   }

    // debugging serial output
    //Serial.println("right = "+String(right_wheel.velocity)+", dir = "+String(right_wheel.direction)+", left = "+String(left_wheel.velocity)+", dir = "+String(left_wheel.direction));
  
}
