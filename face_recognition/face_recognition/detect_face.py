from deepface import DeepFace
import os
import cv2 as cv
import time

face_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')
camera = cv.VideoCapture(0)

# def main():

#     DeepFace.stream(db_path = "/home/vwm/fyp_ws/src/face_recognition/images",model_name="ArcFace",enable_face_analysis=False)


    
def captureCam():
    while True:

        result, video_frame = camera.read()  # read frames from the video
        if result is False:
            break  # terminate the loop if the frame is not read successfully

        faces = detect_bounding_box(
            video_frame
        )  # apply the function we created to the video frame

        cv.imshow(
            "smrr_face_detection", video_frame
        )  # display the processed frame in a window named "My Face Detection Project"

        key = cv.waitKey(5)
        # waiting for q key to be pressed and then breaking
        if key == ord('q'):
            camera.release()
            cv.destroyAllWindows()
            break

def detect_bounding_box(vid):

    people = DeepFace.find(vid,db_path="/home/vwm/fyp_ws/src/face_recognition/images",model_name="ArcFace",enforce_detection=False,threshold=0.5)
        
        
    if(not(people[0].empty)):

        # print(people)
        for i in range(len(people)):
            person = people[i]
            person_name = person['identity'][0].split('/')[8]
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

    return 1

if __name__ == '__main__':
    captureCam()
