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

volatile int right_count = 0, left_count = 0, pre_right_count = 0, pre_left_count = 0;
float right_speed = 0,left_speed = 0;
int right_direction = 0,left_direction = 0;
int velocity[2] = {0,0};
int previous_vel[2] = {0,0};

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

long right_t = millis(),left_t = millis();

void rightEncoder(){
  
  if(right_direction == FORWARD){right_count += 1;}
  else{right_count -= 1;}

  right_speed = (right_count - pre_right_count)*MILIMETERS_PER_TICK/(millis()-right_t);
  
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

  left_speed = (left_count - pre_left_count)*MILIMETERS_PER_TICK/(millis()-left_t);
  
  if(abs(left_count)>32000){
    left_count = 0;
  }

  pre_left_count = left_count;
  left_t = millis();
  //Serial.println("right count = "+String(right_count)+"left Count = "+String(left_count));
}

void driveRight(int vel){

  if(previous_vel[0] != velocity[0]){
      
      if(vel>0){
        right_direction = FORWARD;
        if(previous_vel[0]*velocity[0]<0){
          digitalWrite(right_EL,LOW);
        }     
        analogWrite(right_VR, vel);  
        delay(10);
        digitalWrite(right_ZF,LOW);
        delay(10);
        digitalWrite(right_EL,HIGH);
//        Serial.println("Forward right");
      }
      else if(vel<0){
        right_direction = BACKWARD;
        if(previous_vel[0]*velocity[0]<0){
          digitalWrite(right_EL,LOW);
        } 
        analogWrite(right_VR, abs(vel));  
        delay(10);
        digitalWrite(right_ZF,HIGH);
        delay(10);
        digitalWrite(right_EL,HIGH);
//        Serial.println("Backward right");
      }
      else if(vel == 0){
        digitalWrite(right_EL,LOW);
//        Serial.println("Stopping right");
      }
      previous_vel[0] = velocity[0];
  }
 
}

void driveLeft(int vel){
  if(previous_vel[1] != velocity[1]){
    
    if(vel>0){
      left_direction = FORWARD;
      if(previous_vel[1]*velocity[1]<0){
          digitalWrite(right_EL,LOW);
        } 
      analogWrite(left_VR, vel); 
      delay(5);
      digitalWrite(left_ZF,HIGH);
      delay(5);
      digitalWrite(left_EL,HIGH);
//      Serial.println("Forward left");
    }
    else if(vel<0){
      left_direction = BACKWARD;
      if(previous_vel[1]*velocity[1]<0){
          digitalWrite(right_EL,LOW);
        } 
      analogWrite(left_VR, abs(vel));  
      delay(5);
      digitalWrite(left_ZF,LOW);
      delay(5);
      digitalWrite(left_EL,HIGH);
//      Serial.println("Backward left");
    }
    else if(vel == 0){
      digitalWrite(left_EL,LOW);
//      Serial.println("Stopping left");
    } 
    previous_vel[1] = velocity[1];
  }
}

void readVel(){
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');

    // Parse the data using strtok function
    char *ptr = strtok(const_cast<char *>(data.c_str()), ",");

    // Loop through the tokens and convert them to integers
    for (int i = 0; i < 2 && ptr != NULL; i++){
      velocity[i] = atoi(ptr);
//      Serial.println(velocity[i]);
      ptr = strtok(NULL,",");
   }
   Serial.println(String(right_count)+","+String(left_count));//delay(5);
  }  
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
  driveRight(velocity[0]);
  driveLeft(velocity[1]);
  
  Serial.println("right = "+String(right_count)+"left = "+String(left_count));
  
}