/*************************************************** 
 * This is an example for our Adafruit 24-channel PWM/LED driver
 * 
 * Pick one up today in the adafruit shop!
 * ------> http://www.adafruit.com/products/1429
 * 
 * These drivers uses SPI to communicate, 3 pins are required to  
 * interface: Data, Clock and Latch. The boards are chainable
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 2

//without spacebar and CR


//with space bar and CR
#define num_keys 53




#define data   51
#define clock   52
#define latch   25
#define oe  -1  // set to -1 to not use the enable pin (its optional)

#define LED_dim_value   250   // make this greater than 0. If you make it zero, the first pass through the main loop won't set the TLC properly. But you can change that if you need.
#define LED_bright_value   4095

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

//this is the input array, the first column needs ot be alligned with teh keys
//[charIndex, input, ledNumber]
int pin_config[][3] = {       // n is
  { 28   , 28 ,  12       } ,   //0
  { 29   , 30 ,  13       } ,   //1
  { 26   , 32  ,  14       } ,  //2
  { 31   , 34  ,  15       } ,  //3
  { 27   , 36  ,  16       } ,  //4
  { 23   , 38  ,  17       } ,  //5
  { 44   , 40  ,  18       } ,  //6   //missing
  { 32   , 42  ,  19       } ,  //7
  { 25   , A0  ,  20       } ,  //8
  { 22   , A1  ,  21       } ,  //9
  { 24   , A2  ,  22       } ,  //10
  { 30   , A3  ,  23       } ,  //11
  { 37   , A4  ,   0       } ,  //12
  { 44   , A5  ,   1       } ,  //13    //missing
  { 35   , A6  ,  2        } ,  //14
  { 40   , A7  ,  3        } ,  //15
  { 39   , A8  ,  4        } ,  //16
  { 34   , A9  ,  5        } ,  //17
  { 44   , A10 ,   6       } ,  //18     //missing
  { 33   , A11 ,  7        } ,  //19
  { 36   , A12 ,  8        } ,  //20
  { 38   , A13 ,  9        } ,  //21
  { 41   , A15 ,  11       } ,  //22
  { 16  , 12  ,   36      } ,  //23
  { 18  , 13  ,  37       } ,  //24
  { 17  , 11  ,  38       } ,  //25
  { 13  , 10  ,  39       } ,  //26
  { 20   , 9   ,  40       } ,  //27  
  { 12  , 8   ,  41       } ,  //28
  { 15  , 7   ,  42       } ,  //29
  { 19  , 6   ,  43       } ,  //30
  { 14  , 23  ,  44       } ,  //31
  { 11  , 22  ,  45       } ,  //32
  { 21   , 24  ,  46       } ,  //33
  { 42   , 26  ,  47       } ,  //34
  { 7   , 5   ,  24       } ,  //35
  { 0   , 4   ,  25       } ,  //36
  { 1   , 3   ,  26       } ,  //37  //missing - questionable 1
  { 3   , 2   ,  27       } ,  //38
  { 10  , 14  ,  28       } ,  //39
  { 4   , 15  ,  29       } ,  //40
  { 2   , 16  ,  30       } ,  //41     
  { 8   , 17  ,  31       } ,  //42
  { 9   , 18  ,  32       } ,  //43
  { 6   , 19  ,  33       } ,  //44
  { 5   , 20  ,  34       } ,  //45
  { 43   , 27  ,  48       } , //space1
  { 43   , 29  ,  48       } , //space2
  { 43   , 31  ,  48       } , //space3
  { 43   , 33  ,  48       } , //space4
  { 43   , 35  ,  48       } , //space5
  { 43   , 37  ,  48       } , //Space 6
  { 44  ,  39  ,  48       }  //CR
  ,

};


//this is a lower case, upper case array
char char_config[][3] = {
{10,10, 36},    //0	//this is the backspace char
{'2','"', 37},  //1
{'3','#', 41},  //2
{'4','$', 38},  //3
{'5','%', 40},  //4
{'6','-', 45},  //5
{'7','&', 44},  //6
{'8',39, 35},   //7
{'9','(', 42},  //8
{'0',')', 43},  //9
{'_','*', 39},  //10
{'q','Q', 32},  //11
{'w','W', 28},  //12
{'e','E', 26},  //13
{'r','R', 31},  //14
{'t','T', 29},  //15
{'y','Y', 23},  //16
{'u','U', 26},  //17
{'i','I', 24},  //18
{'o','O', 30},  //19
{'p','P', 27},  //20
{60,61, 33},    //21     //special chars for 1/2 and 1/4
{'a','A', 9},   //22
{'s','S', 5},   //23 
{'d','D', 10},  //24
{'f','F', 8},   //25
{'g','G', 2},   //26
{'h','H', 4},   //27
{'j','J', 0},   //28
{'k','K', 1},   //29
{'l','L', 11},  //30
{';',':', 3},   //31
{62,'@', 7},    //32      //special char for cent char
{'z','Z', 19},  //33
{'x','X', 17},  //34
{'c','C', 14},  //35
{'v','V', 20},  //36
{'b','B', 12},  //37
{'n','N', 21},  //38
{'m','M', 16},  //39
{',','?', 15},  //40
{'.','.', 22},	//41        //special char here for bullet
{'/',92, 34},	//42        //special char here for 3/4
{' ',' ', 48},	//43        //space char
{15,15, 48}	//44        //carrige return char
};

//There are 53 keys
int button_state[num_keys];
//There are only 46 lights, the 6 space keys and the CR do not have lights
int light_state[num_keys];

int button_low_thresh;
int button_high_thresh;
int button_state_low_limit;
int button_state_high_limit;

boolean keySend;

int shift1Key;
int shift2Key;
int shiftLockKey;

boolean shift1On;
boolean shift2On;
boolean shiftLockOn;

boolean debug;

void setup() {
  Serial.begin(115200);

  Serial.println("TLC5974 test");
  tlc.begin();
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }

  for(int n = 0; n < num_keys; n++){
    pinMode(pin_config[n][1], INPUT);
  }

  for(int n = 0; n < num_keys; n++){
    button_state[n] = 0;
  }
  for(int n = 0; n < num_keys; n++){
    light_state[n] = 0;  // Start off at zero. Will go up to LED_dim_value momentarily
  }



  button_low_thresh=2;
  button_high_thresh=18;
  button_state_low_limit=0;
  button_state_high_limit=19;
  keySend = false;
  shift1On = false;
  shift2On = false;
  shiftLockOn = false;
  
   shift1Key = 13;
   shift2Key = 18;
   shiftLockKey = 6;
  
  
  debug = true;


  /*
  button_low_thresh=20;
   button_high_thresh=40;
   button_state_low_limit=0;
   button_state_high_limit=50;
   */

}

