import processing.serial.*;
import ddf.minim.*;

final String portName = "COM9";

final int S0 = 0, S1 = 1, S2 = 2, S3 = 3, S4 = 4, S5 = 5, S6 = 6, S7 = 7, S8 = 8, S9 = 9, S10 = 10, S11 = 11, S12 = 12;
final int RESET = 20, NEXT = 21;
int state, arduinoState;
int input;
boolean show;

Serial myPort;
Minim minim;

Screen space;
Screen hand, blow, swipe;
Screen instructions[];
Screen stories[];

Soundtrack soundtrack;
Timer timer;

void setup() {
  //size(1920, 1080);
  fullScreen();
  noCursor();
  state = S0;
  arduinoState = S0;
  show = true;
  myPort = new Serial(this, portName, 9600);
  minim = new Minim(this);
  setupScreen();
  soundtrack = new Soundtrack();
  timer = new Timer();
}

void draw() {
  space.show();
  soundtrack.play();
  
  switch (state) {
    case S0:
      if (state != arduinoState) break;
      if (show) {
        instructions[0].show();
        hand.dim();
      }
      else {
        hand.hide();
        if (instructions[0].hide()) {
          state = S1;
          show = true;
          myPort.write(S1);
        }
      }
      break;
    case S1:
      if (state != arduinoState) break;
      if (show) {
        instructions[1].show();
        blow.dim();
      }
      else {
        blow.hide();
        if (instructions[1].hide()) {
          state = S2;
          show = true;
          myPort.write(S2);
        }
      }
      break;
    case S2:
      if (state != arduinoState) break;
      if (show) {
        stories[0].show();
        if (timer.check(8)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[0].hide()) {
          state = S3;
          show = true;
          myPort.write(S3);
        }
      }
      break;
    case S3:
      if (state != arduinoState) break;
      if (show) {
        stories[1].show();
        if (timer.check(6)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[1].hide()) {
          state = S4;
          show = true;
          myPort.write(S4);
        }
      }
      break;
    case S4:
      if (state != arduinoState) break;
      if (show) {
        instructions[2].show();
        swipe.dim();
      }
      else {
        swipe.hide();
        if (instructions[2].hide()) {
          state = S5;
          show = true;
          myPort.write(S5);
        }
      }
      break;
    case S5:
      if (state != arduinoState) break;
      if (show) {
        stories[2].show();
        if (timer.check(10)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[2].hide()) {
          state = S6;
          show = true;
          myPort.write(S6);
        }
      }
      break;
    case S6:
      if (state != arduinoState) break;
      if (show) {
        stories[3].show();
        if (timer.check(4)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[3].hide()) {
          state = S7;
          show = true;
          myPort.write(S7);
        }
      }
      break;
    case S7:
      if (state != arduinoState) break;
      if (show) {
        instructions[3].show();
      }
      else {
        if (instructions[3].hide()) {
          state = S8;
          show = true;
          myPort.write(S8);
        }
      }
      break;
    case S8:
      if (state != arduinoState) break;
      if (show) {
        instructions[4].show();
        if (timer.check(5)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (instructions[4].hide()) {
          state = S9;
          show = true;
          myPort.write(S9);
        }
      }
      break;
    case S9:
      if (state != arduinoState) break;
      if (show) {
        instructions[5].show();
      }
      else {
        if (instructions[5].hide()) {
          state = S10;
          show = true;
          myPort.write(S10);
        }
      }
      break;
    case S10:
      if (state != arduinoState) break;
      if (show) {
        stories[4].show();
        if (timer.check(8)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[4].hide()) {
          state = S11;
          show = true;
          myPort.write(S11);
        }
      }
      break;
    case S11:
      if (state != arduinoState) break;
      if (show) {
        stories[5].show();
        if (timer.check(8)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[5].hide()) {
          state = S12;
          show = true;
          myPort.write(S12);
        }
      }
      break;
    case S12:
      if (state != arduinoState) break;
      if (show) {
        stories[6].show();
        if (timer.check(8)) {
          show = false;
          myPort.write(NEXT);
        }
      }
      else {
        if (stories[6].hide()) {
          state = S0;
          show = true;
          myPort.write(S0);
        }
      }
      break;
  }
}

//void mouseClicked() {
//  show = !show;
//}

void serialEvent(Serial myPort) {
  input = myPort.read();
  if (input >= S0 && input <= S12) arduinoState = input;
  else if (input == NEXT) {
    if (show) show = false;
  }
}
