#include <Led_Controller.h>
#include <Animation_Controller.h>
#include <Adafruit_NeoPixel.h>
#include <Proto_Face_Frames.h>

const uint8_t DIN_PIN = 2,
              CS_PIN = 3,
              CLK = 4,
              MODULE_COUNT = 14,
              INTENSITY = 4,
              STRIP_L_PIN = 5,
              STRIP_R_PIN = 6,
              FAN_A_PIN = 7,
              FAN_B_PIN = 8;

Adafruit_NeoPixel Left_Panel(20, STRIP_L_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Right_Panel(20, STRIP_R_PIN, NEO_GRB + NEO_KHZ800);

int blinkCountdown = 0;
unsigned long lastUpdate = 0,
              delayVal = 1;
Led_Controller lcp(DIN_PIN, CS_PIN, CLK, MODULE_COUNT, INTENSITY, false);
EyeAnimator blink(lcp, default_eye_blink, 5, 1, true);
void setup()
{
  delay(500);
  pinMode(FAN_A_PIN, OUTPUT);
  pinMode(FAN_B_PIN, OUTPUT);
  Serial.begin(9600);
  analogWrite(FAN_A_PIN, 255);
  analogWrite(FAN_B_PIN, 255);
  lcp.reset();
  delay(200);
  randomSeed(analogRead(A0) * micros());

  Left_Panel.begin();
  Left_Panel.fill(Left_Panel.Color(255, 0, 0));
  Left_Panel.show();
  Left_Panel.setBrightness(225);

  Right_Panel.begin();
  Right_Panel.fill(Right_Panel.Color(255, 0, 0));
  Right_Panel.show();
  Right_Panel.setBrightness(225);
}
void loop()
{
  if (millis() - lastUpdate < delayVal)
    return;
  // Serial.println(blinkCountdown);
  lastUpdate = millis();
  Serial.print(".");
  blinkCountdown--;

  if (blinkCountdown < 0)
  {
    Left_Panel.setBrightness(225);
    Right_Panel.setBrightness(225);
    blink.startAnim();
    uint8_t chance = random(0, 100);
    if (chance <= 65)
    {
      Serial.println("\n Reg Blink");
      blinkCountdown = random(2800, 4000);
      Serial.println(blinkCountdown);
    }
    else
    {
      chance = random(0, 100);
      if (chance <= 60)
      {
        Serial.println("\n Long Blink");
        blinkCountdown = random(6500, 20000);
        Serial.println(blinkCountdown);
      }

      else
      {
        Serial.println("\n Short Blink");
        blinkCountdown = random(300, 900);
        Serial.println(blinkCountdown);
      }
    }

    // Serial.println("\n\n BLINK! \n\n");
  }
  blink.update();
}