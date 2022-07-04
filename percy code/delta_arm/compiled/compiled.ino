#include <Arduino.h>
#include "ServoEasing.hpp"
#include <ros.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Point.h>


#include <NewPing.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>


//Ultrasound Definitions
#define SONAR_NUM 8      // Number of sensors.
#define MAX_DISTANCE 10000 // Maximum distance (in cm) to ping.
int a = 0;
#define SensorPin A15 //soil sensor
int DOpin = 48; //soil sensor

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(47, 46, MAX_DISTANCE),  
  NewPing(45, 44, MAX_DISTANCE), 
  NewPing(43, 42, MAX_DISTANCE),
  NewPing(41, 40, MAX_DISTANCE),
  NewPing(39, 38, MAX_DISTANCE),
  NewPing(37, 36, MAX_DISTANCE),
  NewPing(35, 34, MAX_DISTANCE),
  NewPing(33, 32, MAX_DISTANCE),
};

// Voltage Monitoring Definitions
#define batVpin A2              // Analogue pin that voltage sensing is connected to
const double VCC = 5.0;          // set the supply voltage
int Vsamples = 15;              // Set number of samples

// Current Monitoring Definitions
#define batIpin A3                  // Analogue pin that current sensor Vout is connected to
double I_sensitivity = 20.0*0.001;    // Set sensitivity of current sensor in V/A  -  Find in datasheet (model currently used in ACS758LCB-100B)
double I_quiescentVoltage = VCC*0.5;    // Set quiescent voltage of sensor (found in datasheet)
int Isamples = 20;                       // Set number of samples

// Kill switch definitions
#define killSwitchPin 8
#define OverCurrentLED 7
#define UnderVoltageLED 6

// General variable definitions
float I;              // Variable to store the measured current
float V;              // Variable to store the measured voltage

// Ros code for battery
ros::NodeHandle  nh;
geometry_msgs::Point point_msg;
ros::Publisher IV_Publisher("battery_status", &point_msg);

// ROS for ultrasounds
std_msgs::Bool bool_msg_block;
std_msgs::String help_msg;
std_msgs::Float32 soil_msg;
geometry_msgs::Twist twist_msg;
ros::Publisher block("block", &bool_msg_block);
ros::Publisher cmd("/percy_velocity_controller/cmd_vel", &twist_msg);
ros::Publisher help("help", &help_msg);
ros::Publisher soil("soil", &soil_msg);

//Servo setup
ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;

#define START_DEGREE_VALUE 50 // The degree value written to the servo at time of attach.

#define SERVO1_PIN 9
#define SERVO2_PIN 10
#define SERVO3_PIN 11
#define SPEED_IN_PIN A0
#define MODE_ANALOG_INPUT_PIN A1
int starttime_ultra; 
int endtime_ultra;
int starttime; 
int endtime;

// robot geometry
const float f = 25.0; // base
const float e = 11.0;  // end effector
const float rf = 20.0;
const float re = 35.0;

// trigonometric constants
const float sqrt3 = sqrt(3.0);
const float pi = 3.141592653; // PI
const float sin120 = sqrt3 / 2.0;
const float cos120 = -0.5;
const float tan60 = sqrt3;
const float sin30 = 0.5;
const float tan30 = 1 / sqrt3;

float theta1 = 0;
float theta2 = 0;
float theta3 = 0;


std_msgs::Bool bool_msg;
ros::Publisher exit_status("exit_status", &bool_msg);
void pointdata( const geometry_msgs::Point& points){

    float x = points.x;
    float y = points.y;
    float z = points.z;
    //double x = Double.parseDouble(first);

    delta_calcInverse(x, y, z, theta1, theta2, theta3);
    char theta1char[5];
    
    char theta2char[5];
    
    char theta3char[5];
    
    theta1 = theta1 + 50;
    theta2 = theta2 + 50;
    theta3 = theta3 + 50;
    move_servos(theta1, theta2, theta3);

    dtostrf(theta1, 6, 2, theta1char);
    nh.loginfo(theta1char);

    dtostrf(theta2, 6, 2, theta2char);
    nh.loginfo(theta2char); 
 
    dtostrf(theta3, 6, 2, theta3char);
    nh.loginfo(theta3char);
    char xchar[5];
    dtostrf(x, 6, 2, xchar);
    nh.loginfo(xchar);
    char ychar[5];
    dtostrf(y, 6, 2, ychar);
    nh.loginfo(ychar);  
    char zchar[5];
    dtostrf(z, 6, 2, zchar);
    nh.loginfo(zchar);
    nh.logerror("Moved");
    starttime = millis();
    endtime = starttime;
    while((endtime - starttime) < 5000){
      endtime = millis();
    }
    
    delta_calcInverse(0, 0, -20.43, theta1, theta2, theta3);
    theta1 = theta1 + 50;
    theta2 = theta2 + 50;
    theta3 = theta3 + 50;
    move_servos(theta1, theta2, theta3);


  delay(1000);
  bool_msg.data = true;
  exit_status.publish( &bool_msg );
}

