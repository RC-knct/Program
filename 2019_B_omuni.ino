#include<PS3BT.h>
#include<SPI.h>

//Pin Name
//F=flont, R=rear
//L=left , R=right
//1=前回転, 2=後回転
#define FL1 2
#define FL2 3
#define FR1 4
#define FR2 5
#define RL1 6
#define RL2 7
#define RR1 8
#define RR2 9
#define motor5_1 10
#define motor5_2 11
//#define motor6_1 24
//#define motor6_2 25
#define air_1 22
#define EM_BTN 48
#define RELAY_OUT A0

#define s 50 //十字ボタンの移動速度
#define motor5 100 //モーター速度調整
#define v 0.7 //移動速度調整
#define r 1.0 //回転速度調整

#define m1_
#define m2_
#define m3_
#define m4_

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

int RX, RY, t; //RX=RightHatX, RY=RightHatY, t=turn
float cos45, m1, m2, m3, m4; //m1,m2,m3,m4=各モーター出力

void setup() {
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  cos45=0.707;

  pinMode(FL1, OUTPUT);
  pinMode(FL2, OUTPUT);
  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(RL1, OUTPUT);
  pinMode(RL2, OUTPUT);
  pinMode(RR1, OUTPUT);
  pinMode(RR2, OUTPUT);
  pinMode(motor5_1, OUTPUT);
  pinMode(motor5_2, OUTPUT);
//  pinMode(motor6_1, OUTPUT);
//  pinMode(motor6_2, OUTPUT);
  pinMode(air_1, OUTPUT);
  pinMode(RELAY_OUT,OUTPUT);
  pinMode(EM_BTN,INPUT_PULLUP);
}

void loop() {
  Usb.Task();
  digitalWrite(RELAY_OUT,!digitalRead(EM_BTN));
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    //digitalWrite(13, HIGH);
    if (PS3.getButtonPress(PS)) {
      digitalWrite(13, LOW);
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }

    //コントローラーの値読み取り
    RX = (PS3.getAnalogHat(LeftHatX));
    RY = (PS3.getAnalogHat(LeftHatY));
    //↓両方のスティックを使う場合↓
    /*RX = (PS3.getAnalogHat(RightHatX));
    RY = (PS3.getAnalogHat(LeftHatY));*/
    //t = -(PS3.getAnalogButton(R2))+(PS3.getAnalogButton(L2)); //回転
    t = -(PS3.getAnalogHat(RightHatX))+127;
    //十字ボタン用
    if     (PS3.getButtonPress(UP))   {RY=RY-s;}
    else if(PS3.getButtonPress(DOWN)) {RY=RY+s;}
    if     (PS3.getButtonPress(RIGHT)){RX=RX+s;}
    else if(PS3.getButtonPress(LEFT)) {RX=RX-s;}

    //中央値を0に調整
    RX =  (RX - 127) * 2;
    RY = -(RY - 127) * 2;

    //各モーターへの出力の計算
    m1=( cos45*v*RY-cos45*v*RX+r*t);
    m2=(-cos45*v*RY-cos45*v*RX+r*t);
    m3=(-cos45*v*RY+cos45*v*RX+r*t);
    m4=( cos45*v*RY+cos45*v*RX+r*t);

    //各モーターへの出力が255を超えないように調整
    m1 = (m1 < -255 ? -255 : (m1 > 255 ? 255 : m1));
    m2 = (m2 < -255 ? -255 : (m2 > 255 ? 255 : m2));
    m3 = (m3 < -255 ? -255 : (m3 > 255 ? 255 : m3));
    m4 = (m4 < -255 ? -255 : (m4 > 255 ? 255 : m4));

    //ブレーキ(動かない)
    /*if(m1<30&&m2<30&&m3<30&&m4<30){
      if(m1>20||m2>20||m3>20||m4>20){
        analogWrite(FL1,100);
        analogWrite(FL2,100);
        analogWrite(FR1,100);
        analogWrite(FR2,100);
        analogWrite(RL1,100);
        analogWrite(RL2,100);
        analogWrite(RR1,100);
        analogWrite(RR2,100);
      }
    }*/
    
    if(m1>30) {
      analogWrite(FR1,m1);
    }
    else if(m1<-30){
      analogWrite(FR2,-m1);
    }
    else{
      analogWrite(FR1,0);
      analogWrite(FR2,0);
    }
    if(m2>30){
      analogWrite(FL1,m2);
    }
    else if(m2<-30){
      analogWrite(FL2,-m2);
    }
    else{
      analogWrite(FL1,0);
      analogWrite(FL2,0);
    }
    if(m3>30){
      analogWrite(RL1,m3);
    }
    else if(m3<-30){
      analogWrite(RL2,-m3);
    }
    else{
      analogWrite(RL1,0);
      analogWrite(RL2,0);
    }
    if(m4>30){
      analogWrite(RR1,m4);
    }
    else if(m4<-30){
      analogWrite(RR2,-m4);
    }
    else{
      analogWrite(RR1,0);
      analogWrite(RR2,0);
    }

    //motor5 PWM制御
    if(PS3.getButtonPress(CIRCLE)){analogWrite(motor5_1,motor5);}
    else{analogWrite(10,0);}
    if(PS3.getButtonPress(SQUARE)){analogWrite(motor5_2,motor5);}
    else{analogWrite(11,0);}

    //motor5,6 HIGH LOW
    /*if(PS3.getButtonPress(CIRCLE))  {digitalWrite(motor5_1,HIGH);}
    else{digitalWrite(motor5_1,LOW);}
    if(PS3.getButtonPress(SQUARE))  {digitalWrite(motor5_2,HIGH);}
    else{digitalWrite(motor5_2,LOW);}
    if(PS3.getButtonPress(TRIANGLE)){digitalWrite(motor6_1,HIGH);}
    else{digitalWrite(motor6_1,LOW);}
    if(PS3.getButtonPress(CROSS))   {digitalWrite(motor6_2,HIGH);}
    else{digitalWrite(motor6_2,LOW);}*/

    //air
    if(PS3.getButtonPress(TRIANGLE)){
      digitalWrite(air_1,HIGH);
    }
    if(PS3.getButtonPress(CROSS)){
      digitalWrite(air_1,LOW);
    }
    
    Serial.print("X=");
    Serial.print(RX);
    Serial.print(" Y=");
    Serial.print(RY);
    Serial.print(" t=");
    Serial.print(t);
    Serial.print(" m1=");
    Serial.print(m1);
    Serial.print(" m2=");
    Serial.print(m2);
    Serial.print(" m3=");
    Serial.print(m3);
    Serial.print(" m4=");
    Serial.println(m4);
  }
}
