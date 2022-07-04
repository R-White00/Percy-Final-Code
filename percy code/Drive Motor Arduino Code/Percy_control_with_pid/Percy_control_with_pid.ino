#include <PID_v1.h>

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>



ros::NodeHandle  nh;
uint8_t dirPin1 = 22;
uint8_t dirPin2 = 23;
uint8_t motor1Pin = 2;
uint8_t motor2Pin = 4;


//encoder Pins array
uint8_t encoderAPins[4];
uint8_t encoderBPins[4];
uint8_t pwmPins[4];
uint8_t dirPins[4];
bool dirMod[4];
//bool wheelDir[] = {false, false,false,false};
bool wheelDir[4];


long prevT_i[4];
volatile float velocity_i[4];

double Kp = 0.1, Ki = 25, Kd = 0.01;

double target, Input, Output;



double motorSpeed[4]; //desired speed
double motorVelocity[4]; //actual speed of motor
double motorCMD[4]; //pwm signal from pid


PID myPID1(&motorVelocity[0], &motorCMD[0], &motorSpeed[0], Kp, Ki, Kd, DIRECT);
PID myPID2(&motorVelocity[1], &motorCMD[1], &motorSpeed[1], Kp, Ki, Kd, DIRECT);
PID myPID3(&motorVelocity[2], &motorCMD[2], &motorSpeed[2], Kp, Ki, Kd, DIRECT);
PID myPID4(&motorVelocity[3], &motorCMD[3], &motorSpeed[3], Kp, Ki, Kd, DIRECT);

float v2Filt = 0;
float v2Prev[4];


double vel1;
double vel2;
void wheelCommands( const std_msgs::Float64MultiArray& WheelCommand){
//  Serial.println("Recieved Data");  
 // Serial.println("Joint2: ");  
//  Serial.println(VelCommand.data[0]); 
    vel2=WheelCommand.data[1];
  vel1=WheelCommand.data[0];


//  Serial.println("Joint2: ");  // blink the led
//    Serial.println(VelCommand.data[1]);   // blink the led
//Serial.println("Joint3: ");  
//  Serial.println(VelCommand.data[2]);   // blink the led
//Serial.println("Joint4: ");  
//  Serial.println(VelCommand.data[3]);   // blink the led


}
void velocityCommands( const std_msgs::Float64MultiArray& VelCommand){
//  Serial.println("Recieved Data");  
 // Serial.println("Joint2: ");  
//  Serial.println(VelCommand.data[0]); 
   vel2=VelCommand.data[0];
  vel1=VelCommand.data[3];

//  Serial.println("Joint2: ");  // blink the led
//    Serial.println(VelCommand.data[1]);   // blink the led
//Serial.println("Joint3: ");  
//  Serial.println(VelCommand.data[2]);   // blink the led
//Serial.println("Joint4: ");  
//  Serial.println(VelCommand.data[3]);   // blink the led


}
ros::Subscriber<std_msgs::Float64MultiArray> sub("VelocityCommands", velocityCommands );
//ros::Subscriber<std_msgs::Float64MultiArray> sub("WheelCommands", wheelCommands );

void setup() {
  // put your setup code here, to run once:

  Serial.begin(57600);
//  pinMode(13, OUTPUT);
//  pinMode(dirPin1, OUTPUT);
//  pinMode(motor1Pin, OUTPUT);
//    pinMode(dirPin2, OUTPUT);
//  pinMode(motor2Pin, OUTPUT);
  nh.initNode();
  //nh.advertise(chatter);
  nh.subscribe(sub);

//Motor 1
//Rear Left
  encoderAPins[1] = 3;
  encoderBPins[1] = 24;
  pwmPins[1] = 8;
  dirPins[1] = 26;
 dirMod[1] = false;

//Motor 2
//front left
    encoderAPins[2] = 18;
    encoderBPins[2] = 23;
    pwmPins[2] = 10;
   dirPins[2] = 27;
    dirMod[2] = false;


//Motor 3
//Rear Right
    encoderAPins[3] = 2;
    encoderBPins[3] = 25;
    pwmPins[3] = 7;
    dirPins[3] = 28;
    dirMod[3] = true;


//Motor 4
//front right
     encoderAPins[0] = 19;
    encoderBPins[0] = 22 ;
    pwmPins[0] = 9;
    dirPins[0] = 29;
   dirMod[0] = true;


  for (int i = 0; i < 4; i++) {
    pinMode(encoderAPins[i], INPUT);
    pinMode(encoderAPins[i], INPUT);
    pinMode(pwmPins[i], OUTPUT);
    pinMode(dirPins[i], OUTPUT);
  }



  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
  myPID3.SetMode(AUTOMATIC);
  myPID4.SetMode(AUTOMATIC);

  attachInterrupt(digitalPinToInterrupt(encoderAPins[0]),
                  encoderPin1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderAPins[1]),
                  encoderPin2, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderAPins[2]),
                  encoderPin3, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderAPins[3]),
                  encoderPin4, RISING);



}

//function in attach interupt do not allow for parameters
void encoderPin1() {
  readEncoder(0);
}
void encoderPin2() {
  readEncoder(1);
}
void encoderPin3() {
  readEncoder(2);
}
void encoderPin4() {
  readEncoder(3);
}




