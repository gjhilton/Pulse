#ifndef __dbbutton__
#define __dbbutton__

const int BUTTON_DEBOUNCE_TIME_MS = 50;

typedef void (*ButtonCallback)();

class DebouncedButton {

  public:

    DebouncedButton(short pin, ButtonCallback callback);

    void begin();
    void service();

  private:
    short pin;
    short pressedState;
    short lastPressedState;
    int debounceTime;
    unsigned long previousDebounceMillis;
    ButtonCallback callback;

};

#endif
