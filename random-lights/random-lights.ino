/*
Random lights

Picks a random LED out of six and flashes it. The time between is each flash is also random.
The max gap between flashes can be set via the analog input.
A LED that is currently flashed can't be flashed, so making sure it finds a LED that is off.

Circuit:
- 6 LED lights with one 470 ohm resistor, connected to every PWM pin
- A potentiometer connected to A0 for input
*/

const int BIG_PI = 314; // PI * 100 rounded to an int
const int OUT_PINS[] = {3, 5, 6, 9, 10, 11}; // all PWM pins
const int PINS_LENGTH = 6;
const int IN_PIN = A0;
const int WAIT = 10;
const int MIN_GAP = 100;

int maxGap = 3000;
int nextStart = 0;
int timePassed = 0;
int phases[] = {0, 0, 0, 0, 0, 0};
int step = 10;
int intensity = 160;

void setup(){
  Serial.begin(9600);
  for (byte i = 0; i < PINS_LENGTH; i = i + 1) {
    pinMode(OUT_PINS[i], OUTPUT);
  }
}


void loop(){
  if (timePassed == 0) {
    // randomly pick when next led will turn on
    nextStart = random(MIN_GAP, maxGap);
  } else if (timePassed >= nextStart) {
    // reset time passed and find the next led to turn on
    timePassed = -WAIT;

    // create a random index and a random step to itereate over to find an empty spot
    int initIndex = random(PINS_LENGTH);
    int isPositive = random(2);
    int offset;
    if (isPositive == 1) {
      offset = 1;
    } else {
      offset = 5;
    }
    
    // Tries initIndex, if it is flashing, try all other till a LED that is off is found.
    for (byte i = 0; i < PINS_LENGTH; i = i + 1) {
      int testIndex = (initIndex + offset * i) % PINS_LENGTH;
      if (phases[testIndex] == 0) {
        phases[testIndex] = 1;
        break;
      }
      Serial.println("index occupied");
    }
  }

  // update all phases for all flashing LEDs
  for (byte i = 0; i < PINS_LENGTH; i = i + 1) {
    int phase = phases[i];
    
    if (phase > 0) {
      if (phases[i] > (2 * BIG_PI)) {
        phases[i] = 0;
      } else {
        phase = phase + step;
        phases[i] = phase;
      }

      float angle = float(phase) / float(100);
      double brightness = (-cos(angle) + 1) / 2 * intensity;
      analogWrite(OUT_PINS[i], brightness);
    }  
  }

  // change the maxGap
  int sensorValue = analogRead(IN_PIN);
  maxGap = int((1023 - sensorValue) * 4);

  timePassed = timePassed + WAIT;
  delay(WAIT);
}