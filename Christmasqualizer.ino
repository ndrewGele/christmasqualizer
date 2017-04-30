/*
*
* Christmasqualizer
* Copyright 2011 Hotchkissmade
* Released under the GPL
*
* Special thanks for the help from the following:
* http://nuewire.com/info-archive/msgeq7-by-j-skoba/
*
* 4/29/2017 Updated/Forked by Andrew Gelé
* Threshold is now a pseudo running average instead of just using an arbitrary value. I'm sure this can be improved upon. I'm not used to programming things this difficult. LOL
* Also made thresholds specific to each pin. Otherwise you just wouldn't see any treble lights in a bassy song.
*
*/


//
// MSGEQ7 interface pins. See above link for more info.
//
int analogPin = A0;
int strobePin = 11;
int resetPin = 12;
int powerPin = 7;

//
// Control pins, from lows to highs in the audio range.
// This MUST have seven values to work correctly.
//
int pins[] = { 2, 3, 4, 5, 8, 9, 10 };

//
// Threshold Vars
//
int lowest[7] = {999, 999, 999, 999, 999, 999, 999}; //Starts off high so that it's updated immediately
int highest[7] = {0, 0, 0, 0, 0, 0, 0}; // Opposite of above
int thresh[7] = {0, 0, 0, 0, 0, 0, 0};

int repeat = 0; // Used for recalibration at the bottom

//
// Main function stuff
//
void setup() {
 Serial.begin(9600);
 pinMode(analogPin, INPUT);
 pinMode(strobePin, OUTPUT);
 pinMode(resetPin, OUTPUT);
 pinMode(powerPin, OUTPUT);

 analogReference(DEFAULT);
 
 for (int i = 0; i < 7; i++) {
  pinMode(pins[i], OUTPUT);
 }

 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);
 digitalWrite(powerPin, HIGH);
}

void loop() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delay(3); // Turn this down to make lights spazzier, turn this up to make them more stable
    int value = analogRead(analogPin);

    // Overwrite low and high values to keep the threshold 
    if (value <= lowest[i]) {lowest[i] = value;}
    if (value >= highest[i]) {highest[i] = value;}
                   
    thresh[i] = ((lowest[i] + highest[i])/2) * 1.00; // Tweak this if you think the threshold is just too low or too high overall
      
    if ((highest[i] - lowest[i]) < 24) { // Checks for variance (Lights stay on with no input)
      digitalWrite(pins[i], HIGH); 
    } else if (value < thresh[i] * 1.00) { // Turn this down if you think lights aren't staying on long enough  
        digitalWrite(pins[i], LOW);
      } else if (value > thresh[i] * 1.05) { // Bring this up if you think lights are turning on from sounds that are too soft
          digitalWrite(pins[i], HIGH); 
        }
      
    digitalWrite(strobePin, HIGH);

    repeat++; // See below.
  
  }

  // This bit just resets lowest and highest every so often so that variables can all recalibrate.
  if (repeat%500 == 0) {
    for (int i = 0; i < 7; i++) {
      lowest[i] = 999;
      highest[i] = 0;
    }
    repeat = 0;
  }

}

