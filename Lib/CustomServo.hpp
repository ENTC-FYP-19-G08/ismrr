#include <Servo.h>

#define REACH_THRES 1
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

    double minSpeed = 0.02;
    double a[4] = {0, 0, 0, 0};

public:
    CustomServo();
    ~CustomServo();
    void init(int pin, int pos = 90, int period = 30, int step = 1);

    bool move(int pos, double speed);
    bool move(int pos, int period, int step);

    void trajInit(int pos, double time);
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

bool CustomServo::move(int pos, double speed)
{
    int _step = 1;
    int _period = _step / speed;
    // Serial.println(pos);
    // Serial.print(' ');
    // Serial.println(speed);
    return move(pos, _period, _step);
}

bool CustomServo::move(int pos, int period, int step)
{
    Serial.println(pos);

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

void CustomServo::trajInit(int pos, double time)
{
    // Serial.println("trajInit-start");
    trajPos = pos;
    a[0] = currPos;
    a[1] = 0;
    double tmp = (double)(pos - currPos) / (time * time);
    // Serial.println(tmp, 10);
    // Serial.println((double)(pos - currPos), 10);
    // Serial.println((double)(time * time), 10);

    a[2] = 3 * tmp;
    a[3] = -2 * tmp / (double)time;
    trajLastMillis = millis();
    trajStartMillis = trajLastMillis;

    // Serial.println(a[0],10);
    // Serial.println(a[1],10);
    // Serial.println(a[2],10);
    // Serial.println(a[3],10);
    // Serial.println("trajInit-end");
}

bool CustomServo::trajFollow()
{
    unsigned long trajCurrentMillis = millis();
    if ((trajCurrentMillis - trajLastMillis) < 1)
    {
        return abs(trajPos - currPos) > REACH_THRES;
    }

    trajLastMillis = trajCurrentMillis;

    if (abs(trajPos - currPos) <= REACH_THRES)
        return false;

    unsigned long trajTime = trajCurrentMillis - trajStartMillis;
    unsigned long trajTime2 = trajTime * trajTime;
    unsigned long trajTime3 = trajTime2 * trajTime;

    int pos = a[0] + a[1] * trajTime + a[2] * trajTime2 + a[3] * trajTime3;
    double speed = abs(a[1] + 2 * a[2] * trajTime + 3 * a[3] * trajTime2) + minSpeed;

    Serial.println(speed, 10);

    // currPos = pos;
    // return abs(trajPos - currPos) > REACH_THRES;

    return move(pos, speed);
}