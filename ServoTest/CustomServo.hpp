#include <Servo.h>

class CustomServo : public Servo
{
private:
    
    int period = 30;
    int step = 1;
    unsigned long moveLastMillis = 0;
    unsigned long trajLastMillis = 0;
    unsigned long trajStartMillis = 0;

    int trajPos = 90;
    int currPos = 90;
    float a[4] = {0, 0, 0, 0};

public:
    CustomServo();
    ~CustomServo();
    void init(int pin, int pos = 90, int period = 30, int step = 1);

    bool move(int pos, float speed);
    bool move(int pos, int period, int step);

    void trajInit(int pos, int time);
    bool trajFollow();
};

CustomServo::CustomServo()
{
}

CustomServo::~CustomServo()
{
}

void CustomServo::init(int pin, int pos, int period, int step)
{
    this->period = period;
    this->currPos = pos;
    Servo::attach(pin);
}

bool CustomServo::move(int pos, float speed)
{
    int _step = 1;
    int _period =_step / speed;
    Serial.println(pos);
    // Serial.print(' ');
    // Serial.println(speed);
    // move(pos, _period, _step);
}

bool CustomServo::move(int pos, int period, int step)
{
    unsigned long moveCurrentMillis = millis();
    if ((moveCurrentMillis - moveLastMillis) < period)
        return pos != currPos;

    moveLastMillis = moveCurrentMillis;

    int diff;

    if (currPos > pos)
    {
        diff = currPos - pos;
        if (diff < step)
            step = diff;
        currPos -= step;
        Servo::write(currPos);
        return true;
    }
    else if (currPos < pos)
    {
        diff = pos - currPos;
        if (diff < step)
            step = diff;
        currPos += step;
        Servo::write(currPos);
        return true;
    }
    else
    {
        return false;
    }
}

void CustomServo::trajInit(int pos, int time)
{
    trajPos = pos;
    a[0] = currPos;
    a[1] = 0;
    float tmp = (float)(pos - currPos) / (float)(time * time);
    a[2] = 3 * tmp;
    a[3] = -2 * tmp;
    trajLastMillis = millis();
}

bool CustomServo::trajFollow()
{
    unsigned long trajCurrentMillis = millis();
    if ((trajCurrentMillis - trajLastMillis) < 1)
        return trajPos != currPos;

    trajLastMillis = trajCurrentMillis;

    unsigned long trajTime = trajCurrentMillis - trajStartMillis;
    unsigned long trajTime2 = trajTime * trajTime;
    unsigned long trajTime3 = trajTime2 * trajTime;

    int pos = a[0] + a[1] * trajTime + a[2] * trajTime2 + a[3] * trajTime3;
    float speed = a[1] + 2 * a[2] * trajTime + 3 * a[3] * trajTime2;
    return move(pos, speed);
}