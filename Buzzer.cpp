#include "Buzzer.h"

Buzzer::Buzzer(int pin){
    pinMode(pin,OUTPUT);
    pino=pin;
}

void Buzzer::acionar(){
    tone(pino,3000);
}

void Buzzer::desligar(){
    noTone(pino);
}

bool Buzzer::status(){
    return digitalRead(pino);
}
