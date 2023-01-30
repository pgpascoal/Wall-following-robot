#include <Arduino.h>

//Motor 1
const int motorPin1 = 2;
const int motorPin2 = 3;
//Motor 2
const int motorPin3 = 4;
const int motorPin4 = 5;

typedef struct {
  int state, new_state;

  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;

fsm_t fsm1, fsm_left, fsm_right, fsm_front;

// Set new state
void set_state(fsm_t& fsm, int new_state)
{
  if (fsm.state != new_state) {  // if the state changed tis is reset
    fsm.state = new_state;
    fsm.tes = millis();
    fsm.tis = 0;
    //Serial.println(fsm_left.state);
  }
}

unsigned long interval, last_cycle;
unsigned long loop_micros;

int left_speed=209, right_speed=230; //right-left=21

void stopped(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}
void forward(){
  analogWrite(motorPin1, left_speed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, right_speed);
}
void turn_RIGHT(){
  analogWrite(motorPin1, left_speed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}
void turn_LEFT(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, right_speed);
}
void backwards(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 128);
  analogWrite(motorPin3, 128);
  analogWrite(motorPin4, 0);
}
void aprox_left(){
  analogWrite(motorPin1, left_speed-90);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, right_speed);
}
void aprox_right(){
  analogWrite(motorPin1, left_speed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, right_speed-90);
}

//sonar LEFT
const int trig_LEFT = 17;
const int echo_LEFT = 16;
//sonar FRONT
const int trig_FRONT = 19;
const int echo_FRONT = 18;
//sonar RIGHT
const int trig_RIGHT = 21;
const int echo_RIGHT = 20;

// defines variables
long duration_LEFT;
int distance_LEFT;

long duration_FRONT;
int distance_FRONT;

long duration_RIGHT;
int distance_RIGHT;

int L_sonar=0, R_sonar=0, F_sonar=0;

int speed = 180;

void setup(){
  Serial.begin(9600); // Starts the serial communication
  
//Set pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  
  //sonar
  pinMode(trig_LEFT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_LEFT, INPUT); // Sets the echo as an Input

  pinMode(trig_RIGHT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_RIGHT, INPUT); // Sets the echo as an Input

  pinMode(trig_RIGHT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_RIGHT, INPUT); // Sets the echo as an Input

  set_state(fsm1, 0);
  set_state(fsm_left, 0);
  set_state(fsm_right, 0);
  set_state(fsm_front, 0);

  interval = 10;

}

