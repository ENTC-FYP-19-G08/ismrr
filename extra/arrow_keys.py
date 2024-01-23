import rclpy
from std_msgs.msg import String
import getch  # Make sure to install the getch module
import pygame
import keyboard
import sys
import time

l_speed = 0
r_speed = 0

import serial

ser = serial.Serial(port='/dev/arduino_serial',baudrate=115200,timeout=0.01)

prev=""

def publish_arrow_key():
    global l_speed, r_speed, prev

    rclpy.init()

    node = rclpy.create_node('test_velocity_publisher')
    publisher = node.create_publisher(String, '/test_velocity', 10)
    pygame.init()
    pygame.display.set_mode((200, 200))
    clock = pygame.time.Clock()
    w_pressed = False
    s_pressed = False
    a_pressed = False
    d_pressed = False

    stop = False
    time.sleep(2)

    try:
        while rclpy.ok():
            # time.sleep(0.05)
            # value= ser.readline()
            # valueInString=value.decode()
            # print(valueInString)
            # key = getch.getch()
            # if key:
            #     key_msg = String()
            #     if key == 'w':
            #         l_speed += 0.5
            #         r_speed += 0.5
            #     elif key == 's':
            #         l_speed -= 0.5
            #         r_speed -= 0.5
            #     elif key == 'a':          
            #         r_speed += 0.5
            #     elif key == 'd':
            #         l_speed += 0.5
            #     elif key == ' ':
            #         l_speed = 0
            #         r_speed = 0                             
            #     key_msg.data = str(l_speed)+" "+str(r_speed)        
            #     node.get_logger().info(f'Publishing test_velocity: {key}')
            #     publisher.publish(key_msg)
                
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_w:
                        w_pressed = True
                    elif event.key == pygame.K_a:
                        a_pressed = True
                    elif event.key == pygame.K_d:
                        d_pressed = True
                    elif event.key == pygame.K_s:
                        s_pressed = True  

                elif event.type == pygame.KEYUP:
                    if event.key == pygame.K_w:
                        w_pressed = False
                    elif event.key == pygame.K_a:
                        a_pressed = False
                    elif event.key == pygame.K_d:
                        d_pressed = False
                    elif event.key == pygame.K_s:
                        s_pressed = False  
                                              

            # Check for both 'W' and 'A' pressed simultaneously
            if w_pressed and a_pressed:
                l_speed = 0.4
                r_speed = 0.6  
            elif w_pressed and d_pressed:
                l_speed = 0.6
                r_speed = 0.4  
            elif s_pressed and a_pressed:
                l_speed = -0.4
                r_speed = -0.6 
            elif s_pressed and d_pressed:
                l_speed = -0.6
                r_speed = -0.4 

            elif w_pressed :
                l_speed = 0.4
                r_speed = 0.4 
            elif s_pressed :
                l_speed = -0.4
                r_speed = -0.4  
            elif a_pressed:
                l_speed = 0.0
                r_speed = 0.4 
            elif d_pressed :
                l_speed = 0.4
                r_speed = 0.0

            else:
                l_speed = 0.0
                r_speed = 0.0           
            
            if  stop == False or l_speed or r_speed:
                if (l_speed == 0 and r_speed ==0):
                    stop = True
                else:
                    stop = False
                key_msg = String()
                key_msg.data = str(l_speed)+" "+str(r_speed)        
                node.get_logger().info(f'Publishing test_velocity: {key_msg.data}')
                publisher.publish(key_msg)  
                
                serial_str = ""


                serial_str = str(r_speed)+","+str(l_speed)
                # if r_speed == 0.0:
                #     serial_str += "0"
                # elif r_speed == 0.4:
                #     serial_str += "50"                
                # elif r_speed == 0.6:
                #     serial_str += "100"     
                # elif r_speed == -0.4:
                #     serial_str += "-50"                
                # elif r_speed == -0.6:
                #     serial_str += "-100"    

                # if l_speed == 0.0:
                #     serial_str += ",0"
                # elif l_speed == 0.4:
                #     serial_str += ",50"                
                # elif l_speed == 0.6:
                #     serial_str += ",100"     
                # elif l_speed == -0.4:
                #     serial_str += ",-50"                
                # elif l_speed == -0.6:
                #     serial_str += ",-100"      
                serial_str += "/n"
                print(serial_str)
                # print(serial_str)
                # if serial_str!=prev:
                x= serial_str.encode()
                ser.write(x)
                # prev = serial_str    
                value= ser.readline()
                valueInString=str(value,'UTF-8')
                print(value.decode())
            clock.tick(60)


    except KeyboardInterrupt:
        pass

    rclpy.spin_once(node)    
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    publish_arrow_key()

