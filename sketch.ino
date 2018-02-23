const int colorsCount = 3;
const int colorButtons[] = {8, 9, 10, 11};
const char* colorNames[] = {"red", "green", "blue", "yellow"};
const int colorLEDs[] = {2, 3, 4, 5};
const int STATE_SIMON_TURN = 0;
const int STATE_USER_TURN = 1;
const int STATE_LOSE = 2;


int guessIndex = 0;
int sequenceLength = 0;
int sequence[128];
int state;

void setup() {
  for (int i = 0; i < colorsCount; i++) {
    // Set buttons to input, using internal pull-up resistor
    pinMode(colorButtons[i], INPUT_PULLUP);
    // Set LEDs to output
    pinMode(colorLEDs[i], OUTPUT);
  }
  Serial.begin(9600);
  randomSeed(analogRead(0));
  startNewGame();
}

void startNewGame() {
  sequenceLength = 0;
  state = STATE_SIMON_TURN;
  guessIndex = 0;
}

void addToSequence() {
  sequence[sequenceLength] = randomColor();
  sequenceLength++;
}

void displaySequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int value = sequence[i];
    digitalWrite(colorLEDs[value], HIGH);
    Serial.print("simon on " + colorLEDs[value]);
    delay(500);
    digitalWrite(colorLEDs[value], LOW);
    Serial.print("simon off " + colorLEDs[value]);
    delay(150);
  }
}

int randomColor() {
  return random(colorsCount);
}

void simonTurn() {
  delay(750);
  addToSequence();
  displaySequence();
}

//// returns TRUE if user lost on their turn
//bool userTurn() {
//  //  bool buttonIsHeldDown = false;
//
//  while (guessIndex < sequenceLength) {
//    for (int i = 0; i < colorsCount; i++) {
//      bool isPressed = LOW == digitalRead(colorButtons[i]);
//      if (isPressed) {
//        //        buttonIsHeldDown = true;
//        if (i == sequence[guessIndex]) {
//          digitalWrite(colorLEDs[i], HIGH);
//          Serial.print("user on " + colorLEDs[i]);
//          delay(150);
//        }
//      }
//      digitalWrite(colorLEDs[i], isPressed);
//    }
//
//  }
//
//
//  for (int i = 0; i < colorsCount; i++) {
//    digitalWrite(colorLEDs[i], LOW);
//
//  }
//
//  //    delay(1000);
//  guessIndex = 0;
//  return false;
//}


bool userTurn() {

  int actualColor;
  bool isPressed;
  while (guessIndex < sequenceLength) {



    actualColor = sequence[guessIndex];
    isPressed = false;

    while (!isPressed) {
      for (int i = 0; i < colorsCount; i++) {
        //LOW means pressed
        if (isPressed) continue;
        isPressed = LOW == digitalRead(colorButtons[i]);
        if (isPressed) {
          int guessColor = i;
          if (actualColor != guessColor) return false;

        } else {
          Serial.println("stuck in loop");
        }
      }

    }
    guessIndex++;
    delay(200);

  }

  guessIndex = 0;
  return true;

}



void lose() {
  Serial.println("you lose!");
  for (int i = 0; i < colorsCount; i++) {

    digitalWrite(colorLEDs[i], HIGH);
  }
  delay(3000);

  for (int i = 0; i < colorsCount; i++) {

    digitalWrite(colorLEDs[i], LOW);
  }
  startNewGame();
}

void loop() {
  //  for (int i = 0; i < colorsCount; i++) {
  //    // Since we used pull-up resistors, a button is pressed
  //    // when it's pulled LOW.
  //    bool isPressed = LOW == digitalRead(colorButtons[i]);
  //    if (isPressed) {
  //      Serial.println(colorNames[i]);
  //    }
  //    digitalWrite(colorLEDs[i], isPressed);
  //  }

  if (state == STATE_SIMON_TURN) {
    simonTurn();
    state = STATE_USER_TURN;
  }
  else if (state == STATE_USER_TURN) {
    bool stillPlaying = userTurn();
    if (stillPlaying) {
      state = STATE_SIMON_TURN;
    } else {
      //state = STATE_LOSE;
      lose();


    }
  }

  Serial.println(state);



  delay(10);
}
