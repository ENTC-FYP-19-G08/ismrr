package ml.entc19fyp.ismrr;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;

import java.util.ArrayList;
import java.util.List;

import android.util.Log;

import androidx.annotation.NonNull;

public class CustomMapView extends androidx.appcompat.widget.AppCompatImageView {

    private List<Pin> pins = new ArrayList<>();
    private Pin targetPin, currentPin;
    private Paint targetPinPaint, currentPinPaint;
    private OnMapTapListener onMapTapListener;

    private int mapImageWidth = 0;
    private int mapImageHeight = 0;

    public CustomMapView(Context context) {
        super(context);
        initialize();
    }

    public CustomMapView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize();
    }

    public CustomMapView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initialize();
    }

    private void initialize() {
        targetPinPaint = new Paint();
        targetPinPaint.setColor(Color.RED);
        targetPinPaint.setStyle(Paint.Style.FILL);

        currentPinPaint = new Paint();
        currentPinPaint.setColor(Color.GREEN);
        currentPinPaint.setStyle(Paint.Style.FILL);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        if (action == MotionEvent.ACTION_DOWN) {
            float x = event.getX();
            float y = event.getY();

            // Store the tapped location coordinates
            if (onMapTapListener != null) {
                onMapTapListener.onMapTap(x, y);
            }

            // Create and add a pin at the tapped location

            targetPin = new Pin(x, y);
            invalidate(); // Redraw the pins on the map

            return true;
        }
        return super.onTouchEvent(event);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        // Draw the pins on the map
        if (targetPin != null)
            canvas.drawCircle(targetPin.x, targetPin.y, 10, targetPinPaint);

        if (currentPin != null)
            canvas.drawCircle(currentPin.x, currentPin.y, 8, currentPinPaint);

    }

    public void setMapImageDimensions(int width, int height) {
        mapImageWidth = width;
        mapImageHeight = height;
    }

    public Location getLocationFromPin(Pin pin) {
        if (mapImageWidth == 0 || mapImageHeight == 0 || pin == null) {
            return null; // Return null if the dimensions are not set
        }

        // Calculate the pixel coordinates within the image
        float x = (pin.x / getWidth()) * mapImageWidth;
        float y = (pin.y / getHeight()) * mapImageHeight;

        return new Location(x, y);
    }

    public Pin getPinFromLocation(Location location) {
        float x = (location.x / mapImageWidth) * getWidth();
        float y = (location.y / mapImageHeight) * getHeight();
        return new Pin(x, y);
    }

    public void setOnMapTapListener(OnMapTapListener listener) {
        this.onMapTapListener = listener;
    }

    public Location getTargetLocation() {
        return getLocationFromPin(targetPin);
    }

    public void setCurrentLocation(Location location) {
        currentPin = getPinFromLocation(location);
        invalidate();
    }

    // Define an interface for handling map tap events
    public interface OnMapTapListener {
        void onMapTap(float x, float y);
    }

    public static class Pin {
        float x;
        float y;

        Pin(float x, float y) {
            this.x = x;
            this.y = y;
        }
    }

    public static class Location {
        float x;
        float y;

        Location(float x, float y) {
            this.x = x;
            this.y = y;
        }

        Location(String xy) {
            String[] xyPair = xy.split(",");
            if (xyPair.length == 2) {
                // Convert the substrings to float values
                this.x = Float.parseFloat(xyPair[0]);
                this.y = Float.parseFloat(xyPair[1]);
            } else {
                System.out.println("Input does not contain two comma-separated float values.");
            }
        }

        @NonNull
        public String toString() {
            return (int)this.x + "," + (int)this.y;
        }

    }


}
