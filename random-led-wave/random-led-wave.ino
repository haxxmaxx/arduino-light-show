/*
Random LED wave

Picks a random LED out of six and starts a wave from it to the other LEDs. The interval between is each flash is also random.
The max gap between flashes and wave speed can be set via the analog input.

Circuit:
- 6 LED lights with one 470 ohm resistor, connected to every PWM pin
- A potentiometer connected to A0 for input
*/

const int BIG_PI = 314; // PI * 100 rounded to an int
const int OUT_PINS[] = {3, 5, 6, 9, 10, 11}; // all PWM pins
const int PINS_LENGTH = 6;
const int IN_PIN = A0;
const int WAIT = 10;

int step = 10;
int intensity = 160;
int minGap = 800;
int maxGap = 5 * minGap;
int nextStart = 0;
int timePassed = 0;
int phases[] = {0, 0, 0, 0, 0, 0};
int centerIndex = -1;
int waveDelay = 100;

void setup(){
  Serial.begin(9600);
  for (byte i = 0; i < PINS_LENGTH; i = i + 1) {
    pinMode(OUT_PINS[i], OUTPUT);
  }
}


void loop(){
  if (timePassed == 0) {
    // randomly pick when next led will turn on
    nextStart = random(minGap, maxGap);
  } else if (timePassed >= nextStart) {
    // find the next led to turn on, then reset timePassed and phases
    Serial.println("start");
    centerIndex = random(PINS_LENGTH);
    timePassed = -WAIT;
    for (byte i = 0; i < PINS_LENGTH; i = i + 1) {
      phases[i] = 0;
    }
  }

  // update all phases for all flashing LEDs
  for (byte i = 0; i < PINS_LENGTH; i = i + 1) {
    int phase = phases[i];
    
    if (phase > -1 && timePassed > waveDelay * abs(centerIndex - i)) {
      if (phase > (2 * BIG_PI)) {
        phases[i] = -1;
      } else {
        phase = phase + step;
        phases[i] = phase;
      }

      float angle = float(phase) / float(100);
      double brightness = (-cos(angle) + 1) / 2 * intensity;
      analogWrite(OUT_PINS[i], brightness);
    }  
  }

  int sensorValue = analogRead(IN_PIN);
  // change the maxGap
  // maxGap = int((1023 - sensorValue) * 6);
  // change the wave speed
  waveDelay = sensorValue;
  minGap = sensorValue * 6 + 100;
  maxGap = 5 * minGap;

  timePassed = timePassed + WAIT;
  delay(WAIT);
}