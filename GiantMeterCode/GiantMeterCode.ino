int output_min=40;
int output_max=140;

int input_min=500;
int input_max= 825;

byte input_pin = A0;
byte output_pin = 9;
byte config_button_pin = 3;

#define smoothing 30
//int smoothing=30;
int buffer[smoothing];
int bufpos = 0;
int autoconf_seconds = 20;
void setup() {
 pinMode(input_pin, INPUT);
 pinMode(output_pin, OUTPUT);
 digitalWrite(output_pin,0);
 
 Serial.begin(9600);
}


void loop() {
 //  analogWrite(output_pin, 90);
  while(1){
    SweepTest();
  }
 // AutoConfigure();
  while(1) {
    RunMeter();
  }
}

void AutoConfigure() {
  unsigned long start_time = millis();
  int max_value = 0;
  int min_value = 1023;
  while (millis()-start_time < autoconf_seconds*1000)
  {
    int value = analogRead(input_pin);
    if(value > max_value)
      max_value = value;
    if(value < min_value)
      min_value = value;
  }
  input_min = min_value;
  //input_max = max_value;
}

void RunMeter() {
  int current_value = analogRead(input_pin);
  int total = 0;
  bufpos = (bufpos+1)%smoothing;
  buffer[bufpos]= current_value;
  for(int i=0; i<smoothing; i++) {
    total += buffer[i];
  }
  int smoothed_value = total/smoothing;
  analogWrite(output_pin, map(smoothed_value, input_min, input_max, output_min, output_max));
  Serial.print(input_min);
  Serial.print(", ");
  Serial.print(current_value);
  Serial.print(", ");
  Serial.print(smoothed_value);
  Serial.print(", ");
  Serial.println(input_max);
}

void SweepTest() {
    int output_current=0;
    for(int pwm_current=output_min; output_current<output_max; output_current++)
    {
     analogWrite(9,output_current);
     delay(50); 
    }
    for(int output_current=output_max; output_current>output_min; output_current--)
    {
     analogWrite(9,output_current);
     delay(50); 
    }
}
