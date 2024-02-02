package ml.entc19fyp.ismrr;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class ActivityRemote extends AppCompatActivity {

    DatabaseReference dbRef;
    ImageButton btnFront, btnBack, btnLeft, btnRight, btnStop, btnPlay;

    ActDir actDir;

    View.OnTouchListener btnTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            String tag = v.getTag().toString();
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    switch (tag) {
                        case "F":
                            actDir.F = 1;
                            break;
                        case "B":
                            actDir.B = 1;
                            break;
                        case "L":
                            actDir.L = 1;
                            break;
                        case "R":
                            actDir.R = 1;
                            break;
                        case "S":
                            actDir.S = 1;
                            break;
                        case "P":
                            actDir.P = 1;
                            break;
                    }
                    sendTopic();
                    v.setPressed(true);
                    v.performClick();
                    break;
                case MotionEvent.ACTION_UP:
                    switch (tag) {
                        case "F":
                            actDir.F = 0;
                            break;
                        case "B":
                            actDir.B = 0;
                            break;
                        case "L":
                            actDir.L = 0;
                            break;
                        case "R":
                            actDir.R = 0;
                            break;
                        case "S":
                            actDir.S = 0;
                            break;
                        case "P":
                            actDir.P = 0;
                            break;
                    }
                    sendTopic();
                    v.setPressed(false);
                    break;
                // Handle other touch events here
            }
            return true; // Return true to indicate that the event has been handled
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remote);
        dbRef = FirebaseDatabase.getInstance().getReference("/ISMRR/robot/move");
        Log.d("QWE", "ST");

        actDir = new ActDir();

        btnFront = findViewById(R.id.btnFront);
        btnBack = findViewById(R.id.btnBack);
        btnLeft = findViewById(R.id.btnLeft);
        btnRight = findViewById(R.id.btnRight);
        btnStop = findViewById(R.id.btnStop);
        btnPlay = findViewById(R.id.btnPlay);

        btnFront.setOnTouchListener(btnTouchListener);
        btnBack.setOnTouchListener(btnTouchListener);
        btnLeft.setOnTouchListener(btnTouchListener);
        btnRight.setOnTouchListener(btnTouchListener);
        btnStop.setOnTouchListener(btnTouchListener);
        btnPlay.setOnTouchListener(btnTouchListener);
    }

    public void sendTopic() {
//        long timeStamp = System.currentTimeMillis();
//        dbRef.child(topic).setValue(timeStamp);
//        Log.d("QWE", topic + ":" + timeStamp);
        dbRef.setValue(actDir);
    }

    public class ActDir {
        public int F = 0;
        public int B = 0;
        public int L = 0;
        public int R = 0;
        public int S = 0;
        public int P = 0;

    }
}