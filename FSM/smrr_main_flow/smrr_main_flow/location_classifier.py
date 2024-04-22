import multiprocessing
from std_msgs.msg import String
import rclpy
from rclpy.node import Node

class LocationClassifier:

    places = {
        "HALL_ENTC1":["ENTC1 hall", "entc1", "entc 1", "entc one", "entc-one"],
        "COMMON_LOWER":["0.5 students' common area", "0.5","point five","point-five", "point five common area", "lower common", "lower-common"],
        "LAB_BM":["bio-medical laboratory", "bio medical lab", "bio medical laboratory", "bio-medical lab", "biomedical lab", "biomedical laboratory"],
        "LAB_UAV":["UAV laboratory", "uav lab"],
        "LIFT":["lift", "elavator","lyft"],
        "WASHROOMS_COMMON":["common washrooms", "washroom", "toilet"],
        "WASHROOMS_STAFF":["staff washroom"],
        "LAB_COMPUTER":["computer lab", "computer laboratory"],
	    "ROOM_LECTURERS":["lecturer rooms", "lecturer's rooms", "lecturers' rooms", "lecturers rooms", "lecturers room"],
        # 9:["staff room", "staff's room", "staff-room"],
        "OFFICE":["department office", "entc office", "main office", 'office'],
        "ROOM_CONFERENCE":["conference room"],
        "ROOM_HOD":["head of the department office", "HOD office"],
        "LAB_ANALOG":["analog electronic laboratory", "analog lab", "analog electronic lab"],
        "COMMON_UPPER":["2nd-floor student common area","2.5 common area", 'upper common', "upper-common"],
        "WORKSHOP":["electronic workshop","workshop"],
        "ROOM_SOLDER":["soldering room", "solder room"],
        "LAB_DIGITAL":["digital electronic lab", "digital lab", "digital laboratory"],
        "HALL_PG":["post-grad seminar room","pg room", "pg seminar room", "pg hall", "p.g room", "p.g seminar room", "p.g hall","p.g. room", "p.g. seminar room", "p.g. hall", "p.g.room", "p.g.seminar room", "p.g.hall", "post grad room", "post graduate room"],
        "LAB_DIALOG":["dialog research lab", "dialog lab","dialog research laboratory", "dialog laboratory", "dialogue research lab", "dialogue lab","dialogue research laboratory", "dialogue laboratory"],
        "LAB_TELECOM":["telecommunication lab", "telecom lab", "telecommunication laboratory", "telecom laboratory"],
        # 21:["Huawei lab","mobitel lab","Huawei laboratory","mobitel laboratory"],
        # 22:["radio room"],
        "LAB_VISION":["vision lab","vision laboratory"],
        "LAB_PG":["pg lab", "post graduate lab", "post grad lab"],
        "HALL_3.5":["3.5 hall", "3.5 lecture hall", "three point five lecture hall"],
        "ROOM_PESHALA":["doctor peshala", "dr peshala", "dr pesala", "doctor pesala"]
}
    direction_request = ["where", "take me", "direction", "way", "go", "find"]
    
    def __init__(self):
        self.input_queue = multiprocessing.Queue()
        self.output_queue = multiprocessing.Queue()

    def initialize_process(self):
        self.process_ = multiprocessing.Process(
            target=self.find_location, args=(self.input_queue, self.output_queue)
        )

        self.process_.start()

    def kill_processes(self):
        if self.process_.is_alive():
            self.process_.kill()

    def find_location(self, input_q, output_q):
        # while True:
        text = input_q.get()
        text_lower = text.lower()

        for place in LocationClassifier.places:
            for place_ in LocationClassifier.places[place]:
                if place_ in text_lower:
                    output_q.put(place)
                    msg = String()
                    msg.data = place
                    self.name_pub.publish(msg)
                    print("Location Found: " + place)
                    return place
        return None
            

    def classify_location(self, text):
        # self.input_queue.put(text)
        text = text
        text_lower = text.lower()

        for place in LocationClassifier.places:
            for place_ in LocationClassifier.places[place]:
                if place_ in text_lower:
                    print("Location Found: " + place)
                    for word in LocationClassifier.direction_request:
                        if word in text_lower:
                            print("Direction Request")
                            return place,True
                    print("Not a Direction Request")
                    return place,False
        return None,False