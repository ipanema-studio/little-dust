final String soundtrackFileName = "The Garden.mp3";

class Soundtrack {
  AudioPlayer sound;
  float volume;
  
  Soundtrack() {
    sound = minim.loadFile("data/sounds/" + soundtrackFileName);
    volume = -40;
    sound.setGain(volume);
  }
  
  void play() {
    if (state == S0) {
      if (volume > -40) {
        volume -= 0.5;
        sound.setGain(volume);
      }
      if (volume == -40) {
        sound.pause();
        sound.rewind();
      }
    }
    else {
      if (volume == -40) {
        sound.loop();
      }
      if (volume < 0) {
        volume += 0.5;
        sound.setGain(volume);
      }
    }
  }
}
