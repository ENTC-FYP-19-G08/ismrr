from enum import Enum

import rclpy
import serial
import time
import sys

class GestureType():
    AYUBOWAN = 0
    HANDSHAKE = 1
    SHOW_LEFT = 2
    SHOW_RIGHT = 3    

class SMRRGestures():

    guesture_map={
    GestureType.AYUBOWAN:b'\x00', #Ayubowan
    GestureType.HANDSHAKE:b'\x01', #Hand Shake  
    GestureType.SHOW_LEFT:b'\x02', #Left
    GestureType.SHOW_RIGHT:b'\x03'  #Right    
    }

    def __init__(self):
        pass

    def do_gesture(self, gesture_type):
        ser = serial.Serial('/dev/pololu_serial', 9600)
        time.sleep(1)
        ser.write(b'\xa7')
        ser.write(self.guesture_map[gesture_type])
        time.sleep(2)
        ser.close()

if __name__ == '__main__':
    if len(sys.argv)>1:
        gesture_type=int(sys.argv[1])
        obj = SMRRGestures()        
        obj.do_gesture(gesture_type)
        print('doing',gesture_type)
        time.sleep(2)
        print('done')
    else: print("no args")
