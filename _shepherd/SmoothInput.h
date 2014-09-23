#ifndef __smooth_input__
#define __smooth_input__

const int SMOOTH_INPUT_BUFFER_SIZE = 10;
const int CHANGE_THRESHOLD = 2;

typedef void (*NewValueCallback)();

class SmoothInput {

  public:

    SmoothInput(short pin, NewValueCallback callback);

    void service();

    int getValue();

  private:
    short pin;
    NewValueCallback callback;

    int readings[SMOOTH_INPUT_BUFFER_SIZE];
    int currentIndex;
    int total;
    int average;
    int previousAverage;
    bool stable;

};

#endif
