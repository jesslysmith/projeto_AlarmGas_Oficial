#ifndef Buzzer_h
#define Buzzer_h
#include <Arduino.h>

class Buzzer{
    private:
        int pino;

    public:
        Buzzer(int pin);
        void acionar();
        void desligar();
        bool status();
};

#endif 