#pragma once
#include <cstdint>

// 基本型に対する条件をまとめるマクロを定義
#define IS_BASIC_TYPE(T) \
    std::is_same<T, int>::value || \
    std::is_same<T, float>::value || \
    std::is_same<T, double>::value || \
    std::is_same<T, char>::value || \
    std::is_same<T, bool>::value || \
    std::is_same<T, int8_t>::value || \
    std::is_same<T, uint8_t>::value || \
    std::is_same<T, int16_t>::value || \
    std::is_same<T, uint16_t>::value || \
    std::is_same<T, int32_t>::value || \
    std::is_same<T, uint32_t>::value || \
    std::is_same<T, int64_t>::value || \
    std::is_same<T, uint64_t>::value || \
    std::is_same<T, wchar_t>::value || \
    std::is_same<T, char16_t>::value || \
    std::is_same<T, char32_t>::value


size_t AlignmentedSize(size_t size, size_t alignment);

template<class T>
inline T clamp(T num, T min = 0, T max = 1)
{
	return (num < min) ? min : ((num > max) ? max : num);
}