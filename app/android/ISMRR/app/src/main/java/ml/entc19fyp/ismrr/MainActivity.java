package ml.entc19fyp.ismrr;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity extends AppCompatActivity {

    DatabaseReference dbRef;
    Button btnFront,btnBack,btnLeft,btnRight;

    View.OnTouchListener btnTouchListener =new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            String topic=v.getTag().toString();
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    sendTopic(topic+"_prs");
                    v.setPressed(true);
                    v.performClick();
                    break;
                case MotionEvent.ACTION_UP:
                    sendTopic(topic+"_rls");
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
        setContentView(R.layout.activity_main);
        dbRef= FirebaseDatabase.getInstance().getReference("/ISMRR/robot/move");
        Log.d("QWE","ST");

        btnFront=findViewById(R.id.btnFront);
        btnBack=findViewById(R.id.btnBack);
        btnLeft=findViewById(R.id.btnLeft);
        btnRight=findViewById(R.id.btnRight);

        btnFront.setOnTouchListener(btnTouchListener);
        btnBack.setOnTouchListener(btnTouchListener);
        btnLeft.setOnTouchListener(btnTouchListener);
        btnRight.setOnTouchListener(btnTouchListener);

    }

    public void sendTopic(String topic){
        long timeStamp=System.currentTimeMillis();
        dbRef.child(topic).setValue(timeStamp);
        Log.d("QWE",topic+":"+timeStamp);
    }
}