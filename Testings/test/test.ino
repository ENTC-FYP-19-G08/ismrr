#define right_SIGNAL 2 
#define right_VR 13    
#define right_ZF 12
#define right_EL 11

#define left_SIGNAL 3
#define left_VR 10 
#define left_ZF 9
#define left_EL 8

#define MILIMETERS_PER_TICK 5.4598

#define FORWARD 1
#define BACKWARD 0
#define STOP 3

// wheel indexes
#define RIGHT_WHEEL 0
#define LEFT_WHEEL 1

volatile int right_count = 0, left_count = 0, pre_right_count = 0, pre_left_count = 0;
int right_direction = 0,left_direction = 0;

float velocity[2] = {0,0};
float target_velocity[2] = {0,0};
int pwm_vel[2] = {0,0};
int previous_pwm[2] = {0,0};

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

long right_t = millis(),left_t = millis();

void rightEncoder(){
  
  if(right_direction == FORWARD){right_count += 1;}
  else{right_count -= 1;}

  velocity[RIGHT_WHEEL] = (right_count - pre_right_count)*MILIMETERS_PER_TICK/(millis()-right_t);
  
  if(abs(right_count)>32000){
    right_count = 0;
  }
  
  pre_right_count = right_count;
  right_t = millis();
  //Serial.println("right count = "+String(right_count)+"left Count = "+String(left_count));
}

void leftEncoder(){
  
  if(left_direction == FORWARD){left_count += 1;}
  else{left_count -= 1;}

  velocity[LEFT_WHEEL] = (left_count - pre_left_count)*MILIMETERS_PER_TICK/(millis()-left_t);
  
  if(abs(left_count)>32000){
    left_count = 0;
  }

  pre_left_count = left_count;
  left_t = millis();
  //Serial.println("right count = "+String(right_count)+"left Count = "+String(left_count));
}

void driveRight(){

  if(previous_pwm[0] != pwm_vel[0]){
      
      if(pwm_vel[0]>0){
        right_direction = FORWARD;
        if(previous_pwm[0]*pwm_vel[0]<0){
          digitalWrite(right_EL,LOW);
        }     
        analogWrite(right_VR, pwm_vel[0]);  
        delay(10);
        digitalWrite(right_ZF,LOW);
        delay(10);
        digitalWrite(right_EL,HIGH);
//        Serial.println("Forward right");
      }
      else if(pwm_vel[0]<0){
        right_direction = BACKWARD;
        if(previous_pwm[0]*pwm_vel[0]<0){
          digitalWrite(right_EL,LOW);
        } 
        analogWrite(right_VR, abs(pwm_vel[0]));  
        delay(10);
        digitalWrite(right_ZF,HIGH);
        delay(10);
        digitalWrite(right_EL,HIGH);
//        Serial.println("Backward right");
      }
      else if(pwm_vel[0] == 0){
        digitalWrite(right_EL,LOW);
//        Serial.println("Stopping right");
      }
      previous_pwm[0] = pwm_vel[0];
  }
 
}

void driveLeft(){
  if(previous_pwm[1] != pwm_vel[1]){
    
    if(pwm_vel[1]>0){
      left_direction = FORWARD;
      if(previous_pwm[1]*pwm_vel[1]<0){
          digitalWrite(right_EL,LOW);
        } 
      analogWrite(left_VR, pwm_vel[1]); 
      delay(5);
      digitalWrite(left_ZF,HIGH);
      delay(5);
      digitalWrite(left_EL,HIGH);
//      Serial.println("Forward left");
    }
    else if(pwm_vel[1]<0){
      left_direction = BACKWARD;
      if(previous_pwm[1]*pwm_vel[1]<0){
          digitalWrite(right_EL,LOW);
        } 
      analogWrite(left_VR, abs(pwm_vel[1]));  
      delay(5);
      digitalWrite(left_ZF,LOW);
      delay(5);
      digitalWrite(left_EL,HIGH);
//      Serial.println("Backward left");
    }
    else if(pwm_vel[1] == 0){
      digitalWrite(left_EL,LOW);
//      Serial.println("Stopping left");
    } 
    previous_pwm[1] = pwm_vel[1];
  }
}

void readVel(){
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');

    // Parse the data using strtok function
    char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

    // Loop through the tokens and convert them to integers
    for (int i = 0; i < 2 && ptr != NULL; i++){
      target_velocity[i] = atof(ptr);
//      Serial.println(velocity[i]);
      ptr = strtok(NULL,",");
   }
   Serial.println(String(right_count)+","+String(left_count));//delay(5);
  }  
}

void readPWM(){
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');

    // Parse the data using strtok function
    char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

    // Loop through the tokens and convert them to integers
    for (int i = 0; i < 2 && ptr != NULL; i++){
      pwm_vel[i] = atoi(ptr);
//      Serial.println(velocity[i]);
      ptr = strtok(NULL,",");
   }
   Serial.println(String(right_count)+","+String(left_count));//delay(5);
  }  
}

int cal_pwm(int wheel){
  int pwm=0;
  float Kp = 10;
  float error = target_velocity[wheel] - velocity[wheel];
  
  pwm = error*Kp;

  if(abs(pwm)>200){
    if(pwm>0){pwm = 200;}
    else if(pwm<0){pwm = -200;}
  }
  
  return pwm;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(5);
  
  pinMode(left_EL,OUTPUT);
  pinMode(left_SIGNAL,INPUT);
  pinMode(left_VR,OUTPUT);
  pinMode(left_ZF,OUTPUT);

  pinMode(right_EL,OUTPUT);
  pinMode(right_SIGNAL,INPUT);
  pinMode(right_VR,OUTPUT);
  pinMode(right_ZF,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(right_SIGNAL),rightEncoder,CHANGE);
  attachInterrupt(digitalPinToInterrupt(left_SIGNAL),leftEncoder,CHANGE);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  readVel();
  while(abs(target_velocity[RIGHT_WHEEL]-velocity[RIGHT_WHEEL])/2+abs(target_velocity[LEFT_WHEEL]-velocity[LEFT_WHEEL])/2 > 0.01){
    pwm_vel[RIGHT_WHEEL] = cal_pwm(RIGHT_WHEEL);
    pwm_vel[LEFT_WHEEL] = cal_pwm(LEFT_WHEEL);
    driveRight();
    driveLeft();
  }
//  Serial.println("right = "+String(right_count)+"left = "+String(left_count));
  
}