#pragma once

#include <typeinfo>
#include <unordered_map>

namespace ml {

template <typename TKey, typename TValue>
bool tryRemove(TValue* outValue, std::unordered_map<TKey, TValue>& map, const TKey& key) {
    auto it = map.find(key);

    if (it == map.end()) {
        return false;
    }

    *outValue = std::move(it->second);
    map.erase(it);

    return true;
}

template <typename T, typename ...Args>
T* placementNew(void* memory, Args&& ...args) {
    return new (memory) T(std::forward<Args>(args)...);
}

/**
 * Checks if an object is derived from another class
 */
template <typename B, typename D>
bool instanceOf(B* base) {
    return dynamic_cast<D*>(base) != nullptr;
}

template <typename B, typename D>
bool instanceOf(const B* base) {
    return dynamic_cast<const D*>(base) != nullptr;
}

template <typename B, typename D>
bool tryCast(B* base, D** cast) {
    *cast = dynamic_cast<D*>(base);
    return *cast != nullptr;
}

template <typename B, typename D, typename A>
bool ifInstanceOf(B* base, A action) {
    D* derived = dynamic_cast<D*>(base);
    if (derived) {
        action(derived);
        return true;
    }

    return false;
}

template <typename T>
const char* getTypeId(const T* ptr) {
    return typeid(*ptr).name();
}

}
