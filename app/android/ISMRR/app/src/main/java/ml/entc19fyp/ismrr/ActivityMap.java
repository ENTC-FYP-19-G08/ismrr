package ml.entc19fyp.ismrr;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import ml.entc19fyp.ismrr.CustomMapView.Location;
public class ActivityMap extends AppCompatActivity {

    Button btnGo, btnTest;
    DatabaseReference dbRefMain, dbRefTargetLocation, dbRefCurrentLocation;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_map);

        CustomMapView map = findViewById(R.id.mapView);
        btnGo = findViewById(R.id.btnGo);
        btnTest = findViewById(R.id.btnTest);
        dbRefMain = FirebaseDatabase.getInstance().getReference("/ISMRR");
        dbRefTargetLocation = dbRefMain.child("/robot/reach");
        dbRefCurrentLocation = dbRefMain.child("/app/pos");
        map.setMapImageDimensions(1113, 1570);

        map.setOnMapTapListener(new CustomMapView.OnMapTapListener() {
            @Override
            public void onMapTap(float x, float y) {
                Log.d("QWER", x + ":" + y);
                // Handle the tap event and call your functions with the coordinates (x, y)
                // For example, you can display a marker or perform other actions.
                // The coordinates (x, y) represent pixel coordinates on the map.
            }
        });

        btnTest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                map.setCurrentLocation(new CustomMapView.Location(1113, 1570));
            }
        });

        btnGo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CustomMapView.Location targetLocation = map.getTargetLocation();
                if (targetLocation != null) {
                    long timeStamp=System.currentTimeMillis();
                    dbRefTargetLocation.setValue(targetLocation.toString()+","+timeStamp);
                }
            }
        });

        dbRefCurrentLocation.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                String value = dataSnapshot.getValue(String.class);
                if (value != null) {
                    map.setCurrentLocation(new Location(value));
                    Log.d("QWER", value);
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                System.out.println("The read failed: " + databaseError.getCode());
            }
        });
    }
}