const int channel_a_enable  = 6;
const int channel_a_input_1 = 4;
const int channel_a_input_2 = 7;
const int channel_b_enable  = 5;
const int channel_b_input_3 = 3;
const int channel_b_input_4 = 2;

const int looking_for_dark = 0;
const int looking_for_light = 1;
const int in_dark = 2;

int state = looking_for_dark;

//photocell
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDpin = 22;
boolean ledOn = false;

void setup()
{
  pinMode( channel_a_enable, OUTPUT );  // Channel A enable
  pinMode( channel_a_input_1, OUTPUT ); // Channel A input 1
  pinMode( channel_a_input_2, OUTPUT ); // Channel A input 2
  
  pinMode( channel_b_enable, OUTPUT );  // Channel B enable
  pinMode( channel_b_input_3, OUTPUT ); // Channel B input 3
  pinMode( channel_b_input_4, OUTPUT ); // Channel B input 4

  pinMode(LEDpin, OUTPUT);
  
  Serial.begin( 9600 );
  Serial.println("Starting up");
  goForward();
}

void loop()
{
  if(state == looking_for_dark) {
    toggleLed();
    if(isDark()) {
      turnLedOff();
      state = in_dark;
      stopWheels();  
    }
  }
  else if(state == looking_for_light && !isDark()) {
    state = looking_for_dark;
    stopWheels();
    spin();
    delay(1000);
    goForward();
  }
  else if(state == in_dark) {
    delay(3000);
    goBackward();
    delay(3000);
    goBackward();
    state = looking_for_light;
  }
  delay(1000);
}

void goForward()
{
    Serial.println("Going forward");
    setSpeed(100);
    digitalWrite( channel_a_input_1, HIGH);
    digitalWrite( channel_a_input_2, LOW);
    digitalWrite( channel_b_input_3, LOW);
    digitalWrite( channel_b_input_4, HIGH);    
}

void goBackward()
{
    Serial.println("Going backward");
    setSpeed(100);
    digitalWrite( channel_a_input_1, LOW);
    digitalWrite( channel_a_input_2, HIGH);
    digitalWrite( channel_b_input_3, HIGH);
    digitalWrite( channel_b_input_4, LOW);    
    
}

void spin() {
  Serial.println("Spinning...");
  setSpeed(150);
  digitalWrite( channel_a_input_1, HIGH);
  digitalWrite( channel_a_input_2, LOW);
  digitalWrite( channel_b_input_3, HIGH);
  digitalWrite( channel_b_input_4, LOW); 
}

void stopIn(int milliseconds) {
  delay(milliseconds);
  stopWheels();
}

void stopWheels()
{
  digitalWrite( 4, LOW );
  digitalWrite( 7, LOW );
  digitalWrite( 6, LOW );
  digitalWrite( 3, LOW );
  digitalWrite( 2, LOW );
  digitalWrite( 5, LOW );
}

void setSpeed(int speed)
{
  analogWrite( channel_a_enable, speed);
  analogWrite( channel_b_enable, speed * 1.1);
}

boolean isDark() {
  photocellReading = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);
  return photocellReading < 400;
}

void toggleLed() {
  if(ledOn) {
    turnLedOff();  
  } else {
    turnLedOn();
  }
}

void turnLedOff() {
  Serial.println("Turning light off");
  ledOn = false;
  digitalWrite(LEDpin, LOW);  
}


void turnLedOn() {
  Serial.println("Turning light on");
  ledOn = true;
  digitalWrite(LEDpin, HIGH);
}