void loop() {
  // put your main code here, to run repeatedly:
  float velocity2[4];
  noInterrupts();
  //loop that sets all velcoity2 elements to velocity_i elements
  for (int i = 0; i < 4; i++) {
    velocity2[i] = velocity_i[i];
  }
  interrupts(); // turn interrupts back on


  //function that runs low pass filter on all elements in velocity2 array
  float v2;
  for (int i = 0; i < 4; i++) {
//    motorSpeed[i] = 90 * (sin(micros() / 1e6) > 0); //NOTE FOR TESTING TO SET THE SAME SPEED

    v2 = velocity2[i] / 420.0 * 60.0;
//    if(i == 1 ){
//          motorVelocity[i] = (0.854 * motorVelocity[i] + 0.0728 * v2 + 0.0728 * v2Prev[i]);
//
//    }else{
//    
//    }

    motorVelocity[i] = 1*(0.854 * motorVelocity[i] + 0.0728 * v2 + 0.0728 * v2Prev[i]);
    v2Prev[i] = v2;
  }

//   motorSpeed[0] = 90*(sin(micros()/1e6)>0); //NOTE FOR TESTING TO SET THE SAME SPEED
//    motorSpeed[1] =45*(sin(micros()/1e6)>0); //NOTE FOR TESTING TO SET THE SAME SPEED
//   motorSpeed[2] = 90*(sin(micros()/1e6)>0); //NOTE FOR TESTING TO SET THE SAME SPEED
//    motorSpeed[3] = 45*(sin(micros()/1e6)>0); //NOTE FOR TESTING TO SET THE SAME SPEED
//
 if(vel1 < 0){
  wheelDir[0] = true;
  wheelDir[3] = true;
 }else{
    wheelDir[0] = false;
  wheelDir[3] = false;
 }

 if(vel2 < 0){
  wheelDir[1] = true;
  wheelDir[2] = true;
 }else{
    wheelDir[1] = false;
  wheelDir[2] = false;
 }
// 
   motorSpeed[2] = abs(vel1*(60/(2*PI))); //NOTE FOR TESTING TO SET THE SAME SPEED
    motorSpeed[0] = abs(vel2*(60/(2*PI))); //NOTE FOR TESTING TO SET THE SAME SPEED
   motorSpeed[1] = abs(vel1*(60/(2*PI))); //NOTE FOR TESTING TO SET THE SAME SPEED
    motorSpeed[3] = abs(vel2*(60/(2*PI))); //NOTE FOR TESTING TO SET THE SAME SPEED

//  Serial.print("motorSpeed[0]: ");
// Serial.println(motorSpeed[0]);
//
//   Serial.print("motorCMD[0]: ");
// Serial.println(motorCMD[0]);

  myPID1.Compute();
  myPID2.Compute();
  myPID3.Compute();
  myPID4.Compute();



  for (int i = 0; i < 4; i++) {
    if(wheelDir[i] == false){
      if(dirMod[i] == false){
       digitalWrite(dirPins[i], LOW);
      }else{
        digitalWrite(dirPins[i], HIGH);
      }
    }else{
            if(dirMod[i] == false){
       digitalWrite(dirPins[i], HIGH);
      }else{
        digitalWrite(dirPins[i], LOW);
      }

    }
    
           

    analogWrite(pwmPins[i], motorCMD[i]);
//    analogWrite(pwmPins[i], 255);

  }


//
//    analogWrite(pwmPins[0], 255);
//       analogWrite(pwmPins[1], 255);

  Serial.print(motorSpeed[1]);
  Serial.print(" ");
  Serial.print(motorVelocity[1]);
  Serial.print(" ");
  Serial.print(motorSpeed[2]);
  Serial.print(" ");
  Serial.print(motorVelocity[2]);
  Serial.print(" ");
  Serial.print(motorSpeed[3]);
  Serial.print(" ");
  Serial.print(motorVelocity[3]);
  Serial.print(" ");
  Serial.print(motorSpeed[0]);
  Serial.print(" ");
  Serial.print(motorVelocity[0]);

  Serial.print(motorSpeed[1]);
  Serial.print(" ");
  Serial.print(motorVelocity[1]);
//  Serial.print(" ");
//  Serial.print(motorSpeed[0]);
//  Serial.print(" ");
//  Serial.print(motorVelocity[2]);
//  Serial.print(" ");
//  Serial.print(motorVelocity[3]);
//  Serial.print(" ");
//  Serial.print(motorVelocity[0]);

  Serial.println();

  nh.spinOnce();
}


void readEncoder(int motorNum) {
  // Read encoder B when ENCA rises
  int b = digitalRead(encoderBPins[motorNum]);
  int increment = 0;
//  if (b > 0) {
//    // If B is high, increment forward
//    increment = 1;
//  }
//  else {
//    // Otherwise, increment backward
//    increment = -1;
//  }
increment = 1;


  // Compute velocity with method 2
  long currT = micros();
  float deltaT = ((float) (currT - prevT_i[motorNum])) / 1.0e6;
  velocity_i[motorNum] = increment / deltaT;
  prevT_i[motorNum] = currT;
}
