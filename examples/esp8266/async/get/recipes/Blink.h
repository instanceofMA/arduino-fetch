#ifndef BLINK_H
#define BLINK_H

#define LEDPIN 2
#define OFF 1
#define ON 0

#include <Arduino.h>

// Makes the LED blink once every 1 second.
void blinkOnce() {
    pinMode(LEDPIN, OUTPUT);
    // Turning ON.
    digitalWrite(LEDPIN, ON);
    delay(1000);
    // Turning OFF.
    digitalWrite(LEDPIN, OFF);
    delay(1000);
}

// Makes the LED blink twice every 1 second.
void blinkTwice() {
    pinMode(LEDPIN, OUTPUT);
    // Turning ON.
    digitalWrite(LEDPIN, ON);
    delay(500);
    // Turning OFF.
    digitalWrite(LEDPIN, OFF);
    delay(500);
}

// Makes the LED blink n-times every 1 second.
void blinkN(size_t n) {
    pinMode(LEDPIN, OUTPUT);
    // Turning ON.
    digitalWrite(LEDPIN, ON);
    delay(1000/n);
    // Turning OFF.
    digitalWrite(LEDPIN, OFF);
    delay(1000/n);
}

// Makes the LED stop blinking.
void blinkStop() {
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, OFF);
}

// Makes the LED fade in and out once every 1 second.
void fadeOnce() {
    pinMode(LEDPIN, OUTPUT);
    // Fading in.
    for(int i = 0; i < 1024; i+=1024/20) {
        analogWrite(LEDPIN, i);
        delay(100);
    }
    // Fading out.
    for(int i = 1023; i > 0; i-=1024/20) {
        analogWrite(LEDPIN, i);
        delay(100);
    }
}

// Makes the LED fade in and out twice every 1 second.
void fadeTwice() {
    pinMode(LEDPIN, OUTPUT);
    // Fading in.
    for(int i = 0; i < 1024; i+=1024/20) {
        analogWrite(LEDPIN, i);
        delay(50);
    }
    // Fading out.
    for(int i = 1023; i > 0; i-=1024/20) {
        analogWrite(LEDPIN, i);
        delay(50);
    }
}

// Makes the LED fade in and out n-times every 1 second.
void fadeN(size_t n) {
    pinMode(LEDPIN, OUTPUT);
    // Fading in.
    for(int i = 0; i < 1024; i+=1024/20) {
        analogWrite(LEDPIN, i);
        delay(100/n);
    }
    // Fading out.
    for(int i = 1023; i > 0; i-=1024/20) {
        analogWrite(LEDPIN, i);
        delay(100/n);
    }
}

#endif /* BLINK_H */