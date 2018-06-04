const int trigPin = 3;
const int echoPin = 2;
const int trigPin2 = 6;
const int echoPin2 = 5;
// defines variables
long duration, duration2;
int distance, distance2;
int E1 = 10;
int M1 = 13;
int E2 = 11;
int M2 = 12;
int value=225;

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT);
Serial.begin(9600); // Starts the serial communication
}
void loop() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);

digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
distance2 = duration2*0.034/2;
//Serial.print("Distance2: ");
//Serial.println(distance2);
void drive_forward();

//if(distance <= 20) hard_left();
//else if(distance2 <= 40) turn_left();
//else if(distance2 > 40 && distance2<=60) drive_forward();
//else if(distance2>60) turn_right();
}
void turn_left(){
digitalWrite(M1, HIGH);
digitalWrite(M2, LOW);
analogWrite(E1, value);
analogWrite(E2, 50);
}
void drive_forward(){
digitalWrite(M1, HIGH);
digitalWrite(M2, LOW);
analogWrite(E1, value);
analogWrite(E2, value); 
}
void turn_right(){
digitalWrite(M1, HIGH);
digitalWrite(M2, LOW);
analogWrite(E1, 150);
analogWrite(E2, value);
}
void hard_left(){
digitalWrite(M1, LOW);
digitalWrite(M2, LOW);
analogWrite(E1, value);
analogWrite(E2, 100);
delay(250);
}


