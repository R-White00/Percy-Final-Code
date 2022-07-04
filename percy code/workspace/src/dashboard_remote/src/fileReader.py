#!/usr/bin/env python
import rospy
import csv
from std_msgs.msg import String
from geometry_msgs.msg import Twist


msg = Twist()
def main():
    global msg

    rospy.init_node('remoteCommands', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():

        with open('/home/gdp31/Dashboard code/Python Scripts/commands.csv', 'r') as file:
            reader = csv.reader(file)
            for row in reader:
                print(row[0])
                if row[0] == '1':
                    #Forward
                    msg.linear.x = 1
                    pub.publish(msg)
                elif row[0] == '2':
                    #Reverse
                    msg.linear.x = -1
                    
                    pub.publish(msg)
                elif row[0] == '3':
                    #Left
                    msg.linear.x = 1
                    msg.angular.z = 1
                    pub.publish(msg)
                elif row[0] == '4':
                    #Right
                    msg.linear.x = 1
                    msg.angular.z = -1
                    pub.publish(msg)
                elif row[0] == '0':
                    #Nothing
                    msg.linear.x = 0
                    msg.angular.z = 0
                    pub.publish(msg)

                    
        rate.sleep()
    rospy.spin()


if __name__ == "__main__":
    pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    
    main()
    
    
