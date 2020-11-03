#include <PinChangeInterruptSettings.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptBoards.h>
#include <Servo.h>
#include <PinChangeInterrupt.h>

/*
 * 
 * 
 * 
 * 
 */
const byte channel_pin[] = {8, 9};
volatile unsigned long rising_start[] = {0, 0};
volatile long channel_length[] = {0, 0};
Servo angle180;
Servo angle0;
int laser = 12;

void setup() {
  Serial.begin(57600);

  pinMode(channel_pin[0], INPUT);
  pinMode(channel_pin[1], INPUT);
  angle180.attach(4);
  angle0.attach(6);
  pinMode(laser, OUTPUT);

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[0]), onRising0, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[1]), onRising1, CHANGE);
}

void processPin(byte pin) {
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channel_pin[pin]));

  if(trigger == RISING) {
    rising_start[pin] = micros();
  } else if(trigger == FALLING) {
    channel_length[pin] = micros() - rising_start[pin];
  }
}

void onRising0(void) {
  processPin(0);
}

void onRising1(void) {
  processPin(1);
}

void loop() {
  Serial.print(channel_length[0]);
  Serial.print(" | ");
  Serial.print(channel_length[1]);
  
  Serial.println("");
  delay(100);
  angle180.write(180);
  angle0.write(0);
  
  
  if (channel_length[0] >= 2000) {
    digitalWrite(laser, LOW);
  } else {
    digitalWrite(laser, HIGH);
  }
  
}
