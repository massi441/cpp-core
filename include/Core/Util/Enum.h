#pragma once

#include <cstdint>
#include <cstring>

#include "Core/Util/Recursion.h"

/**
 * A macro for generating an enum with a metadata struct, containing various utilities, including string conversions
 * Note: This only works with enums that are sequential and starting from 0
 * You are not expected to manually assign values to each enum and risk breaking the order.
 * @param EnumName The name of the enum
 * @param BaseType The underlying type of the enum (short, int...)
 */
#define DATA_TYPED_ENUM(EnumName, BaseType, ...)                                           \
    enum class EnumName : BaseType {                                                       \
        EXPAND(FOR_EACH_RECURSE(IDENT, __VA_ARGS__))                                       \
    };                                                                                     \
                                                                                           \
    struct EnumName##Data {                                                                \
        using Enum = EnumName;                                                             \
        static constexpr const char* Name = #EnumName;                                     \
                                                                                           \
        static constexpr const char* Strings[] = {                                         \
            EXPAND(FOR_EACH_RECURSE(STR, __VA_ARGS__))                                     \
        };                                                                                 \
                                                                                           \
        static constexpr BaseType Count = sizeof(Strings) / sizeof(Strings[0]);            \
                                                                                           \
        static constexpr const char* ToString(EnumName value) {                            \
            return Strings[static_cast<BaseType>(value)];                                  \
        }                                                                                  \
                                                                                           \
        static inline EnumName FromString(const char* name, EnumName defaultValue) {       \
            for (BaseType i = 0; i < Count; i++) {                                         \
                if (strcmp(ToString(static_cast<EnumName>(i)), name) == 0) {               \
                    return static_cast<EnumName>(i);                                       \
                }                                                                          \
            }                                                                              \
                                                                                           \
            return defaultValue;                                                           \
        }                                                                                  \
                                                                                           \
        static inline bool FromString(EnumName* outValue, const char* name) {              \
            for (BaseType i = 0; i < Count; i++) {                                         \
                if (strcmp(Strings[i], name) == 0) {                                       \
                    *outValue = static_cast<EnumName>(i);                                  \
                                                                                           \
                    return true;                                                           \
                }                                                                          \
            }                                                                              \
                                                                                           \
            return false;                                                                  \
        }                                                                                  \
                                                                                           \
        static inline EnumName FromStringSafe(const char* name, EnumName defaultValue) {   \
            if (name == nullptr) {                                                         \
                return defaultValue;                                                       \
            }                                                                              \
                                                                                           \
            return FromString(name, defaultValue);                                         \
        }                                                                                  \
                                                                                           \
        static inline bool FromStringSafe(EnumName* outValue, const char* name) {          \
            if (name == nullptr) {                                                         \
                return false;                                                              \
            }                                                                              \
                                                                                           \
            return FromString(outValue, name);                                             \
        }                                                                                  \
    };

#define DATA_ENUM(EnumName, ...) \
    DATA_TYPED_ENUM(EnumName, uint32_t, __VA_ARGS__)
