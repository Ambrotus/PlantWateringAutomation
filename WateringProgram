// Define our timer.
//delay() Pauses the program for the amount of time (in milliseconds)
//specified as parameter. (There are 1000 milliseconds in a second.)
const int TIMER = 10; // in seconds
const int RESET = TIMER - 1;
const int SENSOR_DELAY = 10; // in milliseconds
int counter;

#define NumberOfPlants 4
int soilSamplerSection1 = A1;
int soilSamplerPowerSlot[NumberOfPlants] = {};//2-6 = 5 slots
int powerPumpRelay[NumberOfPlants] = {13,11,10,9};

//Yes, the analog pins on the Arduino can be used as digital outputs. You can always use the analog pins for digital writing. ... analogRead() works only on analog pins. It can take any value between 0 and 1023.
//sonar variables
const int trigPin = A5;
const int echoPin = A4;
long duration;
int distance;

int waterBuzzer = 12;

void setup() {
  for (int i = 0; i < NumberOfPlants; i++) { //2-6 = 5 slots
    soilSamplerPowerSlot[i] = i + 2;
  }

  pinMode(soilSamplerSection1, INPUT);
  pinMode(waterBuzzer, OUTPUT);

  for (int i = 0; i < NumberOfPlants; i++) { //2-6 = 5 slots
    pinMode(soilSamplerPowerSlot[i], OUTPUT);
  }

  for (int i = 0; i < NumberOfPlants; i++) { //2-6 = 5 slots
    pinMode(powerPumpRelay[i], OUTPUT); //{13,11,10,9};
  }
  
  
  // Start with soil sensors digital powerslots and pumps OFF
  for (int i = 0; i < NumberOfPlants; i++) {
    digitalWrite(soilSamplerPowerSlot[i], LOW);
    digitalWrite(powerPumpRelay[i], LOW); //{13,11,10,9};
  }
  //start with  pumps off and buzzer off
  digitalWrite(powerPumpRelay, LOW);
  digitalWrite(waterBuzzer, LOW);

  // Setup timer.
  counter = RESET;

  //sonar
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Setup serial plotter.
  Serial.begin(9600);
}


void loop() {
  //reset soilWaterLevel to 0 every time unless we need to compare for some reason.
  //might make this an array as we have like 12 plants. Actually we should due to having several kinds of soil sensors and different measurment thresholds
  int soilWaterLevels[NumberOfPlants] = {0};

  // If the counter is at the end of a cycle
  // take a new reading and reset counter.
  if (counter <= 0) {



    // Take reading, send to Serial plotter/monitor and store variables into array.


    //turns on the power to a sensor,reads and stores data, turns off and moves to the next
    for (int i = 0; i < NumberOfPlants; i++) {
      digitalWrite(soilSamplerPowerSlot[i], HIGH);
      delay(SENSOR_DELAY);
      soilWaterLevels[i] = analogRead(soilSamplerSection1);
      Serial.println(analogRead(soilSamplerSection1));

      digitalWrite(soilSamplerPowerSlot[i], LOW);
      delay(SENSOR_DELAY);
    }//2hours 3 min in stream
    Serial.println();

    // Turn sensor OFF again.



    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    Serial.print("duration: ");
    Serial.println(duration);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance > 10) { //min water level
      digitalWrite(waterBuzzer, HIGH);
      delay(1000);
      digitalWrite(waterBuzzer, LOW);
    } else {
      for (int i = 0; i < NumberOfPlants; i++) {
        if (soilWaterLevels[i] > 400) { //1000 is dry, 200 is in water
          //digitalWrite(powerPumpRelay, HIGH);
          digitalWrite(powerPumpRelay[i], HIGH); //{13,11,10,9};
          delay(SENSOR_DELAY + 10000); //10
          //digitalWrite(powerPumpRelay, LOW);
          digitalWrite(powerPumpRelay[i], LOW); //{13,11,10,9};
        }
      }
    }
    // Start new countdown.
    counter = RESET; //TIMER - 1
  }

  // If counter isn't at the end yet, pause for
  // the same amount of time as if the sensor
  // had been activated to keep things in sync.
  else {
    delay(SENSOR_DELAY);
  }

  // Decrement counter and delay until next second.
  counter--;
  delay(1000 - SENSOR_DELAY);
}//check stream st 48 min for ideas on power and data managment
// wait untill 700 to add water for the twin legged ones. wet/damp is around 200 and less, water is ~150 300 is in water partially submerged.
//big fatties dry = ~160 and wet/damp is 140-150 and water is 120 and lower
