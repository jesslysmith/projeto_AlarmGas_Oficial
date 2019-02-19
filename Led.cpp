#include "Led.h"

Led::Led(int pin){
    pinMode(pin,OUTPUT);
    pino = pin;
}

void Led::acender(){
    digitalWrite(pino,HIGH);
}

void Led::apagar(){
    digitalWrite(pino,LOW);
}

bool Led::status(){
    return digitalRead(pino);
}

int Led::getPino(){
    return pino;
}
