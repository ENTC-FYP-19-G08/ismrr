#!/usr/bin/env python3
import serial
import time
import rclpy
from rclpy.node import Node
from std_msgs.msg import String


l_speed = 0
r_speed = 0
state = 15

class ArduinoSerial(Node):
    def __init__(self):
        super().__init__(node_name='arduino_serial_node')
       
        self.ser = serial.Serial(port='/dev/arduino_serial',baudrate=115200, timeout=0.005)

        self.publisher =  self.create_publisher(String, '/wheel_encoders', 1)
        self.send_initialization_msg()

        self.subscription = self.create_subscription(String,'/wheel_velocity',  self.vel_callback,1)
        self.head_sub = self.create_subscription(String,'/head_angle',  self.head_angle_callback,1)
        self.timer = self.create_timer(0.02, self.timer_callback)

    def head_angle_callback(self,msg):
        angle = int(msg.data)
        l_speed = '0'
        r_speed = '0'
        serial_str  = f'{l_speed},{r_speed},{angle},0'  
        serial_str += "/n"
        try:
            x= serial_str.encode()
            self.ser.write(x)
        except:
            print("Writing Error!!!!    data: ",x )
            
        valueInString = -1 
        try:
            value= self.ser.readline()
            valueInString=str(value,'UTF-8')
        except:        
            print("Reading Error!!!!    data: ",valueInString )  

    def send_initialization_msg(self):

        wheel_odom_msg = String()
        wheel_odom_msg.data = "0 0 1"
        for i in range(10):
            self.publisher.publish(wheel_odom_msg)  #In case if you want restart the serial communication, the ROS sholud  know it.
            time.sleep(0.01)

    def timer_callback(self):
        global l_speed,r_speed, state
        angle = 0
        serial_str  = f'{l_speed},{r_speed},{angle},0' 
        serial_str += "/n"
        
        # if serial_str!=prev:
        if state<10:
            x = -1
            try:
                x= serial_str.encode()
                self.ser.write(x)
            except:
                print("Writing Error!!!!    data: ",x )

            valueInString = -1    
            try:
                value= self.ser.readline()
                valueInString=str(value,'UTF-8')

                wheel_odom_msg = String()
                data = valueInString[:-2] + " 0"
                # print(valueInString)
                # print(valueInString, wheel_odom_msg.data)
                # print(wheel_odom_msg.data)
                if  data != " 0":
                    # print(data)
                    data_list = data.split()
                    wheel_odom_msg.data = data_list[0] + " " + data_list[1] + " " + data_list[-1]
                    print(wheel_odom_msg.data)
                    self.publisher.publish(wheel_odom_msg)
            except:        
                print("Reading Error!!!!    data: ",valueInString )  

        if abs(float((l_speed)))<0.001 and abs(float(r_speed)) <0.001:
            state += 1
        else:
            state = 0


    def vel_callback(self,msg):
        global l_speed,r_speed
        l_speed, r_speed = msg.data.split()     
        # print(msg.data)
   


def main():
    rclpy.init()
    node = ArduinoSerial()
    rclpy.spin(node)
    node.destroy_node()

    rclpy.shutdown()

if __name__ == '__main__':
    main()