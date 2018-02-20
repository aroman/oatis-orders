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

  startNewGame();
}

void startNewGame() {
  sequenceLength = 0;
  state = STATE_SIMON_TURN;
}

void addToSequence() {
  sequence[sequenceLength] = randomColor();
  sequenceLength++;
}

void displaySequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int value = sequence[i];
    digitalWrite(colorLEDs[value], HIGH);
    delay(500);
    digitalWrite(colorLEDs[value], LOW);
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

// returns TRUE if user lost on their turn
bool userTurn() {
  //  bool buttonIsHeldDown = false;
  while (guessIndex < sequenceLength) {
    for (int i = 0; i < colorsCount; i++) {
      bool isPressed = LOW == digitalRead(colorButtons[i]);
      if (isPressed) {
        //        buttonIsHeldDown = true;
        if (i == sequence[guessIndex]) {
          guessIndex++;
          digitalWrite(colorLEDs[i], HIGH);
          delay(150);
        }
      }
      digitalWrite(colorLEDs[i], isPressed);
    }
    //    delay(1000);
  }

  guessIndex = 0;
  return false;
}

void lose() {
  Serial.println("you lose!");
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
    bool didLose = userTurn();
    if (didLose) {
      state = STATE_LOSE;
    } else {
      state = STATE_SIMON_TURN;
    }
  }

  Serial.println(state);



  delay(10);
}
