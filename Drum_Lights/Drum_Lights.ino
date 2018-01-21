int led_pin = 9;
int sensor_pin = A3;
int brightness_target = 64;
const int window_size = 25;
int max_value_array[window_size];
int array_index = 0;

int sensor_gain = 10;            //  Snare
int decay_step = 3;
int sensor_threshhold = 25;

//int sensor_gain = 10;          //  Bass
//int decay_step = 3;
//int sensor_threshhold = 25;
//
//int sensor_gain = 10;          //  Small tom
//int decay_step = 3;
//int sensor_threshhold = 25;
//
//int sensor_gain = 10;          //  Floor Tom
//int decay_step = 3;
//int sensor_threshhold = 25;
//
//int sensor_gain = 10;          //  Hi-hat
//int decay_step = 3;
//int sensor_threshhold = 25;
//
//int sensor_gain = 10;          //  Ride
//int decay_step = 3;
//int sensor_threshhold = 25;

void setup() {
  Serial.begin(250000);
  pinMode(led_pin, OUTPUT);
  pinMode(sensor_pin, INPUT);
  setup_timer2();
}

void loop() {

  //Serial.println(analogRead(sensor_pin));
  
  int sensor_value = analogRead(sensor_pin);
  if(sensor_value < sensor_threshhold) sensor_value = 0;
  max_value_array[array_index] = sensor_value;
  //Serial.println(sensor_value);
  array_index++;
  if(array_index >= window_size) array_index = 0;
  //Serial.print(array_index); Serial.print(", "); Serial.println(max_value_array[array_index]);
  //Serial.print(max_value_array[array_index]); Serial.print(", ");

  int max_value = 0;

  for(int i = 0; i < window_size; i++){
    if(max_value_array[i] > max_value) max_value = max_value_array[i];
  }

  max_value = max_value * sensor_gain;

  if(max_value < brightness_target) max_value = brightness_target - decay_step;
  if(max_value > 245) max_value = 245;
  if(max_value < 2) max_value = 2;
  
  brightness_target = max_value;

  //Serial.println(brightness_target);

  delay(1);

  

}

void setup_timer2() {   // 8-bit timer (0-255)  // Used for LED fading

  TCCR2A = 0;  // clear any previous settings
  TCCR2B = 0;

  TCNT2  = 0;  // clear counter register

  OCR2A = 255; // set TOP value
  OCR2B = brightness_target; // set compare value (must be 1 - 244)

  TCCR2A |= (1 << WGM21) | (1 << WGM20); // set mode to fast PWM

  TCCR2B |= (1 << CS22); // set prescaler 64 (~976.5 hz)

  // CS22   CS21   CS20   Prescaler
  //  0      0      0     Timer OFF
  //  0      0      1     1
  //  0      1      0     8
  //  0      1      1     32
  //  1      0      0     64
  //  1      0      1     128
  //  1      1      0     256
  //  1      1      1     1024


  TIMSK2 |= (1 << OCIE2A); // sets interrupt flag A (ISR(TIMER2_COMPA_vect))
  TIMSK2 |= (1 << OCIE2B); // sets interrupt flag B (ISR(TIMER2_COMPB_vect))
  //TIMSK2 |= (1 << TOIE2); // sets interrupt flag at top (ISR(TIMER2_OVF_vect))

}


//**********************************************************************

ISR(TIMER2_OVF_vect) {

  //digitalWrite(led_pin, HIGH);
  
}


//**********************************************************************

ISR(TIMER2_COMPA_vect) {
  
  if(brightness_target > 2) digitalWrite(led_pin, HIGH);

}

//**********************************************************************

//**********************************************************************

ISR(TIMER2_COMPB_vect) {
  
  digitalWrite(led_pin, LOW);
  if(OCR2B != brightness_target) OCR2B = brightness_target;

}

//**********************************************************************

