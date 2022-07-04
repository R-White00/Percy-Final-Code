#! /usr/bin/env python
import Queue
import roslaunch
import roslib; roslib.load_manifest("rosserial_arduino")
import rospy
from geometry_msgs.msg import Twist, Point
import actionlib
from delta_coords.msg import SendCoordsAction, SendCoordsResult
from delta_coords.srv import *
from std_msgs.msg import  Bool, Int32
import numpy as np
 

""" Idea of this is the server will recieve a coordinate from the side camera for a red plant.
It will then stop the Visual Line Follower node. Finds axial coord from the front cam.
3 coords will be sent to the delta arm arduino code which will reply with a completed message once it has carried out its service.
Then follower node can be restarted. A delay must be placed on the delta_coords restart to prevent a loop occuring. """


class DeltaMainServer:
   _result = SendCoordsResult()
   def __init__(self):
      self.server = actionlib.SimpleActionServer('send_coords', SendCoordsAction, self.execute, False)
      self.server.start()

      """ self.node = roslaunch.core.Node('follower_visual','follower_node.py')
      self.launch = roslaunch.scriptapi.ROSLaunch()
      self.launch.start()
      self.process = self.launch.launch(self.node) """

   def bool_callback(self, boolean): 
      self.boolean_val = boolean.data
      rospy.loginfo(self.boolean_val)

   def int_callback(self, intx): 
      self.int_x = intx
	#comes from the rear camera

   def block_visual_follower(self, condition):
      stopcmd = Bool()
      stopcmd = condition
      blocker_pub.publish(stopcmd)

   def execute(self, goal):
   # Do lots of awesome groundbreaking robot stuff here
      
      #Stop Visual Line Follower and Rover
      """ self.stopper = 1
      self.blocker()
      self.process.stop() """
      self.block_visual_follower(True)
      self.msg = Twist()
      self.msg.linear.x = 0
      self.msg.angular.z = 0
      cmd_pub.publish(self.msg)
	
     

      #convert coordinate ranges
      # convert rear camera from pixel 'z' to delta 'x'
      pixel_range = 640
      delta_range1 = 55
      x = (((self.int_x.data) * delta_range1) / pixel_range) - (delta_range1 / 2)

      #convert side horziontal from pixel 'x' to delta 'y'
      theta = 54.32 *np.pi /180
      delta_range2 = 50
      depth_dependent_Length = 2* np.tan(theta/2) * x
      y = ((goal.x * depth_dependent_Length) / pixel_range) - (delta_range2 / 2)

      #convert side vertical from pixel 'y' to delta 'z'
      delta_range3 = 54
      theta = 55.21 * np.pi/ 180
      depth_dependent_Length = 2 * np.tan(theta/2) * x
      z = (((goal.y * depth_dependent_Length) / 480) - (delta_range3 / 2))

      #Send coords to Delta Arm module
      rospy.sleep(1.)
      self.coord_point = Point()
      self.coord_point.x = x
      self.coord_point.y = y
      self.coord_point.z = z
      coord_pub.publish(self.coord_point)
      self.boolean_val = False

      #Recieve reply saying Delta Arm is finished
      """ rospy.sleep() until Subscribe receives message confirming Delta done
      maybe find another method for this actually """
      while(self.boolean_val != True):
         rospy.sleep(0.5)

      #Restart Visual Node

      self.block_visual_follower(False)
      """self.node = roslaunch.core.Node('follower_visual','follower_node.py')
      self.launch = roslaunch.scriptapi.ROSLaunch()
      self.launch.start()
      self.process = self.launch.launch(self.node)
      self.stopper = 0 """
      rospy.sleep(5.)
      self.server.set_succeeded()



if __name__ == '__main__':
   rospy.init_node('delta_main_server')
   server = DeltaMainServer()
   blocker_pub = rospy.Publisher('/block', Bool, queue_size=10)
   cmd_pub = rospy.Publisher('/percy_velocity_controller/cmd_vel',Twist, queue_size=10)
   coord_pub = rospy.Publisher('coords', Point, queue_size=10)
   arduino_sub = rospy.Subscriber('exit_status', Bool, server.bool_callback, queue_size=5)
   under_sub = rospy.Subscriber('delta_coord_z', Int32, server.int_callback,  queue_size = 10)
   
   rospy.spin()
