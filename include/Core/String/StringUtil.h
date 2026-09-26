#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace ml {

inline constexpr const char* EmptyString = "";

inline bool streql(const char* s1, const char* s2) {
    return strcmp(s1, s2) == 0;
}

inline char intAsChar(int num) {
    return static_cast<char>('0' + num);
}

inline char* stpcpy(char* dst, const char* src) {
#ifdef WIN32
    size_t len = std::strlen(src);
    std::memcpy(dst, src, len + 1);
    return dst + len;
#else
    return ::stpcpy(dst, src);
#endif
}

/**
 * Copies and null terminates a string up to an exclusive delimiter character.
 * If no delimiter is found, the entire source string is copied into the destination
 * @param dest The destination string
 * @param source The source string
 * @param delimiter The exclusive delimiter character
 */
inline void strdcpy(char* dest, const char* source, char delimiter) {
    while (*source != delimiter && *source != '\0') {
        *dest++ = *source++;
    }

    *dest = '\0';
}

inline ptrdiff_t delimiterOffset(const char* source, char delimiter) {
    const char* delimPtr = std::strchr(source, delimiter);

    if (delimPtr == nullptr) {
        return -1;
    }

    return delimPtr - source;
}

inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> symbols;
    std::stringstream stream = std::stringstream(str);
    std::string buffer;

    while (std::getline(stream, buffer, delimiter)) {
        symbols.push_back(buffer);
    }

    return symbols;
}

template <typename ...Args>
requires (std::same_as<Args, const char*>&& ...)
std::string concatString(Args... strings) {
    size_t strlen = (std::strlen(strings) + ...);
    std::string str;
    str.resize(strlen + 1);

    char* ptr = str.data();
    ((ptr = ml::stpcpy(ptr, strings)), ...);

    return str;
}

inline const char* toString(bool boolean) {
    return boolean ? "true" : "false";
}

consteval char nullTerminator() {
    return '\0';
}

// TODO: Add case insensitive comparison
// TODO: Add trimming functions

// inline bool endsWith(const char* source, char end) {
//     return false;
// }

}
