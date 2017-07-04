#include <LiquidCrystal.h>

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

// Several custom characters for LCD
byte cycleIcon[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100
};

byte relayOnIcon[8] = {
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00000,
  0b10000,
  0b10000
};

byte relayOffIcon[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000
};

// Configure your parameters below

int speed = 10; // less number -> higher speed
int numberOfRevolutions1=3;
long timeDelayMilliseconds1=5000;
int numberOfRevolutions2=3;
long timeDelayMilliseconds2=1000;
int numberOfRevolutions3=3;
long timeDelayMilliseconds3=1000;
int numberOfReturningRevolutions=3;
int softCoeff=5; // higher number -> harder

// Delay in half of seconds after pressing button
int relay1DelayHalfSeconds=0;
int relay2DelayHalfSeconds=20;
int relay3DelayHalfSeconds=40;
int relay4DelayHalfSeconds=60;

// Duration power on relay after delaying in half of seconds 
int relay1DurationHalfSeconds=10;
int relay2DurationHalfSeconds=10;
int relay3DurationHalfSeconds=10;
int relay4DurationHalfSeconds=10;

// This variables store states of relays
boolean relay1State=false;
boolean relay2State=false;
boolean relay3State=false;
boolean relay4State=false;

// Pins definition
const int ledPin=13;
const int pulPin=12;
const int dirPin=11;
const int buttonPin=10;
const int relay1Pin=14;
const int relay2Pin=15;
const int relay3Pin=16;
const int relay4Pin=17;

unsigned long halfSecondsAfterStart=0;
unsigned long halfSecondsAfterButtonPress=-1;
boolean buttonWasPressed=false;
int timer1_counter;
int cycles=0;
boolean updatingDisplay=true;

//int halfSecondsAfterPressingButton=0;




void relayManager()
{
  if( (halfSecondsAfterButtonPress>=relay1DelayHalfSeconds) && (halfSecondsAfterButtonPress<=relay1DelayHalfSeconds+relay1DurationHalfSeconds) ){
      digitalWrite(relay1Pin, HIGH);
      relay1State=true;
  }
  else
  {
    digitalWrite(relay1Pin, LOW);
    relay1State=false;
  }

  if( (halfSecondsAfterButtonPress>=relay2DelayHalfSeconds) && (halfSecondsAfterButtonPress<=relay2DelayHalfSeconds+relay2DurationHalfSeconds) ){
      digitalWrite(relay2Pin, HIGH);
      relay2State=true;
  }
  else
  {
    digitalWrite(relay2Pin, LOW);
    relay2State=false;
  }

  if( (halfSecondsAfterButtonPress>=relay3DelayHalfSeconds) && (halfSecondsAfterButtonPress<=relay3DelayHalfSeconds+relay3DurationHalfSeconds) ){
    digitalWrite(relay3Pin, HIGH);
    relay3State=true;
  }
  else
  {
    digitalWrite(relay3Pin, LOW);
    relay3State=false;
  }

  if( (halfSecondsAfterButtonPress>=relay4DelayHalfSeconds) && (halfSecondsAfterButtonPress<=relay4DelayHalfSeconds+relay4DurationHalfSeconds) ){
    digitalWrite(relay4Pin, HIGH);
    relay4State=true;
  }
  else
  {
    digitalWrite(relay4Pin, LOW);
    relay4State=false;
  }
    
}

