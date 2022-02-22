// set up LED, buzzer, & button pin numbers
int redLED = 1;
int yellowLED = 2;
int greenLED = 3;
int buzzer = 4;
int button = 5;

int clkStep = 200; // set clock to 200ms
int counter = 0; // initialize ms counter

void setup() {
  // set LEDs and buzzer as output, button as input
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  // ensure LEDs and buzzer start off
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(buzzer, LOW);

  counter = 0;
  // blinking RED loop to begin, broken by button press
  while (digitalRead(button) == 0) { 
    if (counter == 1000)
      digitalWrite(redLED, LOW);
    else if (counter == 2000) {
      digitalWrite(redLED, HIGH);
      counter = 0;
    }
    counter = counter + clkStep;
    delay(clkStep);
  }

  counter = 0; // reset counter
  digitalWrite(redLED, HIGH); //begin with RED on
  int curLight = 0; // current illuminated light
                    // 0 = red, 1 = green, 2 = yellow
  while (1 == 1) {
    if (curLight == 0) {
      // currently RED
      if (counter == 20000) {
        // RED over, turn off LED, reset counter, iterate curLight to GREEN
        digitalWrite(redLED, LOW);
        curLight = 1;
        counter = 0;
      }
      else if (counter >= 17000) {
        // RED almost over, keep LED on and turn buzzer on
        digitalWrite(redLED, HIGH);
        tone(buzzer, 500);
      }
      else {
        // RED in progress, keep LED on, ensure buzzer off
        digitalWrite(redLED, HIGH);
        noTone(buzzer);
      }
    }

    else if (curLight == 1) {
      // currently GREEN
      if (counter == 20000) {
        // GREEN over, turn off LED, reset counter, iterate curLight to YELLOW
        digitalWrite(greenLED, LOW);
        curLight = 2;
        counter = 0;
      }
      else if (counter >= 17000) {
        // GREEN almost over, keep LED on and turn buzzer on
        digitalWrite(greenLED, HIGH);
        tone(buzzer, 500);
      }
      else {
        // GREEN in progress, keep LED on, ensure buzzer off
        digitalWrite(greenLED, HIGH);
        noTone(buzzer);
      }
    }

    else if (curLight == 2) {
      // currently YELLOW
      if (counter == 6000) {
        // YELLOW over, turn off LED, reset counter, iterate curLight to RED
        digitalWrite(yellowLED, LOW);
        curLight = 0;
        counter = 0;
      }
      else if (counter >= 3000) {
        // YELLOW almost over, blink LED and turn buzzer on
        int blinkNum = counter / 200;
        if (blinkNum % 2 == 0) { digitalWrite(yellowLED, HIGH); }
        else { digitalWrite(yellowLED, LOW); }
        tone(buzzer, 500);
      }
      else {
        // YELLOW in progress, keep LED on, ensure buzzer off
        digitalWrite(yellowLED, HIGH);
        noTone(buzzer);
      }
    }
    counter = counter + clkStep;
    delay(clkStep);
  }
}
