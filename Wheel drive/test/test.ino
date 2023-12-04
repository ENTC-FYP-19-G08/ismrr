#define right_SIGNAL 2 
#define right_VR 13    
#define right_ZF 12
#define right_EL 11

#define left_SIGNAL 3
#define left_VR 10 
#define left_ZF 9
#define left_EL 8

#define FORWARD 1
#define BACKWARD 0
#define STOP 3

volatile int right_count = 0, left_count = 0;
int right_speed = 0,left_speed = 0;
int right_direction = 0,left_direction = 0;
int velocity[2] = {0,0};
int previous_vel[2] = {0,0};

long t = 0;

void rightEncoder(){
  
  if(right_direction == FORWARD){right_count += 1;}
  else{right_count -= 1;}
  
  if(abs(right_count)>32000){
    right_count = 0;
  }
  //Serial.println("right count = "+String(right_count)+"left Count = "+String(left_count));
}

void leftEncoder(){
  
  if(left_direction == FORWARD){left_count += 1;}
  else{left_count -= 1;}
  
  if(abs(left_count)>32000){
    left_count = 0;
  }
  //Serial.println("right count = "+String(right_count)+"left Count = "+String(left_count));
}

void driveRight(int vel){

  if(previous_vel[0] != velocity[0]){
      previous_vel[0] = velocity[0];
      if(vel>0){
        right_direction = FORWARD;
        digitalWrite(right_EL,LOW);
        analogWrite(right_VR, vel);  
        delay(500);
        digitalWrite(right_ZF,LOW);
        delay(500);
        digitalWrite(right_EL,HIGH);
        Serial.println("Forward right");
      }
      else if(vel<0){
        right_direction = BACKWARD;
        digitalWrite(right_EL,LOW);
        analogWrite(right_VR, abs(vel));  
        delay(500);
        digitalWrite(right_ZF,HIGH);
        delay(500);
        digitalWrite(right_EL,HIGH);
        Serial.println("Backward right");
      }
      else if(vel == 0){
        digitalWrite(right_EL,LOW);
        Serial.println("Stopping right");
      }
  }
 
}

void driveLeft(int vel){
  if(previous_vel[1] != velocity[1]){
    previous_vel[1] = velocity[1];
    if(vel>0){
      left_direction = FORWARD;
      digitalWrite(left_EL,LOW); 
      analogWrite(left_VR, vel); 
      delay(500);
      digitalWrite(left_ZF,HIGH);
      delay(500);
      digitalWrite(left_EL,HIGH);
      Serial.println("Forward left");
    }
    else if(vel<0){
      left_direction = BACKWARD;
      digitalWrite(left_EL,LOW);
      analogWrite(left_VR, abs(vel));  
      delay(500);
      digitalWrite(left_ZF,LOW);
      delay(500);
      digitalWrite(left_EL,HIGH);
      Serial.println("Backward left");
    }
    else if(vel == 0){
      digitalWrite(left_EL,LOW);
      Serial.println("Stopping left");
    } 
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
      Serial.println(velocity[i]);
      ptr = strtok(NULL,",");
   }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
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
  //Serial.println("right count = "+String(right_count)+"left Count = "+String(left_count));
  //delay(100);
}
