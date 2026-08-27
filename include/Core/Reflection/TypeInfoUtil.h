#pragma once

#include <typeinfo>

namespace ml {

template <typename T>
const char* getTypeId(const T* ptr) {
    return typeid(*ptr).name();
}

}
