#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN_STAGE_1 12
#define PIN_STAGE_2 2
#define PIN_STAGE_3 8
#define PIN_STAR    5
#define PIN_BLOW    10
#define PIN_SWIPE   7

#define MAX_LIGHT   255

Adafruit_NeoPixel neoPixelStage1(16, PIN_STAGE_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelStage2(16, PIN_STAGE_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelStage3(16, PIN_STAGE_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelStar(8, PIN_STAR, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelBlow(8, PIN_BLOW, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelSwipe(8, PIN_SWIPE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixels[] = {neoPixelStage1, neoPixelStage2, neoPixelStage3, neoPixelStar, neoPixelBlow, neoPixelSwipe};

const double tintSpeed = 0.1;

struct Timer {
  unsigned long startTime;
  unsigned long duration;
  boolean able;

  void initialize() {
    startTime = 0;
    duration = 0;
    able = true;
  }

  boolean check(unsigned long millisec) {
    if (able) {
      startTime = millis();
      duration = millisec;
      able = false;
      return false;
    }
    if (millis() - startTime > duration) {
      startTime = 0;
      duration = 0;
      able = true;
      return true;
    }
    return false;
  }

  void reset() {
    startTime = 0;
    duration = 0;
    able = true;
  }
};

struct LED {
  int pinNum;
  double opacity;
  boolean up;

  void initialize(int p) {
    pinNum = p;
    opacity = 0;
    up = true;
    pinMode(pinNum, OUTPUT);
  }

  boolean show() {
    analogWrite(pinNum, opacity);
    if (opacity < MAX_LIGHT) {
      opacity += tintSpeed;
      up = true;
      return false;
    }
    up = false;
    return true;
  }

  boolean hide() {
    if (opacity > 0) {
      opacity -= tintSpeed;
      analogWrite(pinNum, opacity);
      up = false;
      return false;
    }
    up = true;
    return true;
  }

  void dim() {
    if (opacity >= MAX_LIGHT) up = false;
    else if (opacity <= 0) up = true;
    if (up) opacity += tintSpeed;
    else opacity -= tintSpeed;
    analogWrite(pinNum, opacity);
  }
};

struct AmbientLight {
  int pinNum;
  int pixelNum;
  double opacity;
  boolean up;

  void initialize(int p, int pn) {
    pinNum = p;
    pixelNum = pn;
    opacity = 0;
    up = true;
    neoPixels[pinNum].begin();
    neoPixels[pinNum].clear();
  }

  boolean show() {
    for (int i = 0; i != pixelNum; i++) {
      neoPixels[pinNum].setPixelColor(i, neoPixels[pinNum].Color(opacity, opacity, opacity));
    }
    neoPixels[pinNum].show();
    if (opacity < MAX_LIGHT) {
      opacity += tintSpeed;
      up = true;
      return false;
    }
    up = false;
    return true;
  }

  boolean hide() {
    if (opacity > 0) {
      opacity -= tintSpeed;
      for (int i = 0; i != pixelNum; i++) {
        neoPixels[pinNum].setPixelColor(i, neoPixels[pinNum].Color(opacity, opacity, opacity));
      }
      neoPixels[pinNum].show();
      up = false;
      return false;
    }
    neoPixels[pinNum].clear();
    up = true;
    return true;
  }

  void dim() {
    if (opacity >= MAX_LIGHT) up = false;
    else if (opacity <= 0) up = true;
    if (up) opacity += tintSpeed;
    else opacity -= tintSpeed;
    for (int i = 0; i != pixelNum; i++) {
      neoPixels[pinNum].setPixelColor(i, neoPixels[pinNum].Color(opacity, opacity, opacity));
    }
    neoPixels[pinNum].show();
  }
};

struct BlowLight {
  int pinNum;
  int pixelNum;
  int curLine;
  double opacities[8];
  boolean up;

  void initialize(int p, int pn) {
    pinNum = p;
    pixelNum = pn;
    curLine = 6;
    for (int i = 0; i != 8; i++) opacities[i] = 0;
    up = true;
    neoPixels[pinNum].begin();
    neoPixels[pinNum].clear();
  }

  void dim() {
    if (opacities[7] >= MAX_LIGHT) up = false;
    else if (opacities[7] <= 0) up = true;
    if (up) opacities[7] += tintSpeed;
    else opacities[7] -= tintSpeed;
    neoPixels[pinNum].setPixelColor(7, neoPixels[pinNum].Color(opacities[7], opacities[7], opacities[7]));
    neoPixels[pinNum].show();
  }

  boolean trigger() {
    if (opacities[curLine+1] > 0) {
      opacities[curLine+1] -= tintSpeed*10;
      neoPixels[pinNum].setPixelColor(curLine+1, neoPixels[pinNum].Color(opacities[curLine+1], opacities[curLine+1], opacities[curLine+1]));
    }
    if (curLine >= 0 && opacities[curLine] < MAX_LIGHT) {
      opacities[curLine] += tintSpeed*10;
      neoPixels[pinNum].setPixelColor(curLine, neoPixels[pinNum].Color(opacities[curLine], opacities[curLine], opacities[curLine]));
    }
    else if (curLine >= 0) {
      curLine--;
    }
    neoPixels[pinNum].show();
    if (curLine == -1 && opacities[0] <= 0) {
      curLine = 6;
      neoPixels[pinNum].clear();
      for (int i = 0; i != 8; i++) opacities[i] = 0;
      up = true;
      return true;
    }
    return false;
  }
};

struct SwipeLight {
  int pinNum;
  int pixelNum;
  double opacities[8];
  boolean up;
  boolean completed;

  void initialize(int p, int pn) {
    pinNum = p;
    pixelNum = pn;
    for (int i = 0; i != 8; i++) opacities[i] = 0;
    up = true;
    completed = false;
    neoPixels[pinNum].begin();
    neoPixels[pinNum].clear();
  }

  void dim() {
    if (opacities[0] >= MAX_LIGHT) up = false;
    else if (opacities[0] <= 0) up = true;
    if (up) opacities[0] += tintSpeed;
    else opacities[0] -= tintSpeed;
    neoPixels[pinNum].setPixelColor(0, neoPixels[pinNum].Color(opacities[0], opacities[0], opacities[0]));
    neoPixels[pinNum].show();
  }

  boolean command(int pos) {
    if (pos == 9) dim();
    else if (!completed) {
      for (int i = 0; i < 9-pos; i++) {
        if (opacities[i] < MAX_LIGHT) opacities[i] += tintSpeed;
        neoPixels[pinNum].setPixelColor(i, neoPixels[pinNum].Color(opacities[i], opacities[i], opacities[i]));
      }
      if (opacities[7] >= MAX_LIGHT) {
        completed = true;
      }
      neoPixels[pinNum].show();
    }
    if (completed) {
      return true;
    }
    return false;
  }

  boolean hide() {
    for (int i = 0; i != 8; i++) {
      if (opacities[i] > 0) opacities[i] -= tintSpeed;
      neoPixels[pinNum].setPixelColor(i, neoPixels[pinNum].Color(opacities[i], opacities[i], opacities[i]));
    }
    neoPixels[pinNum].show();
    if (opacities[0] <= 0) {
      for (int i = 0; i != 8; i++) opacities[i] = 0;
      up = true;
      completed = false;
      neoPixels[pinNum].clear();
      return true;
    }
    return false;
  }
};

const int S0 = 0, S1 = 1, S2 = 2, S3 = 3, S4 = 4, S5 = 5, S6 = 6, S7 = 7, S8 = 8, S9 = 9, S10 = 10, S11 = 11, S12 = 12;
const int RESET = 20, NEXT = 21;
int state, processingState;
int input;
boolean show;

int touchSensor = 13, blowSensor = 11, objetSensor = 9;
int swipeSensor = A1, swipePos;

AmbientLight stage1, stage2, stage3, star;
BlowLight blowLight;
SwipeLight swipeLight;
Timer timer;


void setup() {
  Serial.begin(9600);
  
  state = S0;
  processingState = S0;
  show = true;

  pinMode(touchSensor, INPUT);
  pinMode(blowSensor, INPUT);
  pinMode(objetSensor, INPUT);

  stage1.initialize(0, 16);
  stage2.initialize(1, 16);
  stage3.initialize(2, 16);
  star.initialize(3, 8);
  blowLight.initialize(4, 8);
  swipeLight.initialize(5, 8);
  timer.initialize();
}

void loop() {
  switch (state) {
    case S0:
      if (show) {
        if (digitalRead(touchSensor) == HIGH && timer.check(5000)) {
          show = false;
          Serial.write(NEXT);
        }
      }
      else {
        if (processingState == S1) {
          state = S1;
          show = true;
          Serial.write(S1);
        }
      }
      break;
    case S1:
      stage1.show();
      if (show) {
        blowLight.dim();
        if (digitalRead(blowSensor) == HIGH) {
          show = false;
          Serial.write(NEXT);
        }
      }
      else {
        if (blowLight.trigger()) {
          if (processingState == S2) {
            state = S2;
            show = true;
            Serial.write(S2);
          }
        }
      }
      break;
    case S2:
      if (show) {
        stage1.show();
      }
      else {
        boolean b1 = stage1.hide();
        boolean b2 = stage2.show();
        if (b1 & b2) {
          if (processingState == S3) {
            state = S3;
            show = true;
            Serial.write(S3);
          }
        }
      }
      break;
    case S3:
      stage2.show();
      if (show) {
      }
      else {
        if (processingState == S4) {
          state = S4;
          show = true;
          Serial.write(S4);
          swipePos = 9;
        }
      }
      break;
    case S4:
      if (show) {
        stage2.show();
        boolean b1 = isSwiped();
        boolean b2 = swipeLight.command(swipePos);
        if (b1 & b2) {
          show = false;
          Serial.write(NEXT);
        }
      }
      else {
        boolean b1 = swipeLight.hide();
        boolean b2 = stage2.hide();
        boolean b3 = stage3.show();
        if (b1 & b2 & b3) {
          if (processingState == S5) {
            state = S5;
            show = true;
            Serial.write(S5);
          }
        }
      }
      break;
    case S5:
      stage3.show();
      if (show) {
      }
      else {
        if (processingState == S6) {
          state = S6;
          show = true;
          Serial.write(S6);
        }
      }
      break;
    case S6:
      stage3.show();
      if (show) {
      }
      else {
        if (processingState == S7) {
          state = S7;
          show = true;
          Serial.write(S7);
        }
      }
      break;
    case S7:
      stage3.show();
      if (show) {
        star.dim();
        if (digitalRead(objetSensor) == LOW) {
          show = false;
          Serial.write(NEXT);
        }
      }
      else {
        star.show();
        if (processingState == S8) {
          state = S8;
          show = true;
          Serial.write(S8);
        }
      }
      break;
    case S8:
      stage3.show();
      star.show();
      if (show) {
      }
      else {
        if (processingState == S9) {
          state = S9;
          show = true;
          Serial.write(S9);
        }
      }
      break;
    case S9:
      stage3.show();
      if (show) {
        star.dim();
        if (digitalRead(objetSensor) == HIGH) {
          show = false;
          Serial.write(NEXT);
        }
      }
      else {
        star.show();
        if (processingState == S10) {
          state = S10;
          show = true;
          Serial.write(S10);
        }
      }
      break;
    case S10:
      star.show();
      if (show) {
        stage3.show();
      }
      else {
        if (stage3.hide()) {
          if (processingState == S11) {
            state = S11;
            show = true;
            Serial.write(S11);
          }
        }
      }
      break;
    case S11:
      if (show) {
        star.show();
      }
      else {
        if (star.hide()) {
          if (processingState == S12) {
            state = S12;
            show = true;
            Serial.write(S12);
          }
        }
      }
      break;
    case S12:
      if (show) {
        stage1.show();
        stage2.show();
        stage3.show();
      }
      else {
        boolean b1 = stage1.hide();
        boolean b2 = stage2.hide();
        boolean b3 = stage3.hide();
        if (b1 & b2 & b3) {
          if (processingState == S0) {
            state = S0;
            show = true;
            Serial.write(S0);
          }
        }
      }
      break;
  }
}

void serialEvent() {
  input = Serial.read();
  if (input >= S0 && input <= S12) processingState = input;
  else if (input == NEXT) {
    if (show) show = false;
  }
}

boolean isSwiped() {
  int val = analogRead(swipeSensor) / 10;
  if (val == swipePos) swipePos--;
  if (swipePos <= 1) return true;
  return false;
}
