#include <Arduino.h>
#line 1 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
#line 1 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
// Configure your parameters below

int speed = 10; // less number -> higher speed
int numberOfRevolutions1=1;
int timeDelayMilliseconds1=1000;
int numberOfRevolutions2=1;
int timeDelayMilliseconds2=1000;
int numberOfRevolutions3=1;
int timeDelayMilliseconds3=1000;
int numberOfReturningRevolutions=3;

// Delay in seconds after pressing button
int relay1DelaySeconds=0;
int relay2DelaySeconds=0;
int relay3DelaySeconds=0;
int relay4DelaySeconds=0;

// Duration power on relay after delaying
int relay1DurationSeconds=0;
int relay2DurationSeconds=0;
int relay3DurationSeconds=0;
int relay4DurationSeconds=0;

const int ledPin=13;
const int pulPin=30;
const int dirPin=31;
const int buttonPin=53;

int timer1_counter;
int secondsAfterPressingButton=0;

#line 32 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeOneStepLeft();
#line 43 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeOneStepRight();
#line 54 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeOneRevolutionLeft();
#line 63 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeOneRevolutionRight();
#line 72 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeRevolutionsLeft(long numberOfRevolutions);
#line 81 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeRevolutionsRight(long numberOfRevolutions);
#line 90 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void setup();
#line 119 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void loop();
#line 32 "C:\\Users\\virand\\AppData\\Local\\Temp\\arduino_modified_sketch_742102\\Blink.ino"
void makeOneStepLeft()
{
  digitalWrite(dirPin, LOW);
  delayMicroseconds(20);
  digitalWrite(pulPin, LOW);
  delayMicroseconds(40);
  digitalWrite(pulPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(dirPin, HIGH);  
}

void makeOneStepRight()
{
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(40);
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(dirPin, HIGH); 
}

void makeOneRevolutionLeft()
{
  for (int i=0; i<200;i++)
  {
    makeOneStepLeft();
    delay(speed);
  }
}

void makeOneRevolutionRight()
{
  for (int i=0; i<200;i++)
  {
    makeOneStepRight();
    delay(speed);
  }
}

void makeRevolutionsLeft(long numberOfRevolutions)
{
  for(int i=0; i<numberOfRevolutions;i++)
  {
    makeOneRevolutionLeft();
  }
}


void makeRevolutionsRight(long numberOfRevolutions)
{
  for(int i=0; i<numberOfRevolutions;i++)
  {
    makeOneRevolutionRight();
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pulPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(pulPin, HIGH);
  digitalWrite(dirPin, HIGH);

  // Initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;;   // preload timer
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

// the loop function runs over and over again forever
void loop() {

  // If button was pressed
    if(digitalRead(buttonPin) == HIGH)
    {
      makeRevolutionsLeft(numberOfRevolutions1);
      delay(timeDelayMilliseconds1);
      makeRevolutionsLeft(numberOfRevolutions2);
      delay(timeDelayMilliseconds2);
      makeRevolutionsLeft(numberOfRevolutions3);
      delay(timeDelayMilliseconds3);
      makeRevolutionsRight(numberOfReturningRevolutions);
    }
}