void makeOneStepLeft()
{
  noInterrupts();
  digitalWrite(dirPin, LOW);
  delayMicroseconds(20);
  digitalWrite(pulPin, LOW);
  delayMicroseconds(40);
  digitalWrite(pulPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(dirPin, HIGH);
  interrupts(); 
}

void makeOneStepRight()
{
    noInterrupts();
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(40);
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(dirPin, HIGH); 
    interrupts(); 
}

void makeOneRevolutionLeft()
{
  for (int i=0; i<200;i++)
  {
    makeOneStepLeft();
    delay(speed);
  }
}

void makeOneAcceleratingRevolutionLeft()
{
  for (int i=0; i<200;i++)
  {
    makeOneStepLeft();
    delay(speed+(200-i)/softCoeff);
  }
}

void makeOneDeceleratingRevolutionLeft()
{
  for (int i=0; i<200;i++)
  {
    makeOneStepLeft();
    delay(speed+i/softCoeff);
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

void makeOneAcceleratingRevolutionRight()
{
  for (int i=0; i<200;i++)
  {
    makeOneStepRight();
    delay(speed+(200-i)/softCoeff);
  }
}

void makeOneDeceleratingRevolutionRight()
{
    for (int i=0; i<200;i++)
  {
    makeOneStepRight();
    delay(speed+i/softCoeff);
  }
}

void makeRevolutionsLeft(long numberOfRevolutions)
{
  updatingDisplay=false;
  for(int i=0; i<numberOfRevolutions;i++)
  {
    makeOneRevolutionLeft();
  }
  updatingDisplay=true;
}

void makeSoftRevolutionsLeft(long numberOfRevolutions, int steps)
{
  updatingDisplay=false;
  for(int i=0; i<numberOfRevolutions;i++)
  {
    if (i==0)
    {
      makeOneAcceleratingRevolutionLeft();
    }
    else if (i==(numberOfRevolutions-1))
    {
      makeOneDeceleratingRevolutionLeft();
    }
    else if(i==(numberOfRevolutions-2))
    {
      makeOneRevolutionLeft();
      for(int i=0; i<steps; i++){
        makeOneStepLeft();
        delay(speed);
      }
      
    }
    else
    {
      makeOneRevolutionLeft();
    }
    
  }
  updatingDisplay=true;
}




void makeRevolutionsRight(long numberOfRevolutions)
{
  updatingDisplay=false;
  for(int i=0; i<numberOfRevolutions;i++)
  {
    makeOneRevolutionRight();
  }
  updatingDisplay=true;
}

void makeSoftRevolutionsRight(long numberOfRevolutions, int steps)
{
  updatingDisplay=false;
  for(int i=0; i<numberOfRevolutions;i++)
  {
    if (i==0)
    {
      makeOneAcceleratingRevolutionRight();
    }
    else if (i==(numberOfRevolutions-1))
    {
      makeOneDeceleratingRevolutionRight();
    }
    else if (i==(numberOfRevolutions-2))
    {
      makeOneRevolutionRight();
      for(int i=0; i<steps; i++)
      {
        makeOneStepRight();
        delay(speed);
      }
    }
    else
    {
      makeOneRevolutionRight();
    }
    
  }
  updatingDisplay=true;
}

void updateDisplay(long cycles, boolean relay1, boolean relay2, boolean relay3, boolean relay4)
{
  
  lcd.setCursor(0, 1);
  lcd.print(cycles);

  
  lcd.setCursor(5,1);
  if(relay1)
    lcd.print("/_");
  else
    lcd.print("__");


  lcd.setCursor(8,1);
  if(relay2)
    lcd.print("/_");
  else
    lcd.print("__");


  lcd.setCursor(11,1);
  if(relay3)
    lcd.print("/_");
  else
    lcd.print("__");


  lcd.setCursor(14,1);
  if(relay4)
    lcd.print("/_");
  else
    lcd.print("__");
}

// the setup function runs once when you press reset or power the board
void setup() {
  lcd.begin(16, 2);
  lcd.print("Powered by");
  lcd.setCursor(0, 1);
  lcd.print("VIRAND");

  lcd.createChar(0, cycleIcon);
  lcd.createChar(1, relayOnIcon);
  lcd.createChar(2, relayOffIcon);
  delay(600);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.print("    R1 R2 R3 R4");


  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pulPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay4Pin, OUTPUT);

  digitalWrite(pulPin, HIGH);
  digitalWrite(dirPin, HIGH); 

  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  digitalWrite(relay3Pin, LOW);
  digitalWrite(relay4Pin, LOW);

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
  TCNT1 = timer1_counter;   // preload timer
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  halfSecondsAfterStart++;
  if(buttonWasPressed==true)
  {
    halfSecondsAfterButtonPress++;
  }
  if(updatingDisplay==true){
    updateDisplay(cycles,relay1State,relay2State,relay3State,relay4State);
  }
  relayManager();
}

// the loop function runs over and over again forever
void loop() {
  // If button was pressed
    if(digitalRead(buttonPin) == HIGH)
    {
      buttonWasPressed=true;
      makeSoftRevolutionsLeft(numberOfRevolutions1, 100);
      delay(timeDelayMilliseconds1);
      makeSoftRevolutionsLeft(numberOfRevolutions2, 100);
      delay(timeDelayMilliseconds2);
      makeSoftRevolutionsLeft(numberOfRevolutions3, 100);
      delay(timeDelayMilliseconds3);
      makeSoftRevolutionsRight(numberOfReturningRevolutions, 100);
      cycles++;
      buttonWasPressed=false;
      halfSecondsAfterButtonPress=-1;
    }
}
