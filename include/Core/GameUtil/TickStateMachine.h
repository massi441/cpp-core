#pragma once

#include <utility>

#include "Core/Util/Types.h"

namespace ml {

template <typename T, typename ...Args>
class TickStateMachine {
public:
    using StateFunc = void(T::*)(Args ...args);

    TickStateMachine() = default;

    explicit TickStateMachine(StateFunc initialState) {
        mCurrentFunc = initialState;
    }

    template <typename ...F>
    void update(T* thisPtr, F&& ...args) {
        (thisPtr->*mCurrentFunc)(std::forward<F>(args) ...);

        if (!mIsStateChanged) {
            mTick++;
        } else {
            mIsStateChanged = false;
        }
    }

    void setState(StateFunc newState) {
        mCurrentFunc = newState;
        mIsStateChanged = true;
        mTick = 0;
    }

    bool isState(StateFunc stateFunc) const {
        return stateFunc == mCurrentFunc;
    }

    uint getTick() const {
        return mTick;
    }

    bool isFirstTick() const {
        return mTick == 0;
    }

    bool isTick(uint tick) const {
        return mTick == tick;
    }

    bool isOverTick(uint tickCount) const {
        return mTick > tickCount;
    }

    bool isOverEqualTick(uint tickCount) const {
        return mTick >= tickCount;
    }

private:
    StateFunc mCurrentFunc;
    uint mTick = 0;
    bool mIsStateChanged = false;
};

// TODO: Remove macro

#define USE_TICK_STATE_MACHINE(T, ...)                                                          \
    private:                                                                                    \
        ml::TickStateMachine<T __VA_OPT__(,) __VA_ARGS__> mStateMachine;                        \
                                                                                                \
    public:                                                                                     \
        ml::TickStateMachine<T __VA_OPT__(,) __VA_ARGS__>* getTickStateMachine() {              \
            return &mStateMachine;                                                              \
        }                                                                                       \
                                                                                                \
        const ml::TickStateMachine<T __VA_OPT__(,) __VA_ARGS__>* getTickStateMachine() const {  \
            return &mStateMachine;                                                              \
        }                                                                                       \

template <typename T, typename ...Args>
void updateState(T* t, Args&& ...args) {
    t->getTickStateMachine()->update(t, std::forward<Args>(args)...);
}

template <typename T, typename StateFunc>
void setState(T* t, StateFunc newState) {
    t->getTickStateMachine()->setState(newState);
}

template <typename T, typename StateFunc>
bool isState(const T* t, StateFunc stateFunc) {
    return t->getTickStateMachine()->isState(stateFunc);
}

template <typename T>
uint getTick(const T* t) {
    return t->getTickStateMachine()->getTick();
}

template <typename T>
bool isFirstTick(const T* t) {
    return t->getTickStateMachine()->isFirstTick();
}

template <typename T>
bool isTick(const T* t, uint tick) {
    return t->getTickStateMachine()->isTick(tick);
}

template <typename T>
bool isOverTick(const T* t, uint tickCount) {
    return t->getTickStateMachine()->isOverTick(tickCount);
}

template <typename T>
bool isOverEqualTick(const T* t, uint tickCount) {
    return t->getTickStateMachine()->isOverEqualTick(tickCount);
}

}
