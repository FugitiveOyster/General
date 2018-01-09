#include<SoftwareSerial.h>

#include <Wire.h>

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *Motor = AFMS.getStepper(200, 1);

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

const int sensorPin = 0;
const int pullPin1 = 8;
const int pullPin2 = 9;
int currentPosition = 0;
int sensorState = 0;
float colorNum = 0.0;
int backlightColor;
int triggerValue;
int sensorThreshold = 250;
int backlash = 15; //steps
int offset;
int ambientLight;

int startNum1 = 0;
int startNum2 = 0;
int startNum3 = 0;
boolean startState = 0;

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//----------------------------------------------
void setup(){
  pinMode(pullPin1, OUTPUT);
  pinMode(pullPin2, OUTPUT);
  digitalWrite(pullPin1, LOW);
  digitalWrite(pullPin2, HIGH);
  
  AFMS.begin();
  Motor->setSpeed(200);
  
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  
  while(!lcd.readButtons()){
    
    lcd.setCursor(0,0);
    lcd.print("Set Dial to Zero");
    lcd.setCursor(0,1);
    lcd.print("Then Push Button");
    
    Serial.println("Send any keys to start setting start combo");
    
    boolean Done = 0;
    
    if(Serial.available()){
      String command = Serial.readString();
      startState = 1;
          
      Serial.print("First Number: ");
      while(Done == 0){
        if(Serial.available()){
          delay(100);
          startNum1 = Serial.parseInt();
          Serial.println(startNum1);
          Done = 1;         
        }
      }
      
      Serial.print("Second Number: ");
      Done = 0;
      while(Done == 0){
        if(Serial.available()){
          delay(100);
          startNum2 = Serial.parseInt();
          Serial.println(startNum2);
          Done = 1;         
        }
      }
 
      Serial.print("Third Number: ");
      Done = 0;
      while(Done == 0){
        delay(100);
        if(Serial.available()){
          startNum3 = Serial.parseInt();
          Serial.println(startNum3);
          Done = 1;         
        }
      }
    }
  }
}
//----------------------------------------------
void pull(){
  digitalWrite(pullPin1, HIGH);
  digitalWrite(pullPin2, LOW);
  lcd.setBacklight(RED);
  delay(1000);
  int sensor = analogRead(sensorPin);
  if(sensor > sensorThreshold){
    triggerValue = sensor;
    sensorState = 1;
  }
  digitalWrite(pullPin1, LOW);
  digitalWrite(pullPin2, HIGH);
  lcd.setBacklight(WHITE);
  delay(500);
}
//----------------------------------------------
float ExtractDecimalPart(float Value) {
  Value = Value / 5;
  int IntegerPart = (int)(Value);
  float DecimalPart = round((Value - IntegerPart)*5); //10000 b/c my float values always have exactly 4 decimal places
  //Serial.print("DecimalPart "); Serial.println(DecimalPart);
  return(DecimalPart);
}
//----------------------------------------------
int changeColor(float Num){
  int NumInt;
  int Color;
  
  NumInt = (int) ExtractDecimalPart(Num) + 2;
  //Serial.print("NumInt"); Serial.print(NumInt); Serial.println();
  
  switch (NumInt) {
    case 1:
      Color = RED;
      break;
    case 2:
      Color = GREEN;
      break;
    case 3:
      Color = YELLOW;
      break;
    case 4:
      Color = BLUE;
      break;
    case 5:
      Color = VIOLET;
      break;
    case 6:
      Color = TEAL;
      break;
    default: 
      Color = WHITE;
  }
 
  //Serial.print("Color "); Serial.println(Color);
  return(Color);
}
//----------------------------------------------
void tryCombo(int N1, int N2, int N3){
  int stepsToMove = currentPosition*5 + 400;
  lcd.clear();
  lcd.print("Trying New Combo");
  //*****Motor->step(stepsToMove+offset, BACKWARD, DOUBLE); //Back to zero + 2 revs
  lcd.clear();
  currentPosition = 0;

  //Serial.print("colorNum"); Serial.println(colorNum);
  if(colorNum != 0){backlightColor = changeColor(colorNum);}
  else if (colorNum == 0){backlightColor = WHITE;}
  colorNum++;
  //Serial.print("backlightColor"); Serial.println(backlightColor); Serial.println();
  lcd.setBacklight(backlightColor);
  
  Serial.print(N1); Serial.print("  ");
  
  ambientLight = analogRead(0);
  lcd.setCursor(0,1);
  lcd.print("Ambient: "); lcd.print(ambientLight);
  lcd.setCursor(0,0);
  //if(currentPosition!=N1){             
  stepsToMove = 200-N1*5;                          // move to first number
  lcd.print(" "); lcd.print(N1); lcd.print("     ");
  Motor->step(stepsToMove, BACKWARD, DOUBLE);
  currentPosition = N1;
  //}
  Serial.print(N2); Serial.print("  ");
  //
  
  //if(currentPosition!=N2){             
  if(N2 > N1){stepsToMove = 200 + ((N2-N1)*5);}    // move to second number
  if(N2 < N1){stepsToMove = 200 + (200-(N1*5)) + (N2*5);}
  lcd.print(N2); lcd.print("     ");
  
  Motor->step(stepsToMove+backlash, FORWARD, DOUBLE);
  currentPosition = N2;
  //}
  Serial.print(N3); Serial.println("  ");
  
  //if(currentPosition!=N3){             // move to third number
  stepsToMove = (N2*5) + (200 - (N3*5));
  lcd.print(N3); lcd.print("     ");
  
  Motor->step(stepsToMove+backlash, BACKWARD, DOUBLE);
  currentPosition = N3;
  //}
  //Serial.println("");
  delay(100);
}
//----------------------------------------------
void loop(){
  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  
  for(num1 = 0; num1 <40; num1++){
    for(num2 = 0; num2 <40; num2++){
      for(num3 = 0; num3 <40; num3++){
        if(startState == 1){
          num1 = startNum1;
          num2 = startNum2;
          num3 = startNum3;
          startState = 0;
        }
        tryCombo(num1, num2, num3);
        pull();
        if(sensorState == 1){break;}
      }
      if(sensorState == 1){break;}
    }
    if(sensorState == 1){break;}
  }
  while(sensorState == 1){
    Serial.println("Done");
    for(int i = 1; i <= 6; i++){
      lcd.setBacklight(i);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" "); lcd.print(num1); lcd.print("     ");
      lcd.print(" "); lcd.print(num2); lcd.print("     ");
      lcd.print(num3);
      lcd.setCursor(0,1);
      lcd.print(" FOUND COMBO!! ");
      lcd.setCursor(0,1);
      delay(500);
      lcd.clear();
      lcd.print(" "); lcd.print(num1); lcd.print("     ");
      lcd.print(" "); lcd.print(num2); lcd.print("     ");
      lcd.print(num3);
      lcd.setCursor(0,1);
      lcd.print("Trigger: "); lcd.print(triggerValue);
      delay(500);
      while(lcd.readButtons()){
        startNum1 = num1;
        startNum2 = num2;
        startNum3 = num3;
        sensorState = 0;
        startState = 1;
        offset = num1*5;
        break;
      }
    }
  }
}
