#define NumberOfPlants 4
int timerAdjustment = 0;
unsigned long HOUR_DELAY = 60UL * 60UL * 1000UL;
//soil sensor and power multiplex
int soilSamplerSection1 = A1;
int soilSamplerPowerSlot[NumberOfPlants] = {};//2-6 = 5 slots
//pump power array
int powerPumpRelay[NumberOfPlants] = {13, 11, 10, 9};
const int SENSOR_DELAY = 10; // in milliseconds
const int WATER_DELAY = 5000; // 5sec
//water level buzzer
int waterBuzzer = 12;
//sonar variables
const int trigPin = A5;
const int echoPin = A4;
long duration;
int distance;

void setup() {
  for (int i = 0; i < NumberOfPlants; i++) { //2-6 = 5 slots
    soilSamplerPowerSlot[i] = i + 2;
    pinMode(soilSamplerPowerSlot[i], OUTPUT); //2-6 = 5 slots
    pinMode(powerPumpRelay[i], OUTPUT); //{13,11,10,9};
    // Start with soil sensors digital powerslots and pumps OFF
    digitalWrite(soilSamplerPowerSlot[i], LOW);
    digitalWrite(powerPumpRelay[i], LOW); //{13,11,10,9};
  }

  pinMode(soilSamplerSection1, INPUT);
  pinMode(waterBuzzer, OUTPUT);

  //start with buzzer off
  digitalWrite(waterBuzzer, LOW);

  //sonar
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Setup serial plotter.
  Serial.begin(9600);
}


void loop() {
  //reset soilWaterLevel to 0 every time unless we need to compare for some reason.
  int soilWaterLevels[NumberOfPlants] = {0};
  //reset timerAdjustment as we will use it to subtract time from the hour delay.
  timerAdjustment = 0;
  //turns on the power to a sensor,reads and stores data, turns off and moves to the next
  for (int i = 0; i < NumberOfPlants; i++) {
    digitalWrite(soilSamplerPowerSlot[i], HIGH);
    delay(SENSOR_DELAY);
    timerAdjustment+=SENSOR_DELAY;
    soilWaterLevels[i] = analogRead(soilSamplerSection1);
    Serial.println(analogRead(soilSamplerSection1));
    digitalWrite(soilSamplerPowerSlot[i], LOW);
    delay(SENSOR_DELAY);
    timerAdjustment+=SENSOR_DELAY;
  }//2hours 3 min in stream

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  timerAdjustment+= 0.002;// shouldnt matter as an int will trunc it
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  timerAdjustment+=0.01; //shouldnt matter as an int will trunc it
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  timerAdjustment+= (duration/1000);//will only matter if its bigger than 1ms will trunc decimal
  Serial.print("duration: ");
  Serial.println(duration);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println();

  if (distance > 10) { //min water level, meaning only 2ish cm of water left
    digitalWrite(waterBuzzer, HIGH);
    delay(2000);
    timerAdjustment+=2000;
    digitalWrite(waterBuzzer, LOW);
  } else {
    for (int i = 0; i < NumberOfPlants; i++) {
      if (soilWaterLevels[i] > 700) { //1000 is dry, 200 is in water
        digitalWrite(powerPumpRelay[i], HIGH); //{13,11,10,9};
        delay(WATER_DELAY); //5sec
        timerAdjustment += WATER_DELAY;
        digitalWrite(powerPumpRelay[i], LOW); //{13,11,10,9};
        delay(SENSOR_DELAY);//10ms
        timerAdjustment += SENSOR_DELAY;
      }
    }
  }
  delay(HOUR_DELAY - timerAdjustment); //started a few sec before 2:49 am
  //delay(2000);//tester
}
//***************************NOTES***************************
// Define our timer.
//delay() Pauses the program for the amount of time (in milliseconds)
//specified as parameter. (There are 1000 milliseconds in a second.)
//delay (60UL * 60UL * 1000UL); is an hour min/sec/milli
////Yes, the analog pins on the Arduino can be used as digital outputs. You can always use the analog pins for digital writing. ... analogRead() works only on analog pins. It can take any value between 0 and 1023.
//}//check stream st 48 min for ideas on power and data managment
// wait untill 700 to add water for the twin legged ones. wet/damp is around 200 and less, water is ~150 300 is in water partially submerged.
//big fatties dry = ~160 and wet/damp is 140-150 and water is 120 and lower