void loop() {

  //****  Read all buttons and enter into button_state array
  for(int n = 0; n < num_keys; n++){      
    if(!digitalRead(pin_config[n][1])){

      if (false ==true){
        Serial.println("\n----pin check-----");
        Serial.println("This is the first loop check.  This is printed when ever a key is pressed");
        Serial.print("N is ");
        Serial.println(n);
        Serial.print("Pin Config is: ");
        Serial.println(pin_config[n][1]);
        Serial.println("----pin check-----");
      }
      //      if (n == 19){
      //
      //      }
      button_state[n]++;
    }
    else{
      button_state[n]--;
    }
    if(button_state[n] < button_state_low_limit) button_state[n] = button_state_low_limit;
    if(button_state[n] > button_state_high_limit) button_state[n] = button_state_high_limit;
    //if(button_state[n] < 0) button_state[n] = 0;
    //if(button_state[n] > 10) button_state[n] = 10;
  }

  //****  write button state to PWM buffer
  for(int n = 0; n < num_keys; n++){        
    
    /*For the positive transition
    
    Here we want to
    *Check if the pin is high
    
    */
    if(button_state[n] > button_high_thresh && light_state[n] != LED_bright_value){

      //   tlc.setPWM(n, LED_bright_value);

      //this is now the write to the led and send the character out
      //Serial.println("\n------------>Setting light high");
      keySend = true;
      
      //if the shifylock key is pressed, set the capslock to high
      if (n == shiftLockKey){
        shiftLockOn = !shiftLockOn;
        }
      
      
      if  (n == shift1Key ){
       shift1On = true;
       }
       
        if  (n == shift1Key ){
       shift2On = true;
       }
       
      
      
      setPWMvlaues(n, LED_bright_value);

      light_state[n]=LED_bright_value;

      if (false ==true){
        Serial.print("\nN is");
        Serial.println(n);
        
        
//        Serial.print("Button State: ");
//        Serial.println(button_state[n]);
//        Serial.print("button_high_thresh: ");
//        Serial.println(button_high_thresh);
//        Serial.print("Light State: ");
//        Serial.println(light_state[n]);
//        Serial.print("LED_bright_value");
//        Serial.println(LED_bright_value);
//
//
//        Serial.print("\nSet high ");
//        Serial.println(n);
      }
      
      
      //      Serial.print(", button_state is ");
      //      Serial.println(button_state[n]);
    }
    
    /*
    For the negative transition
    
    Here we want to 
    -Turn off the light
    -Turn off the shift or shift lock
    
    */
    else if(button_state[n] <=button_low_thresh && light_state[n] != LED_dim_value)
    {

      //   tlc.setPWM(n, LED_dim_value);

      //this is now the write to the led
    //  Serial.println("\n-->Setting light low");
      

      
      
      if  (n == shift1Key){
       shift1On = false;
       }
       
      if  (n == shift2Key){
       shift2On = false;
       }
      
      setPWMvlaues(n, LED_dim_value);


      //            Serial.print("N is");
      //      Serial.println(n);
      //      Serial.print("Button State: ");
      //      Serial.println(button_state[n]);
      //      Serial.print("button_high_thresh: ");
      //      Serial.println(button_high_thresh);
      //      Serial.print("Light State: ");
      //      Serial.println(light_state[n]);
      //      Serial.print("LED_bright_value");
      //      Serial.println(LED_bright_value);

      light_state[n]=LED_dim_value;
      //      Serial.print("Set low ");
      //      Serial.print(n);

      
      //      Serial.print(", button_state is ");
      //      Serial.println(button_state[n]);
    }

  }

}


