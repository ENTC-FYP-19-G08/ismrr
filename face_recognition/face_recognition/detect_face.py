from deepface import DeepFace
import os
import cv2 as cv
import time

face_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')
camera = cv.VideoCapture(0)

def main():

    DeepFace.stream(db_path = "/home/vwm/fyp_ws/src/face_recognition/images",model_name="ArcFace",enable_face_analysis=False)


    
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
    gray_image = cv.cvtColor(vid, cv.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray_image, 1.1, 5, minSize=(40, 40))
    for (x, y, w, h) in faces:
        cv.rectangle(vid, (x, y), (x + w, y + h), (0, 255, 0), 4)
        person = DeepFace.find(vid[x-40:x+w+40][y-40:y+h+40],db_path="/home/vwm/fyp_ws/src/face_recognition/images",model_name="ArcFace",enforce_detection=False)
        
        if(len(person)==0):
            print("Unknown")
        else:
            person_name = person[0]['identity'][0].split('/')[8]
            cv.putText(vid, person_name, (x,y+h+20), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 0), 2, cv.LINE_AA) 
        # print(person_path[8])

    return faces

if __name__ == '__main__':
    captureCam()
