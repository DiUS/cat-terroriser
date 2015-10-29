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
int lastState = state;

//photocell
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int photocellPin2 = 12;     // the cell and 10K pulldown are connected to a0
int photocellReading2;     // the analog reading from the sensor divider
int LEDpin = 22;
int LEDpin2 = 23;
boolean ledOn = false;
boolean spinRight = true;

int darkThreshold = 400;

int loopsInState = 0;

void setup()
{
  pinMode( channel_a_enable, OUTPUT );  // Channel A enable
  pinMode( channel_a_input_1, OUTPUT ); // Channel A input 1
  pinMode( channel_a_input_2, OUTPUT ); // Channel A input 2
  
  pinMode( channel_b_enable, OUTPUT );  // Channel B enable
  pinMode( channel_b_input_3, OUTPUT ); // Channel B input 3
  pinMode( channel_b_input_4, OUTPUT ); // Channel B input 4

  pinMode(LEDpin, OUTPUT);
  pinMode(LEDpin2, OUTPUT);
  
  Serial.begin( 9600 );
  Serial.println("Starting up");
  headToDark();
}

void loop()
{
  if(state == looking_for_dark) {
    headToDark();
    toggleLed();
    if(isDark()) {
      loopsInState = 0;
      turnAllLedsOff();
      state = in_dark;
      stopWheels();  
    }
  }
  else if(state == looking_for_light) {
    headToLight();
    if(!isDark()) {
      loopsInState = 0;
      state = looking_for_dark;
      stopWheels();
      delay(1000);
    }
  }
  else if(state == in_dark) {
    delay(3000);
    headToLight();
    delay(3000);
    state = looking_for_light;
  }

  if(lastState == state) {
    loopsInState++;
  } else {
    loopsInState = 0;
  }
  lastState = state;
  
  if (loopsInState > 0 && loopsInState % 10 == 0) {
    spin();
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
  if(spinRight) {
    digitalWrite( channel_a_input_1, HIGH);
    digitalWrite( channel_a_input_2, LOW);
    digitalWrite( channel_b_input_3, HIGH);
    digitalWrite( channel_b_input_4, LOW);  
    spinRight = false; 
  } else {
    digitalWrite( channel_a_input_1, LOW);
    digitalWrite( channel_a_input_2, HIGH);
    digitalWrite( channel_b_input_3, LOW);
    digitalWrite( channel_b_input_4, HIGH);  
    spinRight = true;
  }
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
  int backReading =  analogRead(photocellPin2);
  return photocellReading < darkThreshold  || backReading < darkThreshold ;
}

void toggleLed() {
  if(ledOn) {
    turnLedOff();  
  } else {
    turnLedOn();
  }
}

void turnLedOff() {
  ledOn = false;
  digitalWrite(LEDpin, LOW);  
  digitalWrite(LEDpin2, HIGH); 
}


void turnLedOn() {
  ledOn = true;
  digitalWrite(LEDpin, HIGH);
  digitalWrite(LEDpin2, LOW);
}

void turnAllLedsOff() {
  digitalWrite(LEDpin, LOW);
  digitalWrite(LEDpin2, LOW);
}


void headToLight() {
  int frontReading = analogRead(photocellPin) / 100;
  int backReading =  analogRead(photocellPin2) / 100;
  if(frontReading >= backReading) {
    Serial.println("going forward");
    goForward();
  }
  else {
    Serial.println("going backward");
    goBackward();
  }
}


void headToDark() {
  int frontReading = analogRead(photocellPin) / 100;
  int backReading =  analogRead(photocellPin2) / 100;
  if(frontReading <= backReading) {
    Serial.println("going forward");
    goForward();
  }
  else {
    Serial.println("going backward");
    goBackward();
  }
}


