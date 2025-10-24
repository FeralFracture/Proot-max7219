#include <Led_Controller.h>
#include <Animation_Controller.h>
#include <Adafruit_NeoPixel.h>
#include <Proto_Face_Frames.h>
// LED Variables
const uint8_t DIN = 2,
              CS = 3,
              CLK = 4,
              MODULE_COUNT = 14,
              INTENSITY = 4,
              STRIP_A = 5,
              STRIP_B = 6,
              FAN_A = 7,
              FAN_B = 8;

Adafruit_NeoPixel strip_A(20, STRIP_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_B(20, STRIP_B, NEO_GRB + NEO_KHZ800);

int countdown = 0;
unsigned long lastUpdate = 0,
              delayVal = 1;
Led_Controller lcp(DIN, CS, CLK, MODULE_COUNT, INTENSITY, false);
EyeAnimator blink(lcp, default_eye_blink, 5);
void setup()
{
  delay(500);
  pinMode(FAN_A, OUTPUT);
  pinMode(FAN_B, OUTPUT);
  Serial.begin(9600);
  analogWrite(FAN_A, 255);
  analogWrite(FAN_B, 255);
  lcp.reset();
  delay(200);
  randomSeed(analogRead(A0) * micros());

  strip_A.begin();
  strip_A.fill(strip_A.Color(255, 0, 0));
  strip_A.show();
  strip_A.setBrightness(40 /*255*/);
  strip_B.begin();
  strip_B.fill(strip_B.Color(255, 0, 0));
  strip_B.show();

  strip_B.setBrightness(40 /*255*/);
  countdown = 0;
  blink.setFrameDelay(1);
}
void loop()
{

  blink.update();

  if (millis() - lastUpdate < delayVal)
    return;

  lastUpdate = millis();
  countdown--;

  if (countdown < 0)
  {
    blink.startAnim();
    countdown = random(7500,22500);
  }
}