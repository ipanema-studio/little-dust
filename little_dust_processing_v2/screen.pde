final int tintSpeed = 5;

void setupScreen() {
  space = new Screen("space.png");
  hand = new Screen("hand.png");
  blow = new Screen("blow.png");
  swipe = new Screen("swipe.png");
  instructions = new Screen[6];
  for (int i = 0; i != instructions.length; i++)
    instructions[i] = new Screen("instruction_" + i + ".png");
  stories = new Screen[7];
  for (int i = 0; i != stories.length; i++)
    stories[i] = new Screen("story_" + i + ".png");
}

class Screen {
  PImage img;
  int opacity;
  boolean up;
  
  Screen(String fileName) {
    img = loadImage("data/images/" + fileName);
    opacity = 0;
    up = true;
  }
  
  boolean show() {
    tint(255, opacity);
    image(img, 0, 0);
    if (opacity < 255) {
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
      tint(255, opacity);
      image(img, 0, 0);
      up = false;
      return false;
    }
    up = true;
    return true;
  }
  
  void dim() {
    if (opacity >= 255) up = false;
    else if (opacity <= 0) up = true;
    if (up) opacity += tintSpeed;
    else opacity -= tintSpeed;
    tint(255, opacity);
    image(img, 0, 0);
  }
}
