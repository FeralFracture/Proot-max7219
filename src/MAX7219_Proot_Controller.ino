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
              BUTTON_3 = 13,

              FAN_SPEEDS[] = {255, 215, 180, 0};

uint8_t fan_speed_1 = 0,
        fan_speed_2 = 0,
        face_selection = 0;

long blinkCountdown = 0;
int16_t buttonCooldown = 0;
float delay_multiplier = 1;
unsigned long lastUpdate = 0,
              delayVal = 1;

Adafruit_NeoPixel Left_Panel(20, STRIP_L_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Right_Panel(20, STRIP_R_PIN, NEO_GRB + NEO_KHZ800);
Led_Controller lcp(DIN_PIN, CS_PIN, CLK_PIN, MODULE_COUNT, INTENSITY, false);
EyeAnimator eyeAnim(lcp, default_eye_blink, 5);
MouthAnimator mouthController(lcp, nullptr);

void setup()
{
  randomSeed(analogRead(A0) * micros());
  Serial.begin(9600);
  Serial.print("Initializing pin modes");
  Serial.print(".");
  pinMode(FAN_A_PIN, OUTPUT);
  Serial.print(".");
  pinMode(FAN_B_PIN, OUTPUT);
  Serial.print(".");
  pinMode(BUTTON_1, INPUT_PULLUP);
  Serial.print(".");
  pinMode(BUTTON_2, INPUT_PULLUP);
  Serial.println(".");
  pinMode(BUTTON_3, INPUT_PULLUP);
  Serial.print("Completed pin setup. \nDelaying for power startup");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("\nInitializing Fans and displays");
  Serial.print(".");
  analogWrite(FAN_A_PIN, FAN_SPEEDS[fan_speed_1]);
  Serial.print(".");
  analogWrite(FAN_B_PIN, FAN_SPEEDS[fan_speed_2]);
  Serial.print(".");
  lcp.reset();
  Serial.print(".");
  Left_Panel.begin();
  Serial.print(".");
  Left_Panel.fill(Left_Panel.Color(255, 0, 0));
  Serial.print(".");
  Left_Panel.show();
  Serial.print(".");
  Left_Panel.setBrightness(225);
  Serial.print(".");
  Right_Panel.begin();
  Serial.print(".");
  Right_Panel.fill(Right_Panel.Color(255, 0, 0));
  Serial.print(".");
  Right_Panel.show();
  Serial.print(".");
  Right_Panel.setBrightness(225);
  Serial.println(".");
  Serial.println("Completed Initialization. \nEntering loop.");
}
void loop()
{
  if (millis() - lastUpdate < delayVal)
    return;
  lastUpdate = millis();
  //Serial.println(blinkCountdown);
  if (buttonCooldown > 0)
  {
    buttonCooldown--;
  }
  if (buttonCooldown <= 0)
  {
    int buttonData[] = {
        digitalRead(BUTTON_1),
        digitalRead(BUTTON_2),
        digitalRead(BUTTON_3)};
    if (buttonData[0] == LOW || buttonData[1] == LOW || buttonData[2] == LOW)
    {
      buttonCooldown = 700;
      if (buttonData[0] == LOW)
      {
        // Serial.println("Fan 1 press");
        fan_speed_1++;
        if (fan_speed_1 >= sizeof(FAN_SPEEDS))
        {
          fan_speed_1 = 0;
        }
        analogWrite(FAN_A_PIN, FAN_SPEEDS[fan_speed_1]);
        // Serial.print("Fan 1: ");
        // Serial.println(FAN_SPEEDS[fan_speed_1]);
      }
      else if (buttonData[1] == LOW)
      {
        // Serial.println("Fan press 2");
        fan_speed_2++;
        if (fan_speed_2 >= sizeof(FAN_SPEEDS))
        {
          fan_speed_2 = 0;
        }
        analogWrite(FAN_B_PIN, FAN_SPEEDS[fan_speed_2]);
        // Serial.print("Fan 2: ");
        // Serial.println(FAN_SPEEDS[fan_speed_2]);
      }
      else if (buttonData[2] == LOW)
      {
        // Serial.println("Face Button Press");
        ++face_selection %= 5;
        switch (face_selection)
        {
        case 1:
          eyeAnim.setFrameData(happy_eye_blink, 6, true, false);
          eyeAnim.setFrameInterval(5);
          mouthController.setFrameData(smile_mouth, 1, false, false);
          mouthController.startAnim();
          delay_multiplier = 1;
          break;
        case 2:
          eyeAnim.setFrameData(focused_eye_blink, 6, true, false);
          eyeAnim.setFrameInterval(10);
          mouthController.setFrameData(closed_mouth, 1, false, false);
          mouthController.startAnim();
          delay_multiplier = 1.5;
          break;
        case 3:
          eyeAnim.setFrameData(glare_eye_blink, 6, true, false);
          eyeAnim.setFrameInterval(20);
          mouthController.setFrameData(persed_mouth, 1, false, false);
          mouthController.startAnim();
          delay_multiplier = 2;
          break;
        case 4:
          eyeAnim.setFrameData(x_eye_blink, 12, false, false, 3);
          eyeAnim.setFrameInterval(1);
          mouthController.setFrameData(dead_mouth, 1, false, false);
          mouthController.startAnim();
          delay_multiplier = 3.1;
          break;
        default:
          eyeAnim.setFrameData(default_eye_blink, 5, true, true);
          eyeAnim.setFrameInterval(1);
          mouthController.setFrameData(dead_mouth);
          mouthController.startAnim();
          delay_multiplier = 1;
        }
        blinkCountdown = -1;
      }
    }
  }
  if (!eyeAnim.isAnimating())
  {
    blinkCountdown--;
    if (blinkCountdown < 0)
    {
      Left_Panel.setBrightness(225);
      Right_Panel.setBrightness(225);
      eyeAnim.startAnim();
      uint8_t chance = random(0, 100);
      if (chance <= 65)
      {
        blinkCountdown = random(2800, 4000) * delay_multiplier;
      }
      else
      {
        chance = random(0, 100);
        if (chance <= 60)
        {
          blinkCountdown = random(6500, 20000) * delay_multiplier;
        }

        else
        {
          blinkCountdown = random(300, 900) * delay_multiplier;
        }
      }
    }
  }

  eyeAnim.update();
  mouthController.update();
}