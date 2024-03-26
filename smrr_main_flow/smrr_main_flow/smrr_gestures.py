from enum import Enum

import rclpy
import serial

class GestureType(Enum):
    AYUBOWAN = 0
    SHOW_LEFT = 1
    SHOW_RIGHT = 2
    HANDSHAKE = 3

class SMRRGestures():

    guesture_map=[
    b'\x00', #Ayubowan
    b'\x05', #Left
    b'\x06'  #Right
    ]

    def __init__(self, node):
        self.node = node
        # self.ser = serial.Serial('/dev/ttyACM0', 9600)

    def do_gesture(self, gesture_type):
        self.ser.write(b'\xa7')
        self.ser.write(self.guesture_map[GestureType.AYUBOWAN])

