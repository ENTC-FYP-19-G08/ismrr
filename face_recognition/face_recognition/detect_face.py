from deepface import DeepFace
import os
import os.path as osp
import cv2 as cv
import time
import shutil
import operator

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import Bool
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from face_recog_interfaces.srv import FaceRecogRequest

# face_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')

class face_recog(Node):

    def __init__(self):
        super().__init__('face_recognition')

        self.face_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')

        path_prefix = "/home/vwm/fyp_ws"
        self.image_path = path_prefix + '/src/face_recognition/images'
        self.unkown_path = path_prefix + '/src/face_recognition/cached_images'

        self.people_publisher = self.create_publisher(String, '/smrr/face_recog/people', 2)

        self.br = CvBridge()
        self.image_subscriber = self.create_subscription(Image, '/zed/zed_node/left/image_rect_color',self.captureCam,2)
        self.vid_frame = None
        # self.trigger_subscriber = self.create_subscription(Bool, '/smrr/face_recog/trigger',self.triggerDetection,2)

        self.recognition_service = self.create_service(FaceRecogRequest, '/smrr/face_recog_srv', self.checkFrame)

        self.known_count = 0
        self.unknown_count = 0    
        self.known_stats={'unknown':0}
        self.frame_count = 10
        self.current_frame = 0
        self.angle = 0
        self.person_name = ""

        # self.triggered = False

    def captureCam(self,msg):
        self.vid_frame = msg
    
    def checkFrame(self,request,response):
        print("request received")
        need_name = request.name_request
        need_angle = request.angle_request

        while self.current_frame<self.frame_count:

            video_frame = self.br.imgmsg_to_cv2(self.vid_frame)
            # uncomment for jetson
            # video_frame = cv.cvtColor(video_frame, cv.COLOR_RGBA2RGB)

            gray_image = cv.cvtColor(video_frame, cv.COLOR_BGR2GRAY)
            faces = self.face_cascade.detectMultiScale(gray_image, 1.1, 5, minSize=(40, 40)) 

            # print("faces",len(faces))

            if(len(faces)!=0):
                video_frame = self.identify_people(video_frame) 
            

            # display the processed frame in a window named "My Face Detection Project"
            cv.imshow("smrr_face_detection", video_frame)  

            key = cv.waitKey(5)
            # waiting for q key to be pressed and then breaking
            if key == ord('q'):
                # self.camera.release()
                cv.destroyAllWindows()
                

            self.current_frame+=1

            if self.current_frame==self.frame_count:
                # self.camera.release()
                cv.destroyAllWindows()

                max_count_name = max(self.known_stats.items(), key=operator.itemgetter(1))[0]

                if max_count_name == 'unknown' and self.known_stats['unknown']!=0:
                    unknown_name = input("Give me your name: ") 
                    # rename and moves the files to new location
                    os.rename(self.unkown_path,osp.join(self.image_path,'people',unknown_name))
                    print("New person registered."+unknown_name)
                else:
                    if osp.exists(self.unkown_path):
                        shutil.rmtree(self.unkown_path)
                        print("cached images deleted because person is identified.")
                    
                 # publish results
                msg = String()
                msg.data = str(self.known_stats)
                self.people_publisher.publish(msg)

                # reset stats
                self.current_frame = 0
                self.known_count=0
                self.unknown_count=0
                self.known_stats = {'unknown':0}

                # self.triggered = False

               

                response.name = max_count_name
                response.angle = self.angle

                return response

        

    def identify_people(self,vid):

        original_frame = vid

        people = DeepFace.find(
            vid,
            db_path = self.image_path, 
            model_name='ArcFace',
            enforce_detection=False,
            threshold=0.4
            )
        names = ''  

        if(not(people[0].empty)):
    
            
            for i in range(len(people)):
                person = people[i]
                if not(person.empty):
                    person_name = person['identity'][0].split('/')[8]
                    self.known_count+=1

                    names+=(person_name+" "+str(self.known_count)+',')

                    if person_name in self.known_stats.keys():
                        self.known_stats[person_name] += 1 
                    else:
                        self.known_stats[person_name] = 1


                    # print(str(i)+" "+person_name)

                    x = person['source_x'][0]
                    y = person['source_y'][0]
                    w = person['source_w'][0]
                    h = person['source_h'][0]

                    # print(x,y,w,h)

                    cv.rectangle(vid, (x, y), (x + w, y + h), (0, 255, 0), 4)
                    cv.putText(vid, person_name, (x,y+h+20), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 0), 2, cv.LINE_AA) 
        #       
            
                # for h in person['identity']:
                #     print(h.split('/')[8])
            # else:
            #     person_name = person[0]['identity'][0].split('/')[8]
            #     cv.putText(vid, person_name, (x,y+h+20), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 0), 2, cv.LINE_AA) 
            # print(person_path[8])

        else: 
            self.unknown_count+=1
            self.known_stats["unknown"] = self.unknown_count
            print("unknown person",self.unknown_count)

            if not(osp.exists(self.unkown_path)):
                print("created unknown folder")
                os.mkdir(self.unkown_path)
                cv.imwrite(osp.join(self.unkown_path,str(self.unknown_count)+'.jpg'),original_frame)
            else:
                print("image saved",self.unknown_count)
                cv.imwrite(osp.join(self.unkown_path,str(self.unknown_count)+'.jpg'),original_frame)


        return vid
    
    def save_unknowns(self):

        return
    
    

def main():
    
    rclpy.init()

    recognizer = face_recog()
    # recognizer.captureCam()
    rclpy.spin(recognizer)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    recognizer.destroy_node()
    rclpy.shutdown()
  
if __name__ == '__main__':
    main()