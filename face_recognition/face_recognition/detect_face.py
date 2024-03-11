from deepface import DeepFace
import os
import cv2 as cv
import time
import shutil

import rclpy
from rclpy.node import Node
from std_msgs.msg import String

# face_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')

class face_recog(Node):

    def __init__(self,camera_ID,path_prefix):
        super().__init__('face_recognition')
        self.camera_ID = camera_ID
        self.camera = cv.VideoCapture(camera_ID) 
        self.image_path = path_prefix + '/src/face_recognition/images'
        self.people_publisher = self.create_publisher(String, '/smrr/people', 10)
    
    def captureCam(self):
        while True:

            # read frames from the video
            result, video_frame = self.camera.read()  
            width = len(video_frame[0])
            # video_frame = video_frame[:,0:width//2+1]

            # terminate the loop if the frame is not read successfully
            if result is False:
                break  

            video_frame = self.identify_people(video_frame) 

            # display the processed frame in a window named "My Face Detection Project"
            cv.imshow("smrr_face_detection", video_frame)  

            key = cv.waitKey(5)
            # waiting for q key to be pressed and then breaking
            if key == ord('q'):
                self.camera.release()
                cv.destroyAllWindows()
                break

    def identify_people(self,vid):

        people = DeepFace.find(
            vid,
            db_path = self.image_path, 
            model_name="ArcFace", 
            enforce_detection=False,
            threshold=0.5
            )
            
            
        if(not(people[0].empty)):

            names = ''
            for i in range(len(people)):
                person = people[i]
                person_name = person['identity'][0].split('/')[8]
                names+=(person_name+',')
                # print(str(i)+" "+person_name)

                x = person['source_x'][0]
                y = person['source_y'][0]
                w = person['source_w'][0]
                h = person['source_h'][0]

                # print(x,y,w,h)

                cv.rectangle(vid, (x, y), (x + w, y + h), (0, 255, 0), 4)
                cv.putText(vid, person_name, (x,y+h+20), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 0), 2, cv.LINE_AA) 
    #       
            msg = String()
            msg.data = names
            self.people_publisher.publish(msg)
                # for h in person['identity']:
                #     print(h.split('/')[8])
            # else:
            #     person_name = person[0]['identity'][0].split('/')[8]
            #     cv.putText(vid, person_name, (x,y+h+20), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 0), 2, cv.LINE_AA) 
            # print(person_path[8])

        return vid
    
    

def main():
    
    rclpy.init()

    recognizer = face_recog(0,"/home/vwm/fyp_ws")
    recognizer.captureCam()
    rclpy.spin(recognizer)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    recognizer.destroy_node()
    rclpy.shutdown()
  
