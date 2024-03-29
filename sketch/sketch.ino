#include <Adafruit_SSD1306.h>
// #include <splash.h>
// include libraries
// #include <Wire.h>
#include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// include screen
#define OLED_WIDTH 128  // OLED display width, in pixels
#define OLED_HEIGHT 64  // OLED display height, in pixels
// include outputs
#define vibOutPin1 3
#define vibOutPin2 5
// SSD1306 Plugs into A4 (SCA) and A5 (SCL)
// include inputs
// rotary 1
#define KNOB1_CLK 2
#define KNOB1_DT 4
#define KNOB1_BTN 6
// rotary 2
#define KNOB2_CLK 7
#define KNOB2_DT 8
#define KNOB2_BTN 9
// rotary 3
#define KNOB3_CLK 10
#define KNOB3_DT 11
#define KNOB3_BTN 12
// rotary 4
#define KNOB4_CLK 13
#define KNOB4_DT 14
#define KNOB4_BTN 15
// set initial values
int strength = 200;  // Variable to store the input from the potentiometer
int length = 250;    // Variable to store the input from the potentiometer
int span = 250;      // Variable to store the input from the potentiometer
// int count = 25; // Variable to store the input from the potentiometer
int run = -1;        // Variable to store the input from the potentiometer
int startup = 1;     // Variable to store the input from the potentiometer
int countDown = 26;  // Variable to store the input from the potentiometer
int countLast = 26;
int statusBary1 = 0;
int statusBary2 = 7;
int statusWidth = 43;
int countX = 50;
int countY = 23;
int lastClk1 = HIGH;
int lastBtn1 = HIGH;
int lastClk2 = HIGH;
int lastBtn2 = HIGH;
int lastClk3 = HIGH;
int lastBtn3 = HIGH;
int lastClk4 = HIGH;
int lastBtn4 = HIGH;
int newClk = HIGH;
int lastClk = HIGH;
int newBtn = 0;
int lastBtn = 0;
int vibOutPin = 0;
int strengthBar = ((strength) / 5);
int lengthBar = ((length) / 14);
int spanBar = ((span) / 8);
int var1 = 0;
int var2 = 0;
unsigned long currentMillis;
unsigned long lastMillis;
unsigned long vibeTime1a;
unsigned long vibeTime1b;
unsigned long vibeTime2a;
unsigned long vibeTime2b;
int period;
// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void setup() {
  // start display
  Serial.begin(9600);
  //	initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      ;
  }
  StartupDisp();
  pinMode(vibOutPin1, OUTPUT);  // define a pin as output
  pinMode(vibOutPin2, OUTPUT);  // define a pin as output
  pinMode(KNOB1_CLK, INPUT);
  pinMode(KNOB1_DT, INPUT);
  pinMode(KNOB1_BTN, INPUT_PULLUP);
  pinMode(KNOB2_CLK, INPUT);
  pinMode(KNOB2_DT, INPUT);
  pinMode(KNOB2_BTN, INPUT_PULLUP);
  pinMode(KNOB3_CLK, INPUT);
  pinMode(KNOB3_DT, INPUT);
  pinMode(KNOB3_BTN, INPUT_PULLUP);
  pinMode(KNOB4_CLK, INPUT);
  pinMode(KNOB4_DT, INPUT);
  pinMode(KNOB4_BTN, INPUT_PULLUP);
  lastMillis = 0;
  vibeTime1a = 0;
  vibeTime1b = 0;
  vibeTime2a = 0;
  vibeTime2b = 9999;
}

void loop() {
  if (run > 0) {
    currentMillis = millis();
    period = min(span, length);  // assigns period to the smaller of span or length
    if (currentMillis - lastMillis > period) {
      runTappers();
    } else {
      analogWrite(vibOutPin1, 0);
      analogWrite(vibOutPin2, 0);
    }
  };
  if (startup > 0) {
    StartupScreen();
  };
  if (startup < 0) {
    rotaryListen();
  };
}

void StartupDisp() {
  oled.clearDisplay();         // clear display
  oled.setTextSize(2);         // text size
  oled.setTextColor(WHITE);    // text color
  oled.setCursor(0, 0);        // position to display
  oled.println("OpenTapper");  // text to display
  oled.setTextSize(1);         // text size
  oled.setCursor(0, 20);       // position to display
  oled.println("Version 0.1");
  oled.setCursor(0, 30);                                      // position to display
  oled.println("github.com/deighne/bilateral-stimulation.");  // text to display
  oled.setCursor(0, 50);                                      // position to display
  oled.println("Push COUNT to start.");                       // text to display
  oled.display();                                             // show on OLED
}

