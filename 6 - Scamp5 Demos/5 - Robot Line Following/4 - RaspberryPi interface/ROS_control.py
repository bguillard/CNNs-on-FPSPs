#!/usr/bin/env python

from __future__ import print_function
import rospy
from geometry_msgs.msg import Twist
import datetime
import sys
import time

FORWARD_TIME = 0.8

RIGHT_FORWARD_TIME = 0.8
RIGHT_TURN_TIME = 0.8

LEFT_FORWARD_TIME = 0.8
LEFT_TURN_TIME = 0.8

msg = """
Reading from stdin, and Publishing to Twist!
---------------------------
Moving around:
   1 (left)
   2 (forward)
   3 (right)

CTRL-C or 'quit' to quit
"""

LEFT    = 1
FORWARD = 2
RIGHT   = 3

moveBindings = {
        LEFT:   (0,0,0,1),
        FORWARD:(1,0,0,0),
        RIGHT:  (0,0,0,-1)
    }

dir_to_str = {  LEFT:   str(LEFT),
                FORWARD:str(FORWARD),
                RIGHT:  str(RIGHT)}

dir_to_readable = { LEFT:   'LEFT   ',
                    FORWARD:'FORWARD',
                    RIGHT:  'RIGHT  '}

speed = 0.5
turn = 1

def send_stop():
    twist = Twist()
    twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
    twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
    pub.publish(twist)

    return 0


msg1 = 'SCAMP5 camera instructing to go '
msg2 = ', passing that instruction to ROS.'

def move(direction):
    x = moveBindings[direction][0]
    y = moveBindings[direction][1]
    z = moveBindings[direction][2]
    th = moveBindings[direction][3]

    twist = Twist()
    twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
    twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn

    pub.publish(twist)

def go_straight():
    sys.stdout.write('\r' + msg1 + dir_to_readable[FORWARD] + msg2)
    sys.stdout.flush()

    move(FORWARD)
    time.sleep(FORWARD_TIME)
    send_stop()
    time.sleep(0.5)

def turn_right():
    sys.stdout.write('\r' + msg1 + dir_to_readable[RIGHT] + msg2)
    sys.stdout.flush()

    move(FORWARD)
    time.sleep(RIGHT_FORWARD_TIME)
    send_stop()
    time.sleep(0.2)

    move(RIGHT)
    time.sleep(RIGHT_TURN_TIME)
    send_stop()
    time.sleep(0.5)

def turn_left():
    sys.stdout.write('\r' + msg1 + dir_to_readable[LEFT] + msg2)
    sys.stdout.flush()

    move(FORWARD)
    time.sleep(LEFT_FORWARD_TIME)
    send_stop()
    time.sleep(0.2)

    move(LEFT)
    time.sleep(LEFT_TURN_TIME)
    send_stop()
    time.sleep(0.5)


if __name__=="__main__":
    pub = rospy.Publisher('cmd_vel', Twist, queue_size = 1)
    rospy.init_node('teleop_twist_keyboard')

    speed = rospy.get_param("~speed", 0.5)
    turn = rospy.get_param("~turn", 1.0)

    start_time = datetime.datetime.now()
    stop_time = datetime.datetime.now()
    try:
        print(msg)
        while True:
            line = sys.stdin.readline()
            start_time = datetime.datetime.now()
            time_diff = (start_time - stop_time).microseconds
            #print(time_diff)
            if time_diff > 20000:
                if line.startswith(dir_to_str[LEFT]):
                    turn_left()
                elif line.startswith(dir_to_str[FORWARD]):
                    go_straight()
                elif line.startswith(dir_to_str[RIGHT]):
                    turn_right()
            if line.startswith('quit'):
                break
            stop_time = datetime.datetime.now()


    except Exception as e:
        print(e)

    finally:
        # Send STOP message
        send_stop()
