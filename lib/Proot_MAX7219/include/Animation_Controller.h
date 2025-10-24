#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H
#include <Led_Controller.h>
#include <progmem_reader.h>
#include <Arduino.h>

class AnimationBase
{
protected:
    uint8_t frameCount = 0;

    int8_t currentFrame = -1;

    unsigned long lastUpdate = 0;
    uint16_t frameInterval = 100;
    
    Led_Controller &led_controller;
    const uint64_t *frameData;


    virtual void animate() = 0;
    virtual void reset() = 0;

    bool bounce = false,
         bouncing = false;

public:
    AnimationBase(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval = 100, bool bounce = false);
    virtual ~AnimationBase() = default;
    void update();
    void startAnim();
    void setFrameInterval(uint16_t interval);
    void setFrameData(const uint64_t *newFrameData, uint8_t newFrameCount, bool bounce = false);
};

class EyeAnimator : public AnimationBase
{
protected:
    void animate() override;
    void reset() override;

public:
    EyeAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval = 100, bool bounce = false);
};

class MouthAnimator : public AnimationBase
{
protected:
    void animate() override;
    void reset() override;

public:
    MouthAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval = 100, bool bounce = false);
};
class NoseAnimator : public AnimationBase
{
protected:
    void animate() override;
    void reset() override;

public:
    NoseAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval = 100, bool bounce = false);
};
#endif