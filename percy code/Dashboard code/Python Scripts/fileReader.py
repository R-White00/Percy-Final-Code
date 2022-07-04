#!/usr/bin/env python
import rospy
import csv
from std_msgs.msg import String
from geometry_msgs.msg import Twist

global msg
def main():
    
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    rospy.init_node('remoteCommands', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():

        with open('/home/reece/Documents/Dashboard code/Python Scripts/commands.csv', 'r') as file:
            reader = csv.reader(file)
            for row in reader:
                print(row[0])
                if row[0] == 1:

                    msg.linear.x = 1
                    pub.publish(msg)
        rate.sleep()


if __name__ == "__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass