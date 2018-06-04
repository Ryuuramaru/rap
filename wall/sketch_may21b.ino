#include <NewPing.h>

#define SONAR_NUM     2 // Number or sensors.
#define MAX_DISTANCE 800 // Maximum distance to ping. Edit the distance as needed.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define trigPin1 1 // yellow wire
#define echoPin1 0 // white wire
#define trigPin2 4 //Yellow wire
#define echoPin2 3 //White wire
//if the car's wheels are going backwards. Edit these pins as needed
const int motor_front[] = {2, 4}; //Front Pins
const int motor_back[] = {7, 8}; //Rear Pins
int rangeFront = 0;
int rangeWall = 0;
//rangeF[1] results from pins 9 and 6
//rangeF[2] results from pins 12 and 13
//int wayToClose = 800;

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(trigPin1, echoPin1, MAX_DISTANCE), //Front
  NewPing(trigPin2, echoPin2, MAX_DISTANCE) //Side
};//endArray

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // Setup motors
  int i;
  //setting up the motor contols
  for(i = 0; i < 2; i++){
    pinMode(motor_back[i], OUTPUT);
    pinMode(motor_front[i], OUTPUT);
    //setting up the SRF05
  }//end for
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  digitalWrite(trigPin1, LOW);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  digitalWrite(trigPin2, LOW);

}

void loop() {

  

}


