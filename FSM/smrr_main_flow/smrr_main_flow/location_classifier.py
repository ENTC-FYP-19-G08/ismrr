import multiprocessing


class LocationClassifier:

    places = [['entc hall', 'entc1', 'entc 1', 'entc one'],
          ["0.5 students' common area", '0.5', 'point five', 'point-five', 'point five common area'],
          ['bio-medical laboratory', 'bio medical lab', 'bio medical laboratory', 'bio-medical lab', 'biomedical lab', 'biomedical laboratory'],
          ['uav laboratory', 'uav lab'],
          ['lift', 'elavator'],
          ['common washrooms', 'washroom', 'toilet'],
          ['staff washroom'],
          ['computer lab', 'computer laboratory'],
          ['staff room', "staff's room", 'staff-room'],
          ['department office', 'entc office', 'main office'],
          ['conference room'],
          ['head of the department office', 'hod office', 'head of the departments office', "head of the department's office"],
          ['analog electronic laboratory', 'analog lab', 'analog electronic lab'],
          ['2nd-floor student common area', '2.5 common area'],
          ['electronic workshop', 'workshop'],
          ['soldering room', 'solder room'],
          ['digital electronic lab', 'digital lab', 'digital laboratory'],
          ['post-grad seminar room', 'pg room', 'pg seminar room', 'pg hall', 'p.g room', 'p.g seminar room', 'p.g hall', 'p.g. room', 'p.g. seminar room', 'p.g. hall', 'p.g.room', 'p.g.seminar room', 'p.g.hall'],
          ['dialog research lab', 'dialog lab', 'dialog research laboratory', 'dialog laboratory'],
          ['telecommunication lab', 'telecom lab', 'telecommunication laboratory', 'telecom laboratory'],
          ['huawei lab', 'mobitel lab', 'huawei laboratory', 'mobitel laboratory'],
          ['radio room'],
          ['vision lab', 'vision laboratory']]
    
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
        while True:
            text = input_q.get()
            text_lower = text.lower()

            for ind, place in enumerate(LocationClassifier.places):
                for place_ in place:
                    if place_ in text_lower:
                        output_q.put(ind)

    def classify_location(self, text):
        self.input_queue.put(text)