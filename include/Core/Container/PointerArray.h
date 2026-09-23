#pragma once

#include <memory>

#include "Core/Container/Array.h"
#include "Core/Util/Macros.h"

namespace ml {


/**
 * A fixed-size array of owned pointers, where each pointer is automatically freed during destruction
 * @tparam T The type of object pointed to by the pointers in the array
 * @tparam D The deleter type, defaults to std::default_delete<T>
 */
template <typename T, typename D = std::default_delete<T>>
requires (!std::is_pointer_v<T>)
class PointerArray final : public ml::Container<PointerArray<T, D>, T*> {
    NO_COPY(PointerArray)
public:
    static constexpr uint64_t InvalidIndex = ml::Array<T>::InvalidIndex;

    explicit PointerArray() = default;

    explicit PointerArray(uint64_t size) : mArray(size), mDeleter(D()) {

    }

    explicit PointerArray(uint64_t size, const D& deleter)
        : mArray(size), mDeleter(deleter)
    {

    }

    explicit PointerArray(uint64_t size, D&& deleter)
        : mArray(size), mDeleter(std::move(deleter))
    {

    }

    explicit PointerArray(std::vector<T*>&& vector, D&& deleter = D())
        : mArray(std::move(vector)), mDeleter(std::move(deleter))
    {

    }

    PointerArray(std::initializer_list<T*> list, D&& deleter = D())
        : mArray(list), mDeleter(std::move(deleter))
    {

    }

    PointerArray(PointerArray&& other) noexcept {
        mArray = std::move(other.mArray);
        mDeleter = std::move(other.mDeleter);
    }

    PointerArray& operator=(PointerArray&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        mArray = std::move(other.mArray);
        mDeleter = std::move(other.mDeleter);

        return *this;
    }

    void set(uint64_t index, T* ptr) {
        this->free(mArray[index]);
        mArray[index] = ptr;
    }

    template <typename ...Args>
    void emplace(uint64_t index, Args&&... args) {
        this->free(mArray[index]);
        mArray[index] = new T(std::forward<Args>(args)...);
    }

    void emplaceNew(uint64_t index) requires (std::is_default_constructible_v<T>) {
        this->set(index, new T());
    }

    bool replace(T* oldPtr, T* newPtr) {
        if (oldPtr == newPtr) {
            return true;
        }

        for (T*& ptr : mArray) {
            if (ptr == oldPtr) {
                this->free(oldPtr);
                ptr = newPtr;
                return true;
            }
        }

        return false;
    }

    void remove(uint64_t index) {
        this->free(mArray[index]);
        mArray[index] = nullptr;
    }

    bool remove(T* item) {
        for (T*& ptr : mArray) {
            if (ptr == item) {
                this->free(ptr);
                ptr = nullptr;
                return true;
            }
        }

        return false;
    }

    std::unique_ptr<T, D> release(uint64_t index) {
        T* item = mArray[index];
        mArray[index] = nullptr;
        return std::unique_ptr<T, D>(item, mDeleter);
    }

    std::unique_ptr<T, D> release(T* item) {
        for (T*& ptr : mArray) {
            if (ptr == item) {
                ptr = nullptr;
                return std::unique_ptr<T, D>(item, mDeleter);
            }
        }

        return std::unique_ptr<T, D>(nullptr, mDeleter);
    }

    ~PointerArray() {
        for (T* ptr : mArray) {
            this->free(ptr);
        }
    }

    bool isInBounds(uint64_t index) const {
        return mArray.isInBounds(index);
    }

    T* safeAt(uint64_t index) {
        if (index >= mArray.size()) {
            return nullptr;
        }

        return mArray[index];
    }

    T** begin() { return mArray.begin(); }
    T** end() { return mArray.end(); }
    T* const* begin() const { return mArray.begin(); }
    T* const* end() const { return mArray.end(); }

    uint64_t size() const { return mArray.size(); }

    T* operator[](uint64_t index) { return mArray[index]; }
    const T* operator[](uint64_t index) const { return mArray[index]; }

private:
    ml::Array<T*> mArray;
    D mDeleter;

    void free(T* ptr) const {
        mDeleter(ptr);
    }
};

}
