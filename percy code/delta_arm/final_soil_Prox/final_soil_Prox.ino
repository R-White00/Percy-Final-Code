// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
// ---------------------------------------------------------------------------

#include <NewPing.h>
#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#define SONAR_NUM 8      // Number of sensors.
#define MAX_DISTANCE 10000 // Maximum distance (in cm) to ping.
int a = 0;
//#define SensorPin A15 //soil sensor
//int DOpin = 48; //soil sensor

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

ros::NodeHandle  nh;
std_msgs::Bool bool_msg;
std_msgs::String help_msg;
std_msgs::Float32 soil_msg;
geometry_msgs::Twist twist_msg;
ros::Publisher block("block", &bool_msg);
ros::Publisher cmd("/percy_velocity_controller/cmd_vel", &twist_msg);
ros::Publisher help("help", &help_msg);
//ros::Publisher soil("soil", &soil_msg);
void setup() {
    nh.initNode();
    nh.advertise(block);
    nh.advertise(cmd);
    nh.advertise(help);
    Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  //pinMode(DOpin, INPUT);
    
}

void loop() { 
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  }
    if (0<sonar[0].ping_cm()<=20|| 0<sonar[1].ping_cm()<=20|| 0<sonar[2].ping_cm()<=20|| 0<sonar[3].ping_cm()<=20|| 0<sonar[4].ping_cm()<=20|| 0<sonar[5].ping_cm()<=20|| 0<sonar[6].ping_cm()<=20|| 0<sonar[7].ping_cm()<=20){
      a += 1;
    }
    if ( a >= 3 ){
      
      twist_msg.linear.x = 0;
    twist_msg.angular.z = 0;
    bool_msg.data = true;
    help_msg.data = "HELP ME AAHHHH!!!!";
    while(true){
      cmd.publish(&twist_msg);
      block.publish(&bool_msg);
      help.publish(&help_msg);
    }
    }
    if ((sonar[0].ping_cm()>20 ||sonar[0].ping_cm()==0) && (sonar[1].ping_cm()>20 ||sonar[1].ping_cm()==0) && (sonar[2].ping_cm()>20 ||sonar[2].ping_cm()==0) && (sonar[3].ping_cm()>20 ||sonar[3].ping_cm()==0) && (sonar[4].ping_cm()>20 ||sonar[4].ping_cm()==0) && (sonar[5].ping_cm()>20 ||sonar[5].ping_cm()==0) && (sonar[6].ping_cm()>20 ||sonar[6].ping_cm()==0) && (sonar[7].ping_cm()>20 ||sonar[7].ping_cm()==0) ){
      a = 0;
    }
  //int DO = digitalRead(DOpin);
  //int sensorValue = analogRead(SensorPin);
  //float hum = (sensorValue * 100)/1024;
  //soil_msg.data = hum;
  //soil.publish(&soil_msg);
  nh.spinOnce();
  delay(1);
}