void StartupScreen() {
  int newClk4 = digitalRead(KNOB4_CLK);
  int newBtn4 = digitalRead(KNOB4_BTN);
  int dtValue4 = digitalRead(KNOB4_DT);
  if (newBtn4 != lastBtn4) {
    if (newBtn4 = HIGH) {
      lastBtn4 = newBtn4;
      startup = -1;
      oled.clearDisplay();  // clear display
      oled.setTextSize(1);  // text size
      printAlignL(String(strength * 100 / 255), statusBary1);
      printAlignC(String(length), statusBary1);
      printAlignR(String(span), statusBary1);
      oled.setTextSize(2);  // text size
      printCenter(String(countDown - 1));
      oled.display();  // show on OLED
      delay(100);
    }
  };
}

void runTappers() {
  if (vibeTime2b > 0) {
    if (currentMillis - vibeTime2b >= span) {
      analogWrite(vibOutPin1, strength);
      vibeTime2b = 0;
      vibeTime1a = currentMillis;
      reduceCount();
      // vertical line
      oled.fillTriangle(statusBary1, OLED_HEIGHT - strengthBar, 0, OLED_HEIGHT, lengthBar, OLED_HEIGHT - 1, WHITE);
      if (countDown < 1) {
        run = -1;
        countDown = countLast;
        analogWrite(vibOutPin1, 0);
        oled.fillRect(OLED_WIDTH / 2 - 15, OLED_HEIGHT / 2 - 15, 30, 30, BLACK);
        printCenter(String(countDown - 1));  // text to display
        oled.fillTriangle(statusBary1, OLED_HEIGHT - strengthBar, 0, OLED_HEIGHT, lengthBar, OLED_HEIGHT - 1, BLACK);
        vibeTime1a = 0;
        vibeTime1b = 0;
        vibeTime2a = 0;
        vibeTime2b = 9999;
      }
      // horizontal line
      oled.drawLine(lengthBar + 1, OLED_HEIGHT - 1, OLED_WIDTH - lengthBar, OLED_HEIGHT - 1, BLACK);
      oled.display();  // show on OLED
    };
  };
  if (vibeTime1a > 0) {
    if (currentMillis - vibeTime1a >= length) {
      analogWrite(vibOutPin1, 0);
      vibeTime1a = 0;
      vibeTime1b = currentMillis;
      // vertical line
      oled.fillTriangle(statusBary1, statusBary2, statusBary1, OLED_HEIGHT - 1, 64, OLED_HEIGHT - 1, BLACK);
      // horizontal line
      oled.drawLine(lengthBar + 1, OLED_HEIGHT - 1, OLED_WIDTH - lengthBar, OLED_HEIGHT - 1, WHITE);
      oled.display();  // show on OLED
    };
  };
  if (vibeTime1b > 0) {
    if (currentMillis - vibeTime1b >= span) {
      analogWrite(vibOutPin2, strength);
      vibeTime1b = 0;
      vibeTime2a = currentMillis;
      // vertical line
      oled.fillTriangle(OLED_WIDTH - 1, OLED_HEIGHT - strengthBar, OLED_WIDTH - 1, OLED_HEIGHT, OLED_WIDTH - lengthBar, OLED_HEIGHT - 1, WHITE);
      // horizontal line
      oled.drawLine(lengthBar + 1, OLED_HEIGHT - 1, OLED_WIDTH - lengthBar, OLED_HEIGHT - 1, BLACK);
      oled.display();  // show on OLED
    };
  };
  if (vibeTime2a > 0) {
    if (currentMillis - vibeTime2a >= length) {
      analogWrite(vibOutPin2, 0);
      vibeTime2a = 0;
      vibeTime2b = currentMillis;
      // vertical line
      oled.fillTriangle(OLED_WIDTH - 1, statusBary2, OLED_WIDTH - 1, OLED_HEIGHT - 1, 65, OLED_HEIGHT - 1, BLACK);
      // horizontal line
      oled.drawLine(lengthBar + 1, OLED_HEIGHT - 1, OLED_WIDTH - lengthBar, OLED_HEIGHT - 1, WHITE);
      oled.display();  // show on OLED
    };
  };
}