void loop()
{ 
  unsigned long now = millis();
  if (now - last_cycle > interval) {
    loop_micros = micros();
    last_cycle = now;

    //______________________________________________
    //LEFT SONAR
    // Clears the trigPin
    digitalWrite(trig_LEFT, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on 255 state for 10 micro seconds
    digitalWrite(trig_LEFT, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_LEFT, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration_LEFT = pulseIn(echo_LEFT, HIGH);
    // Calculating the distance
    distance_LEFT = duration_LEFT * 0.0343 / 2;
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance LEFT: ");
    //Serial.println(distance_LEFT);

    //FRONT SONAR
    // Clears the trigPin
    digitalWrite(trig_FRONT, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on 255 state for 10 micro seconds
    digitalWrite(trig_FRONT, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_FRONT, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration_FRONT = pulseIn(echo_FRONT, HIGH);
    // Calculating the distance
    distance_FRONT = duration_FRONT * 0.0343 / 2;
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance FRONT: ");
    //Serial.println(distance_FRONT);

    //RIGHT SONAR
    // Clears the trigPin
    digitalWrite(trig_RIGHT, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on 255 state for 10 micro seconds
    digitalWrite(trig_RIGHT, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_RIGHT, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration_RIGHT = pulseIn(echo_RIGHT, HIGH);
    // Calculating the distance
    distance_RIGHT = duration_RIGHT * 0.0343 / 2;
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance RIGHT: ");
    //Serial.println(distance_RIGHT);
    //_________________________________
    
    // FSM processing

    // Update tis for all state machines
    unsigned long cur_time = millis();   // Just one call to millis()
    fsm1.tis = cur_time - fsm1.tes;
    fsm_left.tis = cur_time - fsm_left.tes;
    fsm_right.tis = cur_time - fsm_right.tes;
    fsm_front.tis = cur_time - fsm_front.tes;

    //---------CALCULATE IF WALL DETECTED------------

    // Calculate next state for the Left_sonar state machine
    
    /*if(fsm_left.state == 0 && (distance_LEFT<20)){
      fsm_left.new_state = 1;
    } else if(fsm_left.state == 1 && (distance_LEFT>20)){
      fsm_left.new_state = 0;
    } */

    if(fsm_left.state == 0 && (distance_LEFT<20) && (distance_LEFT>15)){
      fsm_left.new_state = 1;
      //Serial.println("Esstou dentro");
    } else if(fsm_left.state == 1 && (distance_LEFT>20) && (distance_LEFT<50)){
      fsm_left.new_state = 2;
      //Serial.println("DIREITA");
    } else if(fsm_left.state == 1 && (distance_LEFT<15)){
      fsm_left.new_state = 3;
      //Serial.println("ESQ");
    } else if((distance_LEFT>50)){
      fsm_left.new_state = 0;
    }

    // Update the states
    set_state(fsm_left, fsm_left.new_state);

    // Actions set by the current state of the Left_sonar state machine

    if(fsm_left.state == 1){
      L_sonar = 1;
    } else if(fsm_left.state == 2){
      L_sonar = 2;
    } else if(fsm_left.state == 3){
      L_sonar = 3;
    } else if(fsm_left.state == 0){
      L_sonar = 0;
    }

    // Calculate next state for the Right_sonar state machine
    
    if(fsm_right.state == 0 && (distance_RIGHT<20)){
      fsm_right.new_state = 1;
    } else if(fsm_right.state == 1 && (distance_RIGHT>20)){
      fsm_right.new_state = 0;
    } 

    // Update the states
    set_state(fsm_right, fsm_right.new_state);

    // Actions set by the current state of the Right_sonar state machine

    if(fsm_right.state == 1){
      R_sonar = 1;
    } else if(fsm_right.state == 0){
      R_sonar = 0;
    }
    // Calculate next state for the Front_sonar state machine
    
    if(fsm_front.state == 0 && (distance_FRONT<20)){
      fsm_front.new_state = 1;
      //Serial.println("estou dentro de 10");
    } else if(fsm_front.state == 1 && (distance_FRONT>20)){
      fsm_front.new_state = 0;
      //Serial.println("NAONAONAONANOA");
    } 

    // Update the states
    set_state(fsm_front, fsm_front.new_state);

    // Actions set by the current state of the Front_sonar state machine

    if(fsm_front.state == 1){
      F_sonar = 1;
    } else if(fsm_front.state == 0){
      F_sonar = 0;
    }
    //---------FINISHED CALCULATING IF WALL DETECTED------------

    /*if(fsm1.state == 0 && !F_sonar && (L_sonar==0) && !R_sonar){
      fsm1.new_state = 1;
    } else if(fsm1.state == 1 && (F_sonar || (L_sonar==1) || R_sonar)){
      fsm1.new_state = 0;
    }
    Serial.print("SONAR ESQ: ");Serial.println(L_sonar);
    Serial.print("SONAR FRONT: ");Serial.println(F_sonar);
    Serial.print("SONAR DIR: ");Serial.println(R_sonar);
    Serial.println(distance_LEFT);

    // Update the states
    set_state(fsm1, fsm1.new_state);

    // Actions set by the current state of the first state machine
    if(fsm1.state == 1 ){
      forward();
    } else if (fsm1.state == 0){
      turn_LEFT();
    }*/
    
    // Calculate next state for the first state machine
    
    if(fsm1.state == 0 && !F_sonar && (L_sonar==0) && !R_sonar){
      fsm1.new_state = 1;
    } else if(fsm1.state == 1 && (F_sonar || L_sonar==1 || R_sonar)){
      fsm1.new_state = 0;
    } else if(fsm1.state == 0 && F_sonar){
      fsm1.new_state = 2;
    } else if(fsm1.state == 2 && (L_sonar==1 || L_sonar==2 || L_sonar==3) && !F_sonar){
      fsm1.new_state = 0;
    } else if(fsm1.state == 0 && R_sonar && !F_sonar && (L_sonar==0)){
      fsm1.new_state = 4;
    } else if(fsm1.state == 4 && (L_sonar==1 || L_sonar==2 || L_sonar==3) && !F_sonar && !R_sonar){
      fsm1.new_state = 0;
    } else if(fsm1.state == 0 && (L_sonar==1 || L_sonar==2 || L_sonar==3) && !F_sonar && !R_sonar){
      fsm1.new_state = 3;
    } else if(fsm1.state == 3 && !R_sonar && !F_sonar && (L_sonar==0)){
      fsm1.new_state = 0;
    } else if(fsm1.state == 3 && F_sonar && (L_sonar==1 || L_sonar==2 || L_sonar==3)){
      fsm1.new_state = 2;
    } else if(fsm1.state == 3 && (L_sonar==3)){
      fsm1.new_state = 6;
    } else if(fsm1.state == 3 && (L_sonar==2)){
      fsm1.new_state = 5;
    } else if(fsm1.state == 6 && (L_sonar==1)){
      fsm1.new_state = 3;
    } else if(fsm1.state == 5 && (L_sonar==1)){
      fsm1.new_state = 3;
    } else if(fsm1.state == 6 && (L_sonar==0)){
      fsm1.new_state = 0;
    } else if(fsm1.state == 5 && (L_sonar==0)){
      fsm1.new_state = 0;
    } //se não der tentar com o estado 5 e 6 adicionar condição de se detetar F_sonar
    
    // Update the states
    set_state(fsm1, fsm1.new_state);

    // Actions set by the current state of the first state machine
    if(fsm1.state == 1 ){
      forward();
    } else if(fsm1.state == 3){
      forward();
    } else if(fsm1.state == 2){
      turn_RIGHT();
    } else if(fsm1.state == 4){
      turn_LEFT();
    } else if(fsm1.state == 0){
      stopped();
    } else if(fsm1.state == 5){
      aprox_left();
    } else if(fsm1.state == 6){
      aprox_right();
    }
  }
}
/*

if(fsm_left.state == 0 && (distance_LEFT<15)){
      fsm_left.new_state = 1;
    } else if(fsm_left.state == 1 && (distance_LEFT>=15)){
      fsm_left.new_state = 0;
    } 

    // Update the states
    set_state(fsm_left, fsm_left.new_state);

    // Actions set by the current state of the left_sonar state machine

    if(fsm_left.state == 1){
      L_sonar = 1;
    } else if(fsm_left.state == 0){
      L_sonar = 0;
    }
*/