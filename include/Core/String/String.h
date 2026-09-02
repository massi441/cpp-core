#pragma once

#include "Core/Container/Array.h"
#include "Core/String/StringUtil.h"

#include <cstring>
#include <string_view>

namespace ml {

/**
 * A wrapper around a heap allocated C-style string
 */
class String {
public:
    String() = default;

    explicit String(const char* str, uint64_t strLen) {
        mBuf = ml::Array<char>(strLen + 1);
        std::memcpy(mBuf.data(), str, strLen);

        this->terminate();
    }

    /**
     * Creates a string from a source, up to a charater delimiter
     * @param source The source string
     * @param stopDelimiter The exclusive stop delimiter on the source string
     */
    explicit String(const char* source, char stopDelimiter) {
        int strLen = ml::delimiterOffset(source, stopDelimiter);

        if (strLen == -1) {
            mBuf = ml::Array<char>::Empty;
            return;
        }

        mBuf = ml::Array<char>(strLen + 1); // delimiter gets replaced by '\0'
        ml::strdcpy(mBuf.data(), source, stopDelimiter);
    }

    template <typename... Args>
    requires (std::same_as<Args, const char*>&& ...)
    explicit String(Args... strings) {
        size_t strlen = (std::strlen(strings) + ...);
        mBuf = ml::Array<char>(strlen + 1);

        char* ptr = mBuf.data();
        ((ptr = ml::stpcpy(ptr, strings)), ...);

        this->terminate();
    }

    String(const char* str) : String(str, strlen(str)) {

    }

    String(std::string_view strView) : String(strView.data(), strView.size()) {

    }

    bool operator==(const String& other) const {
        return std::strcmp(this->c_str(), other.c_str()) == 0;
    }

    bool operator==(const char* other) const {
        return std::strcmp(this->c_str(), other) == 0;
    }

    const char* c_str() const { return mBuf.cdata(); }
    uint64_t length() const { return mBuf.size() - 1; }
    operator std::string_view() const { return std::string_view(this->c_str(), length()); }
    operator const char*() const { return this->c_str(); }

private:
    ml::Array<char> mBuf; // potentially use raw char* instead to avoid - 1 on length

    void terminate() {
        mBuf.last() = '\0';
    }
};

}
