#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H
#include <Led_Controller.h>
#include <progmem_reader.h>
#include <Arduino.h>

class AnimationBase
{
protected:
    Led_Controller &led_controller;
    const uint64_t *frameData;
    uint8_t frameCount = 0;
    uint16_t frameInterval = 100;
    bool bounce = false,
         useDefaultIdle = true;
    uint8_t loop_times = 0;

    int8_t currentFrame = -1,
           loop_i = 0;
    unsigned long lastUpdate = 0;

    virtual void animate() = 0;
    virtual void reset() = 0;

    bool bouncing = false,
         isAnimatingBool = false;

public:
    AnimationBase(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval = 100, bool bounce = false, bool useDefaultIdle = true, uint8_t loop_times = 0);
    virtual ~AnimationBase() = default;
    void update();
    void startAnim();
    bool isAnimating();
    void setFrameInterval(uint16_t interval);
    void setFrameData(const uint64_t *newFrameData, uint8_t newFrameCount = 1, bool bounce = false, bool useDefaultIdle = true, uint8_t loop_times = 0);
};

class EyeAnimator : public AnimationBase
{
protected:
    void animate() override;
    void reset() override;

public:
    EyeAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount = 1, uint16_t frameInterval = 1, bool bounce = true, bool useDefaultIdle = true, uint8_t loop_times = 0);
};

class MouthAnimator : public AnimationBase
{
protected:
    void animate() override;
    void reset() override;

public:
    MouthAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount = 1, uint16_t frameInterval = 1, bool bounce = false, bool useDefaultIdle = true, uint8_t loop_times = 0);
};
class NoseAnimator : public AnimationBase
{
protected:
    void animate() override;
    void reset() override;

public:
    NoseAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount = 1, uint16_t frameInterval = 1, bool bounce = false, bool useDefaultIdle = true, uint8_t loop_times = 0);
};
#endif