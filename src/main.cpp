#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#include <tweetnacl.h>

#include <utility>
#include <vector>

#define OLED_MOSI   2
#define OLED_CLK   12
#define OLED_DC    13
#define OLED_CS    14
#define OLED_RESET 15
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// PlatformIO doesn't support c++14 yet
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args){
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Hi Jeff, looking for open wifi networks.");
  display.display();
}

class Scanner;
class State {
public:
  virtual void run(Scanner &scanner) = 0;
  virtual ~State() = default;
};

class Scanning : public State {
public:
  Scanning() {}
  void run(Scanner &scanner) {

  }
private:
  std::vector<std::string> done;
};

class Sending : public State {
public:
  Sending() {}
};

class Recieving : public State {

};

class Reporting : public State {

};

class Scanner {
public:
  Scanner() : state_(make_unique<Scanning>()) {}
  void tick() {
    state_->run(*this);
  };

  void promote(std::unique_ptr<State> state) {
    state_ = std::move(state);
  };
private:
  std::unique_ptr<State> state_;
};

auto scanner = Scanner();

void loop() {
  int n = WiFi.scanNetworks();
  display.clearDisplay();
  display.setCursor(0,0);
  if(n > 0) {
    display.println("Yo, try these:");
    for(int i = 0; i < n; i++) {
      if(WiFi.encryptionType(i) == ENC_TYPE_NONE)
        display.println(WiFi.SSID(i));
    }
  } else {
    display.println("no networks :(");
  }
  display.display();
  delay(10);
}
