from enum import Enum

import rclpy
import serial
import time

class GestureType(Enum):
    AYUBOWAN = 0
    SHOW_LEFT = 1
    SHOW_RIGHT = 2
    HANDSHAKE = 3

class SMRRGestures():

    guesture_map={
    GestureType.AYUBOWAN:b'\x00', #Ayubowan  
    GestureType.SHOW_LEFT:b'\x02', #Left
    GestureType.SHOW_RIGHT:b'\x03',  #Right
    GestureType.HANDSHAKE:b'\x01' #Hand Shake  
    }

    def __init__(self):
        self.ser = serial.Serial('/dev/ttyACM0', 9600)

    def do_gesture(self, gesture_type):
        self.ser.write(b'\xa7')
        self.ser.write(self.guesture_map[gesture_type])

if __name__ == '__main__':
    obj = SMRRGestures()
    time.sleep(1)
    obj.do_gesture(GestureType.SHOW_LEFT)
    print('done')
    time.sleep(1000)