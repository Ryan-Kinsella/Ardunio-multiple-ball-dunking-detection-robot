//A robot is armed with wheels, distance sensors, bumpers, light sensors, and infrared sensors.
//The robot is in a cage with three platforms with rubber balls on them with one "hoop" to
//socre on. The robot starts directly in the centre of the cage, and based off of a random reading
//from the IR sensor (value 0, 1, or 2) the robot will grab the ball from platforms 0,1, or 2 using
//its arms, and bring the ball to the hoop to dunk on. Once complete, the robot should return to
//its original place and await which platform to grab a ball from next. 
//The goal was to get the most amount of balls in the net IN A ROW as possible within two minuites. 
//I got 3 which was the highest on the marking scheme (and in the class). 



#include <Servo.h>
#include "QSerial.h"
QSerial myIRserial;
//yellow is 3 and 3 is left
//red is 2 and 2 is middle
//black is 1 and 1 is right

//Setup
Servo shoulder, arm, hand;
int touch = 4;
int led = 13;
int rxpin=10;             //input for IR
int txpin=-1;             //non-used input IR
#define LS A0             // left sensor
#define RS A2             // right sensor
#define MS A1             // middle sensor
#define LM1 4             // left motor
#define leftWheel 5        // left motor turn
#define RM1 7             // right motor
#define rightWheel 6       // right motor turn



//Constant Values

//for the purposes of this project, the tolerances are irrelevant since
//the difference between black and white is almost doubled.
int blackTapeMax=350;
int whiteTapeMin=600;
int range = A3;
int infra = 11;
int Speed = 100;
int turnSpeed = 150;
int insideWheelSpeed = 50;
int waitTime = 100; //standard delay time

void setup()
{
  pinMode(LS, INPUT);                   //left sensor
  pinMode(RS, INPUT);                   //right sensor
  pinMode(leftWheel, OUTPUT);           //left turning
  pinMode(rightWheel, OUTPUT);          //right turning
  shoulder.attach(8);                   //used for the arm grabber
  arm.attach(9);                        //used for the arm grabber
  hand.attach(10);                      //used for the arm grabber
  pinMode(touch, INPUT);
  pinMode(led, OUTPUT);
  pinMode(range,INPUT);                 //infra red distance sensor
  pinMode(infra,INPUT); 
  myIRserial.attach(rxpin, txpin);      //used for infrared sensor
  Serial.begin(9600);
}

//re-orient the robot facing 1 each time
void loop()
{
  //opens hand
  dropBall();
  scan();
  int  val = scan();

  
 
  if (val == 48){ //if ASCII = 0
    turnRight();
    lineFollowerTripleBlack();
    gripBall();
    fulturnLeft();
    lineFollowerTripleBlack();
    miniDrive();
    turnLeft();
    lineFollowerTripleBlack();
    dropBall();
    fulturnLeft();
    lineFollowerTripleBlack();
    miniDrive();
  }

  if (val == 50){ //if ASCII = 2 
    turnLeft();
    lineFollowerTripleBlack();
    gripBall();
    fulturnLeft();
    lineFollowerTripleBlack();
    miniDrive();
    turnRight();
    lineFollowerTripleBlack();
    dropBall();
    fulturnLeft();
    lineFollowerTripleBlack();
    miniDrive();
  }

  if (val == 49){ //if ASCII = 1
    lineFollowerDistanceStop();
    gripBall();
    fulturnLeft();
    lineFollowerTripleBlack();
    miniDrive();
    lineFollowerTripleBlack();
    dropBall();
    fulturnLeft();
    lineFollowerTripleBlack();
    miniDrive();
  }
}

int scan(){
  while(true){
    int val = myIRserial.receive(200);
    if (val !=48 || val!=49 || val !=50){
    shoulder.write(0);
    int val = myIRserial.receive(200);
    delay(800);
    shoulder.write(90);
    int val = myIRserial.receive(200);
    delay(800);
    shoulder.write(180);
    int val = myIRserial.receive(200);
    delay(800);
    }
  else{
    return val;
    break;
  }
  }

}

