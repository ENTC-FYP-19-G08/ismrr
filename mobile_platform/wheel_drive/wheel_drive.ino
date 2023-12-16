#include "src/hub_wheel/hub_wheel.h"

// Pins for the right motor
#define right_SIGNAL 2 
#define right_VR 13    
#define right_ZF 12
#define right_EL 11

// Pins for the left motor
#define left_SIGNAL 3
#define left_VR 10 
#define left_ZF 9
#define left_EL 8

// wheel indexes
#define RIGHT_WHEEL 0
#define LEFT_WHEEL 1

hub_wheel right_wheel(right_SIGNAL,right_ZF,right_VR,right_EL,RIGHT_WHEEL);
hub_wheel left_wheel(left_SIGNAL,left_ZF,left_VR,left_EL,LEFT_WHEEL);

void right_encoder(){
    if(right_wheel.direction == FORWARD){right_wheel.count += 1;}
    else{right_wheel.count -= 1;}

    right_wheel.velocity = (right_wheel.count - right_wheel.pre_count)*MILIMETERS_PER_TICK/(millis()-right_wheel.t);
    
    if(abs(right_wheel.count)>MAX_COUNT){
        right_wheel.count = 0;
    }
    
    right_wheel.pre_count = right_wheel.count;
    right_wheel.t = millis();
}

void left_encoder(){
    if(left_wheel.direction == FORWARD){left_wheel.count += 1;}
    else{left_wheel.count -= 1;}

    left_wheel.velocity = (left_wheel.count - left_wheel.pre_count)*MILIMETERS_PER_TICK/(millis()-left_wheel.t);
    
    if(abs(left_wheel.count)>MAX_COUNT){
        left_wheel.count = 0;
    }
    
    left_wheel.pre_count = left_wheel.count;
    left_wheel.t = millis();
}

void readVel(){
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');

    // Parse the data using strtok function
    char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

    // Loop through the tokens and convert them to integers
    for (int i = 0; i < 2 && ptr != NULL; i++){
      if(i==RIGHT_WHEEL){right_wheel.velocity = atof(ptr);}
      if(i==LEFT_WHEEL){left_wheel.velocity = atof(ptr);}
//      Serial.println(velocity[i]);
      ptr = strtok(NULL,",");
   }
   Serial.println(String(right_wheel.count)+","+String(left_wheel.count));//delay(5);
  }  
}

void readPWM(){
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');

    // Parse the data using strtok function
    char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

    // Loop through the tokens and convert them to integers
    for (int i = 0; i < 2 && ptr != NULL; i++){
      if(i==RIGHT_WHEEL){right_wheel.pwm_vel = atof(ptr);}
      if(i==LEFT_WHEEL){left_wheel.pwm_vel = atof(ptr);}
//      Serial.println(velocity[i]);
      ptr = strtok(NULL,",");
   }
   Serial.println(String(right_wheel.count)+","+String(left_wheel.count));//delay(5);
  }  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(5);
  
//   pinMode(left_EL,OUTPUT);
//   pinMode(left_SIGNAL,INPUT);
//   pinMode(left_VR,OUTPUT);
//   pinMode(left_ZF,OUTPUT);

//   pinMode(right_EL,OUTPUT);
//   pinMode(right_SIGNAL,INPUT);
//   pinMode(right_VR,OUTPUT);
//   pinMode(right_ZF,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(right_SIGNAL),right_encoder,CHANGE);
  attachInterrupt(digitalPinToInterrupt(left_SIGNAL),left_encoder,CHANGE);

  right_wheel.t = millis();
  left_wheel.t = millis();
  right_wheel.direction = STOP;
  left_wheel.direction = STOP;
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
//   readVel();
//   while(abs(target_velocity[RIGHT_WHEEL]-velocity[RIGHT_WHEEL])/2+abs(target_velocity[LEFT_WHEEL]-velocity[LEFT_WHEEL])/2 > 0.01){
//     pwm_vel[RIGHT_WHEEL] = cal_pwm(RIGHT_WHEEL);
//     pwm_vel[LEFT_WHEEL] = cal_pwm(LEFT_WHEEL);
//     driveRight();
//     driveLeft();
//   }
    readVel();
    right_wheel.calPWM();right_wheel.drive();
    left_wheel.calPWM();left_wheel.drive();
//  Serial.println("right = "+String(right_count)+"left = "+String(left_count));
  
}
