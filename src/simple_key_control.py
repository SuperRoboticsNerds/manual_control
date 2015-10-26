#!/usr/bin/env python

import curses
import rospy
from geometry_msgs.msg import Twist

to_increase = 0.1

pub = rospy.Publisher("motor_controller/twist",Twist,queue_size=10)
rospy.init_node("keyboard_controller")
screen = curses.initscr()
curses.cbreak()
screen.keypad(1)

key = ''
screen.addstr(0, 0, "Press Space to stop")
screen.addstr(1, 0, "Press arrow keys to control")
screen.addstr(2, 0, "Press q to quit")

message = Twist()
rate = rospy.Rate(20)
while key != ord('q'):  # press <Q> to exit the program
    
    key = screen.getch()  # get the key
    

    if key == curses.KEY_UP:
        screen.addstr(4, 0, "---Up---")
        message.linear.x += to_increase
    elif key == curses.KEY_DOWN:
        screen.addstr(4, 0, "--Down--")
        message.linear.x -= to_increase
    elif key == curses.KEY_LEFT:
        screen.addstr(4, 0, "--Left--")
        message.angular.z += to_increase
    elif key == curses.KEY_RIGHT:
        screen.addstr(4, 0, "--Right-")
        message.angular.z -= to_increase
    elif key == ord(' '):
    	screen.addstr(4, 0, "--Space-")
    	message.angular.z = 0
    	message.linear.x = 0

    screen.addstr(6, 0, "v: " + str(round(message.linear.x,2)))
    screen.addstr(7, 0, "w: " + str(round(message.angular.z,2)))

    pub.publish(message)
    rate.sleep()

message.angular.z = 0
message.linear.x = 0
pub.publish(message)

curses.endwin()
