#include <CapacitiveSensor.h>
#include <Bounce2.h> // import Bounce2

int BUZZER = 14;
int LED = 13;

int RCVSNSR = 8;
int SNDSNSR = 10;

int pshBttn = 19;
Bounce pushButton = Bounce();

bool adjustTone = false;

int sensorMin = 1023;  // minimum sensor value
int sensorMax = 0;     // maximum sensor value
int sensorValue;

long lastTime = millis();
int delayTime = 3000;

CapacitiveSensor sensor = CapacitiveSensor(SNDSNSR, RCVSNSR);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(pshBttn, INPUT);

  // attach pbs pin to bounce object
  pushButton.attach(pshBttn);
  pushButton.interval(5);
  
  calibrateSensor();
}

void loop() {
  // update bounce object
  pushButton.update();
  
  sensorValue = sensor.capacitiveSensor(30);
  
  if (pushButton.fell() == true) {
    adjustTone = !adjustTone;
    if (adjustTone){
      Serial.println("Adjusting Tone");
    } else {
      Serial.println("Adjusting Volume");
    }
  }
  
  int freq = map(sensorValue, sensorMin, sensorMax, 0, 1000);
  int val = map(sensorValue, sensorMin, sensorMax, 0, 255);
  if ((millis() - lastTime) > delayTime){
    Serial.println("Reading and mapped");
    Serial.println(sensorValue);
    Serial.println(freq);
    Serial.println(val);
    Serial.println();
    lastTime = millis();
  }
  
  val = sensorValue;
  freq = sensorValue;
  
  if(adjustTone){
    //tone(BUZZER, freq);
  } else {
    //analogWrite(BUZZER, val);
  }
  analogWrite(LED, val);
}

void calibrateSensor() {
  digitalWrite(LED, HIGH);
  
  // calibrate during the first five seconds
  while (millis() < 6000) {
    sensorValue = sensor.capacitiveSensor(30);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }

  // signal the end of the calibration period
  digitalWrite(LED, LOW);
  Serial.println("Max Min");
  Serial.println(sensorMin);
  Serial.println(sensorMax);
  Serial.println();
}
