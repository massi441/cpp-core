#pragma once

namespace ml {

class Timer {
public:
    Timer() = default;

    Timer(int time) {
        mTime = time;
    }

    void start(int time) {
        if (time < 0) {
            this->disable();
            return;
        }

        mTime = time;
    }

    /**
     * Decrements the timer and returns true if the timer is done.
     * @return True if the timer has expired, false otherwise
     */
    bool update() {
        if (mTime > 0) {
            mTime--;
        }

        return this->isDone();
    }

    void stop() {
        mTime = 0;
    }

    void enable() {
        mTime = 0;
    }

    void disable() {
        mTime = -1;
    }

    bool isActive() const {
        return mTime > 0;
    }

    bool isDone() const {
        return mTime == 0;
    }

    bool isEnabled() const {
        return mTime != -1;
    }

    bool isDisabled() const {
        return mTime == -1;
    }

    int getTime() const {
        return mTime;
    }

private:
    int mTime = 0;
};

}
