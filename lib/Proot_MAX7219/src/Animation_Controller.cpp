#include "Animation_Controller.h"

AnimationBase::AnimationBase(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce, bool useDefaultIdle, uint8_t loop_times)
    : led_controller(controller), frameData(frameData), frameCount(frameCount), frameInterval(frameInterval), bounce(bounce), useDefaultIdle(useDefaultIdle), loop_times(loop_times) {}
bool AnimationBase::isAnimating()
{
    return this->isAnimatingBool;
}
void AnimationBase::startAnim()
{

    if (useDefaultIdle)
    {
        this->currentFrame = 0;
    }
    else
    {
        this->currentFrame = 1;
    }
}

void AnimationBase::setFrameInterval(uint16_t interval)
{
    this->frameInterval = interval;
}

void AnimationBase::setFrameData(const uint64_t *newFrameData, uint8_t newFrameCount, bool bounce, bool useDefaultIdle, uint8_t loop_times)
{
    frameData = newFrameData;
    frameCount = newFrameCount;
    this->bounce = bounce;
    this->useDefaultIdle = useDefaultIdle;
    this->loop_times = loop_times;
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
        isAnimatingBool = false;
        return;
    }
    isAnimatingBool = true;
    if ((this->bounce && this->currentFrame >= this->frameCount) || this->bouncing)
    {
        this->currentFrame--;
        if (this->currentFrame < 0)
        {
            this->bouncing = false;
            if (this->loop_i < this->loop_times)
            {
                this->loop_i++;
                this->currentFrame = 0;
                animate();
                this->currentFrame = 1;
                return;
            }
            this->loop_i = 0;
            if (this->useDefaultIdle)
            {
                reset();
            }
            else
            {
                this->currentFrame = 0;
                animate();
                this->currentFrame = -1;
            }
            return;
        }
        this->bouncing = true;
        animate();
        return;
    }
    else if (this->currentFrame >= this->frameCount)
    {
        this->bouncing = false;
        if (this->loop_i < this->loop_times)
        {
            this->loop_i++;
            this->currentFrame = 0;
            animate();
            this->currentFrame = 1;
            return;
        }
        this->loop_i = 0;
        if (this->useDefaultIdle)
        {
            this->currentFrame = -1;
            reset();
        }
        else
        {
            this->currentFrame = 0;
            animate();
            this->currentFrame = -1;
        }
        return;
    }
    animate();
    this->currentFrame++;
}

EyeAnimator::EyeAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce, bool useDefaultIdle, uint8_t loop_times) : AnimationBase(controller, frameData, frameCount, frameInterval, bounce, useDefaultIdle, loop_times)
{
}
MouthAnimator::MouthAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce, bool useDefaultIdle, uint8_t loop_times) : AnimationBase(controller, frameData, frameCount, frameInterval, bounce, useDefaultIdle, loop_times)
{
}
NoseAnimator::NoseAnimator(Led_Controller &controller, const uint64_t *frameData, uint8_t frameCount, uint16_t frameInterval, bool bounce, bool useDefaultIdle, uint8_t loop_times) : AnimationBase(controller, frameData, frameCount, frameInterval, bounce, useDefaultIdle, loop_times)
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

void MouthAnimator::animate()
{
    led_controller.setModule(2, FRAME(currentFrame * 4 + 0, frameData));
    led_controller.setModule(3, FRAME(currentFrame * 4 + 1, frameData));
    led_controller.setModule(4, FRAME(currentFrame * 4 + 2, frameData));
    led_controller.setModule(5, FRAME(currentFrame * 4 + 3, frameData));
    led_controller.setModule(11, FRAME(currentFrame * 4 + 0, frameData), true);
    led_controller.setModule(10, FRAME(currentFrame * 4 + 1, frameData), true);
    led_controller.setModule(9, FRAME(currentFrame * 4 + 2, frameData), true);
    led_controller.setModule(8, FRAME(currentFrame * 4 + 3, frameData), true);
    led_controller.latchDisplays();
}

void MouthAnimator::reset()
{
    led_controller.setModule(2, MOUTH, 0);
    led_controller.setModule(3, MOUTH, 1);
    led_controller.setModule(4, MOUTH, 2);
    led_controller.setModule(5, MOUTH, 3);
    led_controller.setModule(11, MOUTH, 0, true);
    led_controller.setModule(10, MOUTH, 1, true);
    led_controller.setModule(9, MOUTH, 2, true);
    led_controller.setModule(8, MOUTH, 3, true);
    led_controller.latchDisplays();
}
