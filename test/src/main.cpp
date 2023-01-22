#include <Arduino.h>

//Motor 1
const int motorPin1 = 2;
const int motorPin2 = 3;
//Motor 2
const int motorPin3 = 4;
const int motorPin4 = 5;

//sonar LEFT
const int trig_LEFT = 17;
const int echo_LEFT = 16;
//sonar MID
const int trig_MID = 19;
const int echo_MID = 18;
//sonar RIGHT
const int trig_RIGHT = 21;
const int echo_RIGHT = 20;

// defines variables
long duration_LEFT;
int distance_LEFT;

long duration_MID;
int distance_MID;

long duration_RIGHT;
int distance_RIGHT;

int speed = 180;

void setup(){
  Serial.begin(9600); // Starts the serial communication
  
  //Set pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  //sonar
  pinMode(trig_LEFT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_LEFT, INPUT); // Sets the echo as an Input
}

void loop()
{
  /*//Motor Control A in both directions
  analogWrite(motorPin1, speed);
  delay(2000);
  analogWrite(motorPin1, 0);
  delay(200);
  analogWrite(motorPin2, speed);
  delay(2000);
  analogWrite(motorPin2, 0);
  //Motor Control B in both directions
  analogWrite(motorPin3, speed);
  delay(2000);
  analogWrite(motorPin3, 0);
  delay(200);
  analogWrite(motorPin4, speed);
  delay(2000);
  analogWrite(motorPin4, 0);*/

  //______________________________________________
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);

  // wait for a second
  delay(100);

  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);

   // wait for a second
  delay(100);
  //______________________________________________
  //LEFT SONAR
  // Clears the trigPin
  digitalWrite(trig_LEFT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_LEFT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_LEFT = pulseIn(echo_LEFT, HIGH);
  // Calculating the distance
  distance_LEFT = duration_LEFT * 0.0343 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance LEFT: ");
  Serial.println(distance_LEFT);

  //MID SONAR
  // Clears the trigPin
  digitalWrite(trig_MID, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_MID, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_MID, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_MID = pulseIn(echo_MID, HIGH);
  // Calculating the distance
  distance_MID = duration_MID * 0.0343 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance MID: ");
  Serial.println(distance_MID);

  //RIGHT SONAR
  // Clears the trigPin
  digitalWrite(trig_RIGHT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_RIGHT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_RIGHT = pulseIn(echo_RIGHT, HIGH);
  // Calculating the distance
  distance_RIGHT = duration_RIGHT * 0.0343 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance RIGHT: ");
  Serial.println(distance_RIGHT);
}