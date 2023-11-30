#define EL 5
#define SIGNAL 3
#define ZF 4
#define VR 2 

int pos=0;int steps=0;int speed1=0;
String direction1;
//#######################################
//#######################################
void plus() {
  pos++; //count steps
  Serial.print("Count: ");
  Serial.println(pos);
//    if(pos>=steps){
//    wheelStop();
//    pos=0;
//  }
}

void wheelMoveForward(){
      analogWrite(VR, speed1);
      digitalWrite(EL,LOW);
      delay(1000);
      digitalWrite(ZF,HIGH);
      delay(1000);
      digitalWrite(EL,HIGH);
      Serial.println("Starting forward");
}

void wheelMoveBackward(){
      analogWrite(VR, speed1);
      digitalWrite(EL,LOW);
      delay(1000);
      digitalWrite(ZF,LOW);
      delay(1000);
      digitalWrite(EL,HIGH);
      Serial.println("Starting backward");
}

void wheelStop(){
  Serial.println("Stopping");
  digitalWrite(EL,LOW);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(EL,OUTPUT);
  pinMode(SIGNAL,INPUT);
  pinMode(ZF,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SIGNAL),plus,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    speed1 = int(Serial.read());
    wheelMoveForward();
    delay(5000);
    wheelStop();
    delay(1000);
    pos=0;

    wheelMoveBackward();
    delay(5000);
    wheelStop();
    delay(1000);
    pos = 0;

    speed1=0;
    
  }
}
