#include <Servo.h>
#include <IRremote.h>
#define IR1 A3
#define IR2 A4
#define IR3 A5
#define SM 2
Servo SVM;
IRrecv irrecv(A0);//수신부 입력핀 설정
decode_results results;//수신된 데이터 저장
const int TRIG=13;
const int ECHO=12;
int svmPosition=0;
const int ENA = 5;
const int ENB = 6;
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
int count =0;

void setup() {
 Serial.begin(9600);
 pinMode(13,OUTPUT);
 SVM.attach(2);
 SVM.write(0);
 pinMode(TRIG,OUTPUT);
 pinMode(ECHO,INPUT);
 pinMode(ENA,OUTPUT);
 pinMode(ENB,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT);
 pinMode(IN3,OUTPUT);
 pinMode(IN4,OUTPUT);
 pinMode(ECHO,OUTPUT);
 //digitalWrite(ENA,HIGH);
 //digitalWrite(ENB,HIGH);
 analogWrite(ENA,120);
 analogWrite(ENB,120);
 irrecv.enableIRIn();
 int remoteNum=0;
 do{
  if(irrecv.decode(&results)){
    if(remoteNum==0 && results.value == 0xFF6897) {
      remoteNum=1;
      SVM.write(0);
    }
    else if(remoteNum==1 && results.value == 0xFF30CF) {
      remoteNum=2;
      SVM.write(90);
    }
    else if(remoteNum==2 && results.value == 0xFF18E7) {
      remoteNum=3;
      SVM.write(180);
    }
    else if(remoteNum==3 && results.value == 0xFF7A85) {
      remoteNum=4;
      SVM.write(90);
    }
    else if(remoteNum == 4){
      if(results.value == remote_1){
    SVM.write(0);
    remoteNum=5;
      }
    else if(results.value == remote_2){
      SW.write(90);
      remoteNum=5;
    }
    else if(results.value == remote_3){
      SVM.write(180);
      remoteNum=5;
      }
    }
    
    Serial.println(results.value, HEX);
    Serial.println(" ");
    Serial.println(remoteNum);
    irrecv.resume();
    
  
  
 }while(remoteNum<4);
 
}

void measureDistance(){
  long duration,distance;
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
 
  duration=pulseIn(ECHO,HIGH);
  distance = duration * 340;  
  distance = (distance / 10000)/2; //us --> s로 변환(/1000000)
                                 //m --> cm로 변환(/10000)
                                 //왕복 -->편도 변환(/2)
  Serial.println(distance);
  Serial.println("cm"); 

  if(distance<5){
    if(svmPosition==0){
      SVM.write(180);
      svmPosition=180;
    }
    else if(svmPosition==180){
      SVM.write(0);
      svmPosition=0;
    }
  }
}
void DCMF(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void DCMS(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void DCML(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}
void DCMR(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}
void loop() {//흰색:90,검정:700//
  /*DCMF();
  delay(0);
  DCML();
  delay(0);
  DCMR();
  delay(0);*/
Serial.print("IR1:");
Serial.println(analogRead(IR1));
Serial.print("IR2:"); 
Serial.println(analogRead(IR2));
Serial.print("IR3:");
Serial.println(analogRead(IR3));
Serial.println("");

  if(analogRead(IR1)<50 && analogRead(IR2)>600 && analogRead(IR3)<50){
    DCMF();
  }
  else if(analogRead(IR1)>600 && analogRead(IR2)<50 && analogRead(IR3)<50){
    DCMR();
  }
  else if(analogRead(IR1)<50 && analogRead(IR2)<50 && analogRead(IR3)>600){
    DCML();
  }
  else if(analogRead(IR1)>600 && analogRead(IR2)>600 && analogRead(IR3)>600){
    DCMS();//교차로에 도착하면 정지
    delay(5000);
    count++;//교차로 감지 횟수
    if(count==1){//첫번째 교차로 이면
      DCMF();  //교차로를 벗어나기 위해 조금 앞으로 이동
      delay(300);
      DCML();  //좌회전
      delay(600);//시간으로 회전 각도 조절
        }
        else if (count==2){//두번째 교차로 이면
          DCMR();//우회전
          analogWrite(ENA,130);
          analogWrite(ENB,130);
          delay(600);
          analogWrite(ENA,120);
          analogWrite(ENB,120);
          }
        else if (count==3){//세번째 교차로 이면
          analogWrite(ENA,120);
          analogWrite(ENB,120);
          DCMR();//우회전
          delay(600);
          analogWrite(ENA,120);
          analogWrite(ENB,120);
  }
  
  

  
  
  /*
  measureDistance();
 digitalWrite(13,HIGH);
 delay(100);
 digitalWrite(13,LOW);
 delay(100);
  
 Serial.println("IR1: ");
 Serial.println(analogRead(IR1));
 Serial.println("IR2: ");
 Serial.println(analogRead(IR2));
 Serial.println("IR3: ");
 Serial.println(analogRead(IR3));
 Serial.println("");

 if(analogRead(IR1)>800 && analogRead(IR2)>800 && analogRead(IR3)>800){
 Serial.println("");
 Serial.println("OK");
 Serial.println("");
 
 } 
 digitalWrite(SM,HIGH);
 delayMicroseconds(1500);
 digitalWrite(SM,LOW);
 delay(20);
 
 //delay(300);
 */
  }
}