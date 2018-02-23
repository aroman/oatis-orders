const int colorsCount = 3;
const int colorButtons[] = {8, 9, 10, 11};
const char* colorNames[] = {"red", "green", "blue", "yellow"};
const int colorLEDs[] = {2, 3, 4, 5};
const int STATE_SIMON_TURN = 0;
const int STATE_USER_TURN = 1;
const int STATE_LOSE = 2;
const int buzzer = 13; 
const int colorTones[]={400,800,1000,1200};
int sum=0;


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
  guessIndex = 0;
  for (int i=0;i<sequenceLength;i++){
    sum+=sequence[i];
  }
  randomSeed(sum);
  
}

void addToSequence() {
  sequence[sequenceLength] = randomColor();
  sequenceLength++;
}

void playSound(int color) {
  tone(buzzer,colorTones[color]);
  delay(500);
  noTone(buzzer);
}

void displaySequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int value = sequence[i];
    digitalWrite(colorLEDs[value], HIGH);
    playSound(value);
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
          digitalWrite(colorLEDs[guessColor],HIGH);
          playSound(guessColor);
          digitalWrite(colorLEDs[guessColor],LOW);
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