//follows line until it all three sensors see black, then robot stops 
void lineFollowerTripleBlack(){ 
  while (true){ 
  
    //move forward
    if(analogRead(LS)>whiteTapeMin && analogRead(RS)>whiteTapeMin && analogRead(MS)<blackTapeMax) 
    {
      digitalWrite(4, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(leftWheel, Speed);
      analogWrite(rightWheel, Speed);
    }
    //turn right
    if(analogRead(LS)>whiteTapeMin && analogRead(RS)<blackTapeMax) // 
    {
      digitalWrite(4, HIGH);
      digitalWrite(7, LOW);
      analogWrite(leftWheel, insideWheelSpeed);
      analogWrite(rightWheel, turnSpeed);
      delay(50);
    }
    //turn left
    if(analogRead(LS)<blackTapeMax && analogRead(RS)>whiteTapeMin)
    {
      digitalWrite(4, LOW); //left
      digitalWrite(7, HIGH);
      analogWrite(leftWheel, turnSpeed);
      analogWrite(rightWheel, insideWheelSpeed);
      delay(50);
    }
    
    //stop, breaks out of line follower function as well
    if(analogRead(LS)<blackTapeMax && analogRead(RS)<blackTapeMax && analogRead(MS)<blackTapeMax) 
    {
      digitalWrite(4, LOW);
      digitalWrite(7, LOW);
      analogWrite(leftWheel, 0);
      analogWrite(rightWheel, 0);
      break;
    }
    else{ //move forward if an error ocurs and the middle sensor goes off
     digitalWrite(4, HIGH);
     digitalWrite(7, HIGH);
     analogWrite(leftWheel, Speed);
     analogWrite(rightWheel, Speed);
    }
  }
  
}



//follows line until it distance sensor reads greater than 480 (hits the wall), only used for 1
void lineFollowerDistanceStop(){
  while (true){ 
      if (analogRead(range)<450){
      //move forward
        if(analogRead(LS)>whiteTapeMin && analogRead(RS)>whiteTapeMin && analogRead(MS)<blackTapeMax) 
        {
          digitalWrite(4, HIGH);
          digitalWrite(7, HIGH);
          analogWrite(leftWheel, Speed);
          analogWrite(rightWheel, Speed);
          delay(50);
        }
        //turn right
        if(analogRead(LS)>whiteTapeMin && analogRead(RS)<blackTapeMax) // 
        {
          digitalWrite(4, LOW);
          digitalWrite(7, HIGH);
          analogWrite(leftWheel, insideWheelSpeed);
          analogWrite(rightWheel, turnSpeed);
          delay(50);
        }
        //turn left
        if(analogRead(LS)<blackTapeMax && analogRead(RS)>whiteTapeMin)
        {
          digitalWrite(4, HIGH); //left
          digitalWrite(7, LOW);
          analogWrite(leftWheel, turnSpeed);
          analogWrite(rightWheel, insideWheelSpeed);
          delay(50);
        }
        else //move forward if an error ocurs and the middle sensor goes off
        { 
         digitalWrite(4, HIGH);
         digitalWrite(7, HIGH);
         analogWrite(leftWheel, Speed);
         analogWrite(rightWheel, Speed);
         delay(50);
        }
      }
      else
      {
        while(true){
               
          //drive slowly up to the wall until it reaches
          digitalWrite(4, HIGH);
          digitalWrite(7, HIGH);
          analogWrite(leftWheel, 75);
          analogWrite(rightWheel, 75);
        
          if (analogRead(range)>480){ //once distance senor hits the wall
             hand.write(0); //opens hand
             gripBall();
             break;
          }
        }
      }
    }
  }

//drivesStraight
void driveStraight(){
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  analogWrite(leftWheel, Speed);
  analogWrite(rightWheel, Speed);
}

//turn 180 degrees, should be calibrated properly
void fulturnLeft(){ 
   digitalWrite(4, HIGH);
   digitalWrite(7, LOW);
   analogWrite(leftWheel, 180);
   analogWrite(rightWheel, 180);
   delay(1050);
}

//turn 90 degrees right
void turnRight(){
   digitalWrite(4, HIGH);
   digitalWrite(7, LOW);
   analogWrite(leftWheel, 180);
   analogWrite(rightWheel, 180);
   delay(450);
}

void turnLeft(){
   digitalWrite(4, LOW);
   digitalWrite(7, HIGH);
   analogWrite(leftWheel, 180);
   analogWrite(rightWheel, 180);
   delay(450);
}



//stop the robot, wait 2 sec
void stopBreak(){
   digitalWrite(4, HIGH);
   digitalWrite(7, LOW);
   analogWrite(leftWheel, 0);
   analogWrite(rightWheel, 0);
}

//drive a short distance
void miniDrive(){
   digitalWrite(4, HIGH);
   digitalWrite(7, HIGH);
   analogWrite(leftWheel, 180);
   analogWrite(rightWheel, 180);
   delay(350);
}

//turn a short distance
void miniTurn(){
   digitalWrite(4, HIGH);
   digitalWrite(7, HIGH);
   analogWrite(leftWheel, 180);
   analogWrite(rightWheel, 180);
   delay(100);
}

//not done, this just stops gripping when it has it 
void gripBall(){
  hand.write(0); //opens hand
  delay(1000);
  while (true) {
    if (analogRead(touch) >= 250 || hand.read() == 180){
      hand.write(hand.read());
      delay(100);
      break;
      }
    else{
      hand.write(hand.read()+1.5);
      delay(100);
    }
  }
}

//drops ball
void dropBall(){
    hand.write(0); //opens hand
}

//Recieves a serial input: 0, 1, or 2
int readASCII(){
  int val = myIRserial.receive(200);
  return val;
}



