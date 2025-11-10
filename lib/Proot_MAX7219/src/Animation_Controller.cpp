#include "Animation_Controller.h"

AnimationBase::AnimationBase(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce)
    : led_controller(controller), frameData(frameData), frameCount(frameCount), frameInterval(frameInterval), bounce(bounce) {}

void AnimationBase::startAnim()
{
    this->currentFrame = 0;
}

void AnimationBase::setFrameInterval(uint16_t interval)
{
    this->frameInterval = interval;
}

void AnimationBase::setFrameData(const uint64_t *newFrameData, uint8_t newFrameCount, bool bounce)
{
    frameData = newFrameData;
    frameCount = newFrameCount;
    this->bounce = bounce;
}

void AnimationBase::update()
{
    if (millis() - this->lastUpdate < this->frameInterval)
    {
        return;
    }

    this->lastUpdate = millis();
    
    if (this->currentFrame == -1)
    {
        return;
    }

    if ((this->bounce && this->currentFrame >= this->frameCount) || this->bouncing)
    {
        this->currentFrame--;
        if (this->currentFrame < 0)
        {
            this->bouncing = false;
            reset();
            return;
        }
        this->bouncing = true;
        animate();
        return;
    }
    else if (this->currentFrame >= this->frameCount)
    {
        this->bouncing = false;
        this->currentFrame = -1;
        reset();
        return;
    }
    animate();
    currentFrame++;
}

EyeAnimator::EyeAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce) : AnimationBase(controller, frameData, frameCount, frameInterval, bounce)
{
}
MouthAnimator::MouthAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce) : AnimationBase(controller, frameData, frameCount, frameInterval, bounce)
{
}
NoseAnimator::NoseAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce) : AnimationBase(controller, frameData, frameCount, frameInterval, bounce)
{
}
void EyeAnimator::animate()
{
    led_controller.setModule(0, FRAME(currentFrame * 2 + 0, frameData));
    led_controller.setModule(1, FRAME(currentFrame * 2 + 1, frameData));
    led_controller.setModule(13, FRAME(currentFrame * 2 + 0, frameData), true);
    led_controller.setModule(12, FRAME(currentFrame * 2 + 1, frameData), true);
    led_controller.latchDisplays();
}

void EyeAnimator::reset()
{
    led_controller.setModule(0, EYE, 0);
    led_controller.setModule(1, EYE, 1);
    led_controller.setModule(12, EYE, 1, true);
    led_controller.setModule(13, EYE, 0, true);
    led_controller.latchDisplays();
}
