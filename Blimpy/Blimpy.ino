#include <SoftwareSerial.h>

int aileronSignal;
int LEDpin1 = 5;
int LEDpin2 = 6;
int aileronPin = 7;
int throttlePin = 8;
int LEDpin3 = 9;
int leftThrottlePin = 10;
int rightThrottlePin = 11;
int leftSignal;
int leftBoundary = 1194;
int rightSignal;
int rightBoundary = 1841;

int throttleSignal;
int throttleTop = 1912;
int throttleBottom = 1107;
int LEDoffTime = 500;
int LEDcounter = 255;
int LEDbrightness;

int lastAileronSignal;
int lastThrottleSignal;

void setup()
{
  pinMode(LEDpin1, OUTPUT);
  pinMode(LEDpin2, OUTPUT);
  pinMode(LEDpin3, OUTPUT);
  pinMode(aileronPin, INPUT);
  pinMode(throttlePin, INPUT);
  pinMode(leftThrottlePin, OUTPUT);
  pinMode(rightThrottlePin, OUTPUT);
  Serial.begin(9600);
  aileronSignal = pulseIn(aileronPin, HIGH);
  throttleSignal = pulseIn(throttlePin, HIGH);
  lastAileronSignal = aileronSignal;
  lastThrottleSignal = throttleSignal;
}

void loop()
{
  
  
  aileronSignal = pulseIn(aileronPin, HIGH);
  throttleSignal = pulseIn(throttlePin, HIGH);
  
  if(aileronSignal < 1000){aileronSignal = lastAileronSignal;}
  if(throttleSignal < 1000){throttleSignal = lastAileronSignal;}

  lastAileronSignal = aileronSignal;
  lastThrottleSignal = throttleSignal;  
   
  Serial.print("Ailerons: "); Serial.println(aileronSignal);
  Serial.print("Throttle: "); Serial.println(throttleSignal);
  
  //setBoundaries();
  
  Serial.print("Left Boundary: "); Serial.println(leftBoundary);
  Serial.print("Right Boundary: "); Serial.println(rightBoundary);
  Serial.print("Top Boundary: "); Serial.println(throttleTop);
  Serial.print("Bottom Boundary: "); Serial.println(throttleBottom);
  
  calculateSignals();
  
  LEDcounter = LEDcounter - 4;
  LEDbrightness = LEDcounter;
  if(LEDbrightness < 0){LEDbrightness = 0;}
  if(LEDcounter < (-1*LEDoffTime)){LEDcounter = 255;}
  
  analogWrite(leftThrottlePin, leftSignal);
  analogWrite(rightThrottlePin, rightSignal);
  analogWrite(LEDpin1, 0);
  analogWrite(LEDpin2, 0);
  analogWrite(LEDpin3, 0);
  
  //analogWrite(rightThrottlePin, 30);
  //analogWrite(leftThrottlePin, 0);
  
  //Serial.print("Ailerons: "); Serial.println(aileronSignal);
  //Serial.print("Throttle: "); Serial.println(throttleSignal);
  Serial.print("Left: "); Serial.println(leftSignal);
  Serial.print("Right: "); Serial.println(rightSignal);
  Serial.println();
  //delay(100);
}


void setBoundaries(){
  if(aileronSignal < leftBoundary && aileronSignal > 1000){
    leftBoundary = aileronSignal;
  }
  if(aileronSignal > rightBoundary && aileronSignal < 2000){
    rightBoundary = aileronSignal;
  }
  if(throttleSignal < throttleBottom && aileronSignal > 1000){
    throttleBottom = throttleSignal;
  }
  if(throttleSignal > throttleTop && throttleSignal < 2000){
    throttleTop = throttleSignal;
  }
}

void calculateSignals(){
  if(aileronSignal < 2000 && aileronSignal > 1000 && throttleSignal < 2000 && throttleSignal > 1000) {
    
    aileronSignal = map(aileronSignal, leftBoundary, rightBoundary, -64, 64);
    throttleSignal = map(throttleSignal, throttleBottom, throttleTop, 0, 255);
    
    if(aileronSignal > -20 && aileronSignal < 20) {
      aileronSignal = 0;
    }
    if(throttleSignal < 15) {
      throttleSignal = 0;
    }
    
    leftSignal = throttleSignal + aileronSignal;
    rightSignal = throttleSignal - aileronSignal;
    
    if(leftSignal < 0) {
      rightSignal = rightSignal - leftSignal;
    }
    if(rightSignal < 0) {
      leftSignal = leftSignal - rightSignal;
    }
    if(leftSignal > 255) {
      rightSignal = rightSignal - (leftSignal - 255);
    }
    if(rightSignal > 255) {
      leftSignal = leftSignal - (rightSignal - 255);
    }
    
    leftSignal = constrain(leftSignal, 0, 255);
    rightSignal = constrain(rightSignal, 0, 255);
    
    leftSignal = map(leftSignal, 0, 255, 0, 30);
    rightSignal = map(rightSignal, 0, 255, 0, 30);
    
    if(leftSignal < 5){leftSignal = 0;}
    if(rightSignal < 5){rightSignal = 0;}
  }
  else {
    leftSignal = 0;
    rightSignal = 0;
    
  }
}


  

