#! /usr/bin/env python

import rospy
import os
from std_msgs.msg import  String

def kill_callback(string):
    rospy.loginfo(string)
    os.system("rosnode kill /usb_cam_front")
		

if __name__ == '__main__':
    rospy.init_node('testreturner')
    killsub = rospy.Subscriber('/kill', String, kill_callback)
    rospy.spin()
