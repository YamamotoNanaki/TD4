#pragma once
size_t AlignmentedSize(size_t size, size_t alignment);

template<class T>
inline T clamp(T num, T min = 0, T max = 1)
{
	return (num < min) ? min : ((num > max) ? max : num);
}

