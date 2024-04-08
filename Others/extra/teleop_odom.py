import rclpy
from std_msgs.msg import String
import pygame
import sys
import time

l_odom = 0
r_odom = 0

def publish_test_wheel_odom():
    global l_odom, r_odom

    rclpy.init()

    node = rclpy.create_node('test_wheel_odom_publisher')
    publisher = node.create_publisher(String, '/test_wheel_odom', 10)
    pygame.init()
    pygame.display.set_mode((200, 200))
    clock = pygame.time.Clock()
    w_pressed = False
    s_pressed = False
    a_pressed = False
    d_pressed = False

    time.sleep(2)

    try:
        while rclpy.ok():
                
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
                l_odom += 1
                r_odom += 2
            elif w_pressed and d_pressed:
                l_odom += 2
                r_odom += 1 
            elif s_pressed and a_pressed:
                l_odom -= 1
                r_odom -= 2 
            elif s_pressed and d_pressed:
                l_odom -= 2
                r_odom -= 1 

            elif w_pressed :
                l_odom += 1
                r_odom += 1
            elif s_pressed :
                l_odom -= 1
                r_odom -= 1
            elif a_pressed:
                l_odom += 0
                r_odom += 1
            elif d_pressed :
                l_odom += 1
                r_odom += 0

            else:
                l_odom += 0
                r_odom += 0         
            
            if l_odom > 1000:
                l_odom = l_odom - 1000
            if r_odom > 1000:  
                r_odom = r_odom - 1000
            if l_odom < -1000:
                l_odom = l_odom + 1000
            if r_odom < -1000:  
                r_odom = r_odom + 1000

            key_msg = String()
            key_msg.data = str(l_odom)+" "+str(r_odom)        
            node.get_logger().info(f'Publishing test_wheel_odom: {key_msg.data}')
            publisher.publish(key_msg)  

            clock.tick(80)


    except KeyboardInterrupt:
        pass

    rclpy.spin_once(node)    
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    publish_test_wheel_odom()

