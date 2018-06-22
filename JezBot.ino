
// This helps us to receive the infra-red signal
#include "IRremote.h"
int receiver = 12; // We've put the infra-red receiver on pin 12
IRrecv irrecv(receiver); // This is what we'll use to see if there's a message
decode_results results;  // This is where we'll put the message

// Which pins are we using to drive the right motor?
int enableA = 9;
int pinA1 = 7;
int pinA2 = 5;

// Left motor pins
int enableB = 10;
int pinB1 = 4;
int pinB2 = 2;

unsigned int speed = 150;
long steps = -1;

// This is what happens when we turn JezBot on
void setup()
{
 irrecv.enableIRIn(); // Start the receiver

 // We need to tell it we're sending data out of these pins,
 // not receiving from them.
 pinMode (enableA, OUTPUT);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);

 pinMode (enableB, OUTPUT);
 pinMode (pinB1, OUTPUT);
 pinMode (pinB2, OUTPUT);
}

void slower() { // We'll use this when we want to slow down.
  if (speed < 50) return;
  speed -= 50;
}

void faster() { // And we'll use this to speed up.
  if (speed >= 250) return;
  speed += 50;
}

void stop() { speed = 0; } // We'll do this when we want to stop.

// These set up the motors to go the right way. Leave these alone!
void motorAforward() {
 digitalWrite (pinA1, LOW);
 digitalWrite (pinA2, HIGH);
}
void motorBforward() {
 digitalWrite (pinB1, HIGH);
 digitalWrite (pinB2, LOW);
}
void motorAbackward() {
 digitalWrite (pinA1, HIGH);
 digitalWrite (pinA2, LOW);
}
void motorBbackward() {
 digitalWrite (pinB1, LOW);
 digitalWrite (pinB2, HIGH);
}

void forward() {
  motorAforward(); motorBforward();
}

void reverse() {
  motorAbackward(); motorBbackward();
}

void left() {
  motorAbackward(); motorBforward();
}

void right() {
 motorAforward();
 motorBbackward();
}

// This decides what to do when JezBot sees a signal from
// the remote controller. The signals come as a code number,
// but I've told you what button has that code.

// If you want to change what JezBot does for each button,
// change what happens inside this next section.

void translateIR() {
  switch(results.value) {
  case 0xFF9867:  // This is the button marked "100"
    // We use it to go slower
    Serial.println("Slow");
    slower();
    break;

  case 0xFFB04F: // This is "200"
    Serial.println("Fast");
    faster();
    break;

  case 0xFF6897: // "0" means stop
    Serial.println("Stop");
    stop();
    break;

  case 0xFFA857: // "+""
    Serial.println("Forward");
    forward();
    break;

  case 0xFF22DD: // |<<
    Serial.println("Left");
    left();
    break;

  case 0xFF02FD: // >>|
    right();
    break;

  case 0xFFE01F: // Minus
    reverse();
    break;

  case 0xFF30CF: // 1 (and the rest are the other number buttons)
    steps = 1;
    forward();
    break;

  case 0xFF18E7:
    steps = 2;
    forward();
    break;

  case 0xFF7A85:
    steps = 3;
    forward();
    break;

  case 0xFF10EF:
    steps = 4;
    forward();
    break;

  case 0xFF38C7:
    steps = 5;
    forward();
    break;

  case 0xFF5AA5:
    steps = 6;
    forward();
    break;

  case 0xFF42BD:
    steps = 7;
    forward();
    break;

  case 0xFF4AB5:
    steps = 8;
    forward();
    break;

  case 0xFF52AD:
    steps = 9;
    forward();
    break;

  }
}

// What happens in this section gets done over and over again
void loop()
{
  // If we're in the middle of moving some steps, wait for a
  // second and then count down the number of steps. So if we
  // were moving 9 steps, we move for a second and then we only
  // have 8 steps left to move.
  if (steps >= 1) { delay(100); steps--; speed=200;  }
  // If we come to the end, then we stop.
  if (steps == 0) { stop(); steps--; return; }

  // This checks if we have received a signal from the remote controller
  if (irrecv.decode(&results))
  {
    translateIR();
    irrecv.resume();
  }

  // We use "speed" to keep track of the speed we want, but we
  // don't want to just keep track of it inside the computer. That
  // wouldn't do anything! We have to tell the motor controller
  // about the speed too, so that's what this does.
  if (speed > 200) {
    digitalWrite(enableA, HIGH);
    digitalWrite(enableB, HIGH);
  } else {
    analogWrite(enableA, speed);
    analogWrite(enableB, speed);
  }

}