ros::Subscriber <geometry_msgs::Point> sub("points", &pointdata);
void setup()
{
  // put your setup code here, to run once:

  //initialise publisher and subscriber nodes
  nh.initNode();
  nh.advertise(exit_status);
  nh.subscribe(sub);
  nh.advertise(block);
  nh.advertise(cmd);
  nh.advertise(soil);
  nh.advertise(IV_Publisher);
  nh.advertise(help);
  //servo.attach(servoPin);
  Serial.begin(115200);
  pinMode(killSwitchPin, OUTPUT);             // sets the digital kill pin as output
  pinMode(OverCurrentLED, OUTPUT);            // sets the over current LED pin as output
  pinMode(UnderVoltageLED, OUTPUT);           // sets the under voltage LED pin as output
  digitalWrite(killSwitchPin, HIGH);          // Turn MOSFET on
  digitalWrite(OverCurrentLED, LOW);          // Turn over current LED off
  digitalWrite(UnderVoltageLED, LOW);         // Turn under voltage LED 
  Servo1.attach(SERVO1_PIN, START_DEGREE_VALUE);
  Servo2.attach(SERVO2_PIN, START_DEGREE_VALUE);

  if (Servo3.attach(SERVO3_PIN, START_DEGREE_VALUE) == INVALID_SERVO)
  {
    nh.logerror("Invalid serv3");
  }

  delay(500);
}
double batteryCurrent(){
  analogRead(batIpin);
  double V_Read=0;
  double current=0;

  for (int i=0; i<Isamples; i++){
    V_Read+=-(((analogRead(batIpin)+5)*(VCC/1023.0))-I_quiescentVoltage);      //4 is a number to make the current zero at 0 current
  }
  V_Read/=Isamples;

  current = V_Read/I_sensitivity;
  if(current>55.0){KillRobot(1);}
  
  return current;
}

double batteryVoltage(){
  double batV = 0;
  analogRead(batVpin);
  for (int i=0; i<Vsamples; i++){
    batV+=analogRead(batVpin)*(VCC/1023.0)*4.3054 + 0.105;       // Convert a to d value to measured value then scale analogue voltage to fit potential divider circuit (33k and 10k: (10+33)/10 = 4.3)
                                                                  // Calibration resulted in an adjustment og 4.3 to 4.3054 and a constant shift of 0.105
  }
  batV /= Vsamples;
  
  if(batV<13.6 && batV>5){KillRobot(2);}
                                          
  return batV;
}