void rotaryListen() {
  // grab values
  int newClk1 = digitalRead(KNOB1_CLK);
  int newClk2 = digitalRead(KNOB2_CLK);
  int newClk3 = digitalRead(KNOB3_CLK);
  int newClk4 = digitalRead(KNOB4_CLK);
  int newBtn1 = digitalRead(KNOB1_BTN);
  int newBtn2 = digitalRead(KNOB2_BTN);
  int newBtn3 = digitalRead(KNOB3_BTN);
  int newBtn4 = digitalRead(KNOB4_BTN);
  int dtValue1 = digitalRead(KNOB1_DT);
  int dtValue2 = digitalRead(KNOB2_DT);
  int dtValue3 = digitalRead(KNOB3_DT);
  int dtValue4 = digitalRead(KNOB4_DT);
  oled.setTextSize(1);       // text size
  oled.setTextColor(WHITE);  // text color
  // encoder 1
  {
    if (newClk1 != lastClk1 || newBtn1 != lastBtn1) {
      if (newClk1 != lastClk1) {
        knobInput(newClk1, lastClk1, dtValue1);
        strength = (strength) + var1;
      };
      if (newBtn1 != lastBtn1) {
        lastBtn1 = newBtn1;
        strength = 200;
      };
      strengthBar = ((strength) / 5);
      oled.fillRect(statusBary1, statusBary1, statusWidth, statusBary2, BLACK);
      printAlignL(String(strength * 100 / 255), statusBary1);  // text to display
      // oled.println(String(strength * 100 / 255)); // text to display
      oled.display();  // show on OLED
    };
    // encoder 2
    if (newClk2 != lastClk2 || newBtn2 != lastBtn2) {
      if (newClk2 != lastClk2) {
        knobInput(newClk2, lastClk2, dtValue2);
        length = (length) + var1;
      };
      if (newBtn2 != lastBtn2) {
        lastBtn2 = newBtn2;
        length = 250;
      };
      lengthBar = ((length) / 14);
      oled.fillRect(statusWidth, statusBary1, statusWidth, statusBary2, BLACK);
      printAlignC(String(length), statusBary1);
      oled.display();  // show on OLED
    };
    // encoder 3
    if (newClk3 != lastClk3 || newBtn3 != lastBtn3) {
      if (newClk3 != lastClk3) {
        knobInput(newClk3, lastClk3, dtValue3);
        span = (span) + var1;
      };
      if (newBtn3 != lastBtn3) {
        lastBtn3 = newBtn3;
        span = 250;
      };
      spanBar = ((span) / 8);
      oled.fillRect(statusWidth * 2, statusBary1, statusWidth, statusBary2, BLACK);
      printAlignR(String(span), statusBary1);
      oled.display();  // show on OLED
    };
    // encoder 4
    oled.setTextSize(2);  // text size
    if (newClk4 != lastClk4 || newBtn4 != lastBtn4) {
      if (newClk4 != lastClk4) {
        knobInput(newClk4, lastClk4, dtValue4);
        countDown = (countDown) + ((var1) / 10);
        countLast = (countDown);
      };
      if (newBtn4 != lastBtn4) {
        lastBtn4 = newBtn4;
        knobPush(newBtn4, lastBtn4);
        run = (run) * (var2);
      };
      oled.fillRect(OLED_WIDTH / 2 - 15, OLED_HEIGHT / 2 - 15, 30, 30, BLACK);
      printCenter(String(countDown - 1));  // text to display
      oled.display();                      // show on OLED
    };
  }
}

void knobInput(int newClk, int lastClk, int dtValue) {
  {
    lastClk = newClk;
    if (newClk == LOW && dtValue == HIGH) {
      var1 = 10;
    }
    if (newClk == LOW && dtValue == LOW) {
      var1 = -10;
    }
  }
}

void knobPush(int newBtn, int lastBtn) {
  if (newBtn == LOW) {
    var2 = 1;
  }
  if (newBtn == HIGH) {
    var2 = -1;
  }
}

void printAlignL(String text, int heightY) {
  oled.setCursor(0, heightY);
  oled.println(text);  // text to display
}

void printAlignR(String text, int heightY) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;
  oled.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  oled.setCursor((OLED_WIDTH - width), heightY);
  oled.println(text);  // text to display
}

void printAlignC(String text, int heightY) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;
  oled.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  oled.setCursor((OLED_WIDTH - width) / 2, heightY);
  oled.println(text);  // text to display
}

void printCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;
  oled.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  // display on horizontal and vertical center
  oled.setCursor((OLED_WIDTH - width) / 2, (OLED_HEIGHT - height) / 2);
  oled.println(text);  // text to display
}

void reduceCount() {
  countDown = (countDown)-1;
  oled.fillRect(OLED_WIDTH / 2 - 15, OLED_HEIGHT / 2 - 15, 30, 30, BLACK);
  oled.setTextSize(2);                 // text size
  printCenter(String(countDown - 1));  // text to display
}
