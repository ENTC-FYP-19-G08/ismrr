#include "hub_wheel.h"

hub_wheel::hub_wheel(uint8_t SIGNAL_, uint8_t ZF_, uint8_t VR_, uint8_t EL_, uint8_t wheel_type_){
    SIGNAL = SIGNAL_;
    ZF = ZF_;
    VR = VR_;
    EL = EL_;
    wheel_type = wheel_type_;

    pinMode(EL,OUTPUT);
    pinMode(SIGNAL,INPUT);
    pinMode(VR,OUTPUT);
    pinMode(ZF,OUTPUT);
}



void hub_wheel::drive(){
    if(pre_pwm_vel != pwm_vel){
      
      if(pwm_vel>0){

        if(direction == BACKWARD || direction == STOP){
          digitalWrite(EL,LOW);
          direction = FORWARD;
        }  

        analogWrite(VR, pwm_vel);  
        delay(10);
        digitalWrite(ZF,(wheel_type)?LEFT_FORWARD:RIGHT_FORWARD);
        delay(10);
        digitalWrite(EL,HIGH);

        // Serial.println("Forward right");
      }

      else if(pwm_vel<0){

        if(direction == FORWARD || direction == STOP){
          digitalWrite(EL,LOW);
          direction = BACKWARD;
        } 

        analogWrite(VR, abs(pwm_vel));  
        delay(10);
        digitalWrite(ZF,(wheel_type)?LEFT_BACKWARD:RIGHT_BACKWARD);
        delay(10);
        digitalWrite(EL,HIGH);

        // Serial.println("Backward right");
      }

      else if(pwm_vel == 0){
        digitalWrite(EL,LOW);
        direction = STOP;
        count = 0;
        pre_count = 0;
        // Serial.println("Stopping right");
      }
      pre_pwm_vel = pwm_vel;
  }
}

void hub_wheel:: calPWM(){

  float error = target_velocity - velocity;
  
  pwm_vel = error*KP + sum_of_errors*KI;

  if(abs(pwm_vel)>200){
    if(pwm_vel>0){pwm_vel = MAX_PWM;}
    else if(pwm_vel<0){pwm_vel = -MAX_PWM;}
  }
  
}