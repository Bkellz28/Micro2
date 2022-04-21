#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <TimeLib.h>

#define ENABLE  5
#define DIR_CC  3
#define DIR_CCW 4

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //setup pins for LCD
const byte interruptPin = 2;
volatile byte state = LOW;
int curDIR, fanSpeed, sec; //sec variable watches for when clock hits 0 & 30 seconds
unsigned long lastDebounceTime = 0, debounceDelay = 50; // pretty sure we can delete these (and should if don't need)
 
void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), btnPress, CHANGE); //enable interrupts for button
  
  //set fan control pins to output pins
  pinMode(ENABLE,OUTPUT);
  pinMode(DIR_CC,OUTPUT);
  pinMode(DIR_CCW,OUTPUT);
  Serial.begin(9600);
  
  // initialize the lcd display size
  lcd.begin(16, 2);
}

void loop() {
  updateTime();
  updateFanSpeed();

  checkBtn();
  fan();
}

void btnPress(){ //ISR specified function
  // invert state upon interrupt
  // will be caught by checkButton function
  state = !state; 
}

void fan(){ //waits for 30 seconds to turn fan on/off
  if(sec == 0){
    fanOn();
  }else if(sec == 30){
    fanOff();
  }
}

void checkBtn(){ //watches for button press
  if(state != 0){ 
    changeDirection();
  }
}

void twodigits(int number) { //prints time to LCD while checking for single digit numbers to make sure there is a zero in front
  if (number >= 0 && number < 10) {
    lcd.print('0');
    lcd.print(number); 
  }
  else{
    lcd.print(number);
  }
}

void updateTime(){ //function to display real time using RTC module
  lcd.setCursor(0, 0);
  tmElements_t tm;

  //reads RTC for time, then checks digits and prints to LCD
  if(RTC.read(tm)){
    twodigits(tm.Hour);
    lcd.print(':');
    twodigits(tm.Minute);
    lcd.print(':');
    twodigits(tm.Second);
    sec = tm.Second;
  }else if(RTC.chipPresent()){ //quick check to make sure RTC is working correctly
      lcd.print("reset RTC");
    }
}

void fanOn(){ //function used to start fan
    fanSpeed = 128;
    
    if(curDIR == 1){
      digitalWrite(DIR_CC, HIGH); //CW
      digitalWrite(DIR_CCW, LOW); 
      lcd.setCursor(0, 1);
      lcd.print("C ");
      curDIR = 1;
    } 
    else{
      digitalWrite(DIR_CCW, HIGH); //CCW
      digitalWrite(DIR_CC, LOW); 
      lcd.setCursor(0, 1);
      lcd.print("CC ");
      curDIR = 0;
    }

    digitalWrite(ENABLE,fanSpeed); // fan enable on at half speed
}

void fanOff(){ //turns fan off
  fanSpeed = 0;
  digitalWrite(ENABLE, fanSpeed);
}

void changeDirection(){ //used to change direction of fan from earlier button press
  if(curDIR == 1){
    digitalWrite(DIR_CC, LOW);
    digitalWrite(DIR_CCW, HIGH); //CCW
    curDIR = 0;
    //delay(500); //buffer for system to process button press
    lcd.setCursor(0, 1);
    lcd.print("CC ");
  }
  else{
    digitalWrite(DIR_CCW, LOW);
    digitalWrite(DIR_CC, HIGH);
    curDIR = 1;
    //delay(500); //buffer for system to process button press
    lcd.setCursor(0, 1);
    lcd.print("C ");
  }
}

void updateFanSpeed(){ //displaying fan speed
  lcd.setCursor(7, 1);
  lcd.print("Speed:");
  if(fanSpeed == 128){
    lcd.print("0.5");
  }else{
    lcd.print("0.0");
  }
}
