#pragma once

#include <concepts>

#include "Core/Memory/SpanWriter.h"
#include "Core/Memory/SpanReader.h"

namespace ml {

template <typename T>
concept Serializable = requires(const T& t, void* dest) {
    { t.serialize(dest) } -> std::same_as<uint64_t>;
};

template <typename T>
concept Deserializable = requires(T& t, const void* src) {
    { t.deserialize(src) } -> std::same_as<void>;
};

template <typename T>
concept SerializableStruct = requires(const T& t, SpanWriter* writer) {
    { t.serialize(writer) } -> std::same_as<uint64_t>;
};

template <typename T>
concept DeserializableStruct = requires(T& t, SpanReader* reader) {
    { t.deserialize(reader) } -> std::same_as<void>;
};

class Serializer {
public:
    template <Serializable T>
    static uint64_t Serialize(const T& src, void* dest) {
        return src.serialize(dest);
    }

    template <SerializableStruct T>
    static uint64_t SerializeStruct(const T& src, SpanWriter* writer) {
        return src.serialize(writer);
    }

    template <Deserializable T>
    static void Deserialize(T* outValue, const void* src) {
        outValue->deserialize(src);
    }

    template <Deserializable T>
    static T Deserialize(const void* source) {
        T value = T();
        value.deserialize(source);
        return value;
    }

    template <DeserializableStruct T>
    static void DeserializeStruct(T* outValue, SpanReader* reader) {
        outValue->deserialize(reader);
    }

    template <DeserializableStruct T>
    static T DeserializeStruct(SpanReader* reader) {
        T value = T();
        value.deserialize(reader);
        return value;
    }

private:
    Serializer() = delete;
};

}
