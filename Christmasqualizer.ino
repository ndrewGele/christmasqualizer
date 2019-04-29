/*
*
* Christmasqualizer
* Copyright 2011 Hotchkissmade
* Released under the GPL
*
* Special thanks for the help from the following:
* http://nuewire.com/info-archive/msgeq7-by-j-skoba/
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
// Blinking Parameters!
// All you need to change is here!
//
double spread = 0.011; // Number of standard deviations above/below threshold required for lights to change
double multi = 0.985; // Running average will be multiplied by this value to set threshold. Turn down for lights to be on more often. Turn up for lights to be on less often.
int sampleRate = 5; // How many loops to wait before writing new value to array for running average.
int wait = 10; // Short delay before looping to prevent lights from being too flickery
// Running average will be based on a sample of 30 readings from the last [sampleRate] * ([wait] + [compute time]) miliseconds.

//
// Initialize matrix for running averages
//
double thresh = 0.00; // Just initializing
int sample[7][30] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
double stDevs[7] = {0,0,0,0,0,0,0};


// Init some other stuff
int loopCount = 1; // +1 each time the code loops
int sampleWrite[7] = {0,0,0,0,0,0,0}; // Keeps track of sample array index


//
// Define some functions we'll use
//

// Mean function
double mean(int arr[], int size) {

   int i;
   double avg;
   double sum = 0;

   for (i = 0; i < size; i++) {
      sum += arr[i];
   }

   avg = sum / size;

   return avg;
}

// Standard Deviation Function
double sd(int arr[], int size, double mean) {
  
  double stDev;
  double sqDevSum = 0.0;

  for(int i = 0; i < size; i++) {
    sqDevSum += pow((mean - float(arr[i])), 2);
  }

  stDev = sqrt(sqDevSum/float(size));

  return stDev;
}
  

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
    
    int value = analogRead(analogPin);

    // Set threshold using the function defined above
    thresh = mean(sample[i], 30) * multi; // Turn this down if you think lights aren't staying on long enough  

    // Turn lights on or off based on thresh
    if (thresh < 69) { // If average is really low, there probably isn't music playing, so just turn the lights on!
      digitalWrite(pins[i], HIGH); 
    } else if (value < thresh - (stDevs[i] * spread)) { 
        digitalWrite(pins[i], LOW);
      } else if (value > thresh + (stDevs[i] * spread)) {
          digitalWrite(pins[i], HIGH); 
        }
      
    digitalWrite(strobePin, HIGH);

    // Check for occasional loop of code to write value to our running sample
    if (loopCount == sampleRate) {

      // Compute standard deviation and write to array
      stDevs[i] = sd(sampleWrite[i], 30, mean(sample[i], 30));
      
      // Write value to array and increase index
      sample[i][sampleWrite[i]] = value;
      sampleWrite[i]++;
      
      // If end of array, loop back to beginning
      if (sampleWrite[i] == 29) {
        sampleWrite[i] = 0;
      }

      // Reset loop counter
      loopCount = 1;
    
    } else {

      // Increase loop counter
      loopCount++;
    
    }
  
  } // end of primary loop

  // Short delay before next loop so lights aren't too spazzy
  delay(wait);

}