/*
AN 7/24/15
 
 This is a hack solution.  I wiried the lightoutputs in to the wrong place, 
 so instead of rewiring I am swapping the light output values.
 
 They are all off by 12.  I also am using the same function for both the high and low checks
 
 */
void setPWMvlaues(int n, int value){
  




  //  Serial.print("\nwriting to light: ");
  //  Serial.print(n);  
  //  Serial.print(" and value: ");
  //  Serial.println(value); 

  if (keySend == true){
    if (true ==true){
      Serial.print("\n-----serialout-----");
      Serial.print("\nThis is the Serial out section. n: ");
      Serial.print(n);
      Serial.println(" and char:");
    
    }
    
    //do caps check
    if (shiftLockOn == true || shift1On == true || shift2On == true){
      Serial.print(char_config[pin_config[n][0]][0]);
    }
    else {
       Serial.print(char_config[pin_config[n][0]][1]);
    }
   
 
    keySend = false;

  }
  if (n < 47){
    if (false == true){
      Serial.print("\n-----write to led-----");
      Serial.print("\nSetting led ");
      Serial.print(n);
      Serial.print(" to ");
      Serial.println(value);
      Serial.println("-----write to led-----");
    }
    tlc.setPWM(pin_config[n][2], value); 
    
    //set shiftlock light
    if (shiftLockOn == true)
    
    
    
    tlc.write();
    
  }
}


////for receiving a charcter, get the char, look up the position against teh char aray, and then set that light high
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    
//    char inChar = (char)Serial.read(); 
//    
//    //look up char against the char array andnthen send n
//    
//    for (int i = 0; i < 44; i++){
//     if (inChar == char_config[i][0] || inChar == char_config[i][1]){
//       
//        //clear all lights
//        dimAllKeys();
//       
//        tlc.setPWM(i, LED_bright_value); 
//        tlc.write();
//       
//       }
//     }
//  }
//}

void dimAllKeys(){
       for (int i = 0; i < 48; i++){
        tlc.setPWM(i, LED_dim_value);
      } 
}


/*  
 
 
 tlc.setPWM(0, 4095);
 tlc.write();
 delay(500);
 tlc.setPWM(0, 3000);
 tlc.write();
 delay(500);
 tlc.setPWM(0, 2000);
 tlc.write();
 delay(500);  
 */
/*
   tlc.setPWM(0, 0);
 tlc.write();
 delay(1000);
 
 for(int n = 0; n < 409; n++){
 digitalWrite(7, digitalRead(7)^1);
 
 for(int i = 0; i < 24; i++){
 tlc.setPWM(i, n*10);
 }
 tlc.write();
 }
 */

/*
  for(int i = 0; i < 4096; i++){
 tlc.setPWM(0, 4096);
 tlc.write();
 delay(1000);
 }
 */
//}








