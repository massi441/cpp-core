#pragma once

#include <cstdint>
#include <type_traits>

namespace ml {

// TODO: Indexable container subtype

template <typename TContainer, typename T>
class Container {
public:
    T* find(const T& item) {
        for (T& it : this->self()) {
            if (it == item) {
                return &it;
            }
        }

        return nullptr;
    }

    template <typename F>
    T* findIf(const F& predicate) {
        for (T& it : this->self()) {
            if (predicate(it)) {
                return &it;
            }
        }

        return nullptr;
    }

    const T* find(const T& item) const {
        for (const T& it : this->self()) {
            if (it == item) {
                return &it;
            }
        }

        return nullptr;
    }

    template <typename F>
    const T* findIf(const F& predicate) const {
        for (const T& it : this->self()) {
            if (predicate(it)) {
                return &it;
            }
        }

        return nullptr;
    }

    template <typename F>
    T* findIf(const F& predicate, uint64_t start, uint64_t count) {
        T* it = this->self().begin() + start;
        T* last = it + count;

        while (it != last) {
            if (predicate(*it)) {
                return it;
            }

            ++it;
        }

        return nullptr;
    }

    template <typename F>
    const T* findIf(const F& predicate, uint64_t start, uint64_t count) const {
        const T* it = this->self().begin() + start;
        const T* last = it + count;

        while (it != last) {
            if (predicate(*it)) {
                return it;
            }

            ++it;
        }

        return nullptr;
    }

    template <typename B>
    requires std::is_convertible_v<T, B>
    bool contains(const B& item) const {
        for (const T& it : this->self()) {
            if (it == item) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks if an item is contained within the array using == comparison
     * @param item The item to check for
     * @return true if the item is inside the array, false otherwise
     */
    bool contains(const T& item) const {
        return this->template contains<T>(item);
    }

    template <typename F>
    bool containsIf(const F& predicate) const {
        for (const T& item : this->self()) {
            if (predicate(item)) {
                return true;
            }
        }

        return false;
    }

    template <typename F>
    bool containsIf(const F& predicate, uint64_t start, uint64_t count) const {
        const T* it = this->self().begin() + start;
        const T* last = it + count;

        while (it != last) {
            if (predicate(*it)) {
                return true;
            }

            ++it;
        }

        return false;
    }

    /**
     * Computes the index of an item within the array, using == comparison
     * @param item The item to compute the index of
     * @return The index of the item if it is found, UINT64_MAX otherwise
     */
    uint64_t indexOf(const T& item) const {
        const T* begin = this->self().begin();
        const T* end = this->self().end();

        for (const T* it = begin; it != end; ++it) {
            if (*it == item) {
                return it - begin;
            }
        }

        return TContainer::InvalidIndex;
    }

    /**
     * Replaces the first occurrence of the target item with the provided value
     * @param target The target item to replace
     * @param value The value to replace the target with
     * @return True if the target was replaced, false otherwise
     */
    bool replaceFirst(const T& target, const T& value) {
        for (T& item : this->self()) {
            if (item == target) {
                item = value;
                return true;
            }
        }

        return false;
    }

protected:
    Container() = default;

private:
    TContainer& self() { return *static_cast<TContainer*>(this); }
    const TContainer& self() const { return *static_cast<const TContainer*>(this); }
};

}
