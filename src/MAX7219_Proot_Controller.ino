#include <Led_Controller.h>
#include <Animation_Controller.h>
#include <Adafruit_NeoPixel.h>
#include <Proto_Face_Frames.h>

const uint8_t DIN_PIN = 2,
              CS_PIN = 3,
              CLK_PIN = 4,
              MODULE_COUNT = 14,
              INTENSITY = 4,

              STRIP_L_PIN = 5,
              STRIP_R_PIN = 6,

              FAN_A_PIN = 9,
              FAN_B_PIN = 10,

              BUTTON_1 = 7,
              BUTTON_2 = 8,

              FAN_SPEEDS[] = {255, 215, 180, 0};

uint8_t fan_speed_1 = 0,
        fan_speed_2 = 0,
        face_selection = 0;

int blinkCountdown = 0,
    buttonCooldown = 0;

float delay_multiplier = 1;
unsigned long lastUpdate = 0,
              delayVal = 1;

Adafruit_NeoPixel Left_Panel(20, STRIP_L_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Right_Panel(20, STRIP_R_PIN, NEO_GRB + NEO_KHZ800);
Led_Controller lcp(DIN_PIN, CS_PIN, CLK_PIN, MODULE_COUNT, INTENSITY, false);
EyeAnimator eyeAnim(lcp, default_eye_blink, 5, 1, true, true);
MouthAnimator mouthController(lcp, nullptr, 1, 1, false, false);

void setup()
{
  delay(500);
  pinMode(FAN_A_PIN, OUTPUT);
  pinMode(FAN_B_PIN, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  Serial.begin(9600);
  analogWrite(FAN_A_PIN, FAN_SPEEDS[fan_speed_1]);
  analogWrite(FAN_B_PIN, FAN_SPEEDS[fan_speed_2]);
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
  // Serial.print(".");
  blinkCountdown--;
  if (buttonCooldown > 0)
  {
    buttonCooldown--;
  }
  if (digitalRead(BUTTON_1) == LOW && buttonCooldown <= 0)
  {

    buttonCooldown = 700;
    fan_speed_1++;
    if (fan_speed_1 >= sizeof(FAN_SPEEDS))
    {
      fan_speed_1 = 0;
    }
    analogWrite(FAN_A_PIN, FAN_SPEEDS[fan_speed_1]);
    Serial.print("Fan 1: ");
    Serial.println(FAN_SPEEDS[fan_speed_1]);

    ++face_selection %= 5;
    switch (face_selection)
    {
    case 1:
      eyeAnim.setFrameData(happy_eye_blink, 6, true, false);
      eyeAnim.setFrameInterval(45);
      mouthController.setFrameData(smile_mouth, 1, false, false);
      mouthController.startAnim();
      delay_multiplier = 1;
      break;
    case 2:
      eyeAnim.setFrameData(focused_eye_blink, 6, true, false);
      eyeAnim.setFrameInterval(30);
      mouthController.setFrameData(closed_mouth, 1, false, false);
      mouthController.startAnim();
      delay_multiplier = 1.2;
      break;
    case 3:
      eyeAnim.setFrameData(glare_eye_blink, 6, true, false);
      eyeAnim.setFrameInterval(60);
      mouthController.setFrameData(persed_mouth, 1, false, false);
      mouthController.startAnim();
      delay_multiplier = 1.75;
      break;
    case 4:
      eyeAnim.setFrameData(x_eye_blink, 12, false, false);
      eyeAnim.setFrameInterval(150);
      mouthController.setFrameData(persed_mouth, 1, false, false);
      mouthController.startAnim();
      delay_multiplier = 2.1;
      break;
    default:
      eyeAnim.setFrameData(default_eye_blink, 5, true, true);
      eyeAnim.setFrameInterval(1);
      mouthController.setFrameData(smile_mouth, 0, false, true);
      mouthController.startAnim();
    }
    blinkCountdown = -1;
  }
  else if (digitalRead(BUTTON_2) == LOW && buttonCooldown <= 0)
  {

    buttonCooldown = 700;
    fan_speed_2++;
    if (fan_speed_2 >= sizeof(FAN_SPEEDS))
    {
      fan_speed_2 = 0;
    }
    analogWrite(FAN_B_PIN, FAN_SPEEDS[fan_speed_2]);
    Serial.print("Fan 2: ");
    Serial.println(FAN_SPEEDS[fan_speed_2]);
  }

  if (blinkCountdown < 0)
  {
    Left_Panel.setBrightness(225);
    Right_Panel.setBrightness(225);
    eyeAnim.startAnim();
    uint8_t chance = random(0, 100);
    if (chance <= 65)
    {
      // Serial.println("\n Reg Blink");
      blinkCountdown = random(2800, 4000) * delay_multiplier;
      // Serial.println(blinkCountdown);
    }
    else
    {
      chance = random(0, 100);
      if (chance <= 60)
      {
        // Serial.println("\n Long Blink");
        blinkCountdown = random(6500, 20000) * delay_multiplier;
        // Serial.println(blinkCountdown);
      }

      else
      {
        // Serial.println("\n Short Blink");
        blinkCountdown = random(300, 900) * delay_multiplier;
        // Serial.println(blinkCountdown);
      }
    }

    // Serial.println("\n\n BLINK! \n\n");
  }
  eyeAnim.update();
  mouthController.update();
}