void KillRobot(int killcode){
  digitalWrite(killSwitchPin, LOW);                       //Turn off all circuits by switching off MOSFET
  if(killcode == 1){
    //Serial.println("Overcurrent protection activated");
    digitalWrite(OverCurrentLED, HIGH);
  }
  else if(killcode = 2){
    //Serial.println("Undervoltage protection activated");
    digitalWrite(UnderVoltageLED, HIGH);
  }
}
int time_now = 0;
int period = 1000;
void loop()
{
  if(millis() > time_now + period){
        time_now = millis();
        I=batteryCurrent();
        V=batteryVoltage();
        point_msg.x = I;
        point_msg.y = V;
      
        IV_Publisher.publish(&point_msg);
  }

  //Ultrasound
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    starttime_ultra = millis();
    endtime_ultra = starttime_ultra;
    while((endtime_ultra - starttime_ultra) < 50){
      endtime_ultra = millis();
    } // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  }
    if (0<sonar[0].ping_cm()<=20|| 0<sonar[1].ping_cm()<=20|| 0<sonar[2].ping_cm()<=20|| 0<sonar[3].ping_cm()<=20|| 0<sonar[4].ping_cm()<=20|| 0<sonar[5].ping_cm()<=20|| 0<sonar[6].ping_cm()<=20|| 0<sonar[7].ping_cm()<=20){
      a += 1;
    }
    if ( a >= 3 ){
      
      twist_msg.linear.x = 0;
    twist_msg.angular.z = 0;
    bool_msg_block.data = true;
    help_msg.data = "HELP ME AAHHHH!!!!";
    while(true){
      cmd.publish(&twist_msg);
      block.publish(&bool_msg_block);
      help.publish(&help_msg);
    }
    }
    if ((sonar[0].ping_cm()>20 ||sonar[0].ping_cm()==0) && (sonar[1].ping_cm()>20 ||sonar[1].ping_cm()==0) && (sonar[2].ping_cm()>20 ||sonar[2].ping_cm()==0) && (sonar[3].ping_cm()>20 ||sonar[3].ping_cm()==0) && (sonar[4].ping_cm()>20 ||sonar[4].ping_cm()==0) && (sonar[5].ping_cm()>20 ||sonar[5].ping_cm()==0) && (sonar[6].ping_cm()>20 ||sonar[6].ping_cm()==0) && (sonar[7].ping_cm()>20 ||sonar[7].ping_cm()==0) ){
      a = 0;
    }
  int DO = digitalRead(DOpin);
  int sensorValue = analogRead(SensorPin);
  float hum = (sensorValue * 100)/1024;
  soil_msg.data = hum;
  soil.publish(&soil_msg);

  
  nh.spinOnce();
  delay(1);
}

// inverse kinematics
// helper functions, calculates angle theta1 (for YZ-pane)
int delta_calcAngleYZ(float x0, float y0, float z0, float &theta)
{
  float y1 = -0.5 * 0.57735 * f; // f/2 * tg 30
  y0 -= 0.5 * 0.57735 * e;       // shift center to edge

  // z = a + b*y
  float a = (x0 * x0 + y0 * y0 + z0 * z0 + rf * rf - re * re - y1 * y1) / (2 * z0);
  float b = (y1 - y0) / z0;

  // discriminant
  float d = -(a + b * y1) * (a + b * y1) + rf * (b * b * rf + rf);
  if (d < 0)
    return -1;                                     // non-existing point
  float yj = (y1 - a * b - sqrt(d)) / (b * b + 1); // choosing outer point
  float zj = a + b * yj;
  theta = 180.0 * atan(-zj / (y1 - yj)) / pi + ((yj > y1) ? 180.0 : 0.0);
  return 0;
}

// inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
// returned status: 0=OK, -1=non-existing position

int delta_calcInverse(float x0, float y0, float z0, float &theta1, float &theta2, float &theta3)
{
  theta1 = theta2 = theta3 = 0;
  int status = delta_calcAngleYZ(x0, y0, z0, theta1);
  if (status == 0)
    status = delta_calcAngleYZ(x0 * cos120 + y0 * sin120, y0 * cos120 - x0 * sin120, z0, theta2); // rotate coords to +120 deg
  if (status == 0)
    status = delta_calcAngleYZ(x0 * cos120 - y0 * sin120, y0 * cos120 + x0 * sin120, z0, theta3); // rotate coords to -120 deg
  return status;
}

void move_servos(float t1, float t2, float t3)
{
  // Move three servos synchronously without interrupt handler
  // this speed is changed for the first 2 servos below by synchronizing to the longest duration
  setSpeedForAllServos(20);

  ServoEasing::ServoEasingArray[0]->setEaseTo(t1); // This servo uses effectively 10 degrees per second, since it is synchronized to Servo3
  ServoEasing::ServoEasingArray[1]->setEaseTo(t2); // "ServoEasing::ServoEasingArray[1]->" can be used instead of "Servo2."
  Servo3.setEaseTo(t3);                            // This servo has the longest distance -> it uses 20 degrees per second
  synchronizeAllServosAndStartInterrupt(false);    // Do not start interrupt
  nh.loginfo("Moving");
  do
  {
    // here you can call your own program
    delay(REFRESH_INTERVAL_MILLIS); // optional 20ms delay
  } while (!updateAllServos());
}
