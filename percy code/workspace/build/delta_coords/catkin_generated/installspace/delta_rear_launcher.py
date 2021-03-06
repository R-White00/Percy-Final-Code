#!/usr/bin/env python2
import rospy
import roslaunch
from std_msgs.msg import  Bool, Int32

class Launcher:
        def start_callback(self, yesno):
                uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
                roslaunch.configure_logging(uuid)
                self.launch = roslaunch.parent.ROSLaunchParent(uuid, ["/home/gdp31/GDP31/workspace/src/delta_coords/launch/rear_delta_detect.launch"])
                self.launch.start()
                rospy.loginfo("started")

        def stop_callback(self, stop):
                self.launch.shutdown()

if __name__ == '__main__':
   rospy.init_node('delta_rear_launcher')
   Launcher = Launcher()
   start_sub = rospy.Subscriber('/launch_start', Bool, Launcher.start_callback, queue_size=1)
   stop_sub = rospy.Subscriber('/launch_stop', Bool, Launcher.stop_callback, queue_size=1)
   rospy.spin()