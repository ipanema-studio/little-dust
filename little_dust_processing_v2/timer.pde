class Timer {
  int startTime;
  int duration;
  boolean ready;
  
  Timer() {
    startTime = 0;
    duration = 0;
    ready = true;
  }
  
  boolean check(int sec) {
    if (ready) {
      startTime = millis();
      duration = sec * 1000;
      ready = false;
      return false;
    }
    if (millis() - startTime > duration) {
      startTime = 0;
      duration = 0;
      ready = true;
      return true;
    }
    return false;
  }
  
  void reset() {
    startTime = 0;
    duration = 0;
    ready = true;
  }
}
