#include <Servo.h>

class CustomServo : public Servo
{
private:
    /* data */
    int period = 100;
    unsigned long lastMoveMillis = 0;
    int currPos = 90;

public:
    CustomServo();
    ~CustomServo();
    void init(int pin, int _period = 30, int _pos = 90);
    bool move(int pos);
};

CustomServo::CustomServo()
{
}

CustomServo::~CustomServo()
{
}

void CustomServo::init(int pin, int _period, int _pos)
{
    period = _period;
    currPos = _pos;
    Servo::attach(pin);
}

bool CustomServo::move(int pos)
{
    unsigned long currentMoveMillis = millis();
    if ((currentMoveMillis - lastMoveMillis) < period)
        return pos != currPos;

    lastMoveMillis = currentMoveMillis;

    if (currPos > pos)
    {
        currPos--;
        Servo::write(currPos);
        return true;
    }
    else if (currPos < pos)
    {
        currPos++;
        Servo::write(currPos);
        return true;
    }
    else
    {
        return false;
    }
}
