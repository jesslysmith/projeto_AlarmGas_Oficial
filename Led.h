#ifndef Led_h
#define Led_h
#include <Arduino.h>

using namespace std;

class Led{
    private:
        int pino;
        
    public:
        Led(int pin);
        void acender();
        void apagar();
        bool status();
        int getPino();
};

#endif
