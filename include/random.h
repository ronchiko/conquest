#pragma once

#include <random>

namespace Random {
	constexpr std::int32_t FLT_ACCURACY = 10000;
	constexpr std::uint32_t MAX_UINT_VALUE = 0xFFFFFFFF;  

	using random = std::random_device;
	
	inline float value(void) {
		static std::uniform_int_distribution<int32_t> distribution(0, FLT_ACCURACY);

		random random_device;

		return static_cast<float>(distribution(random_device)) / FLT_ACCURACY;
	}
	
	inline uint32_t rand(void) {
		static std::uniform_int_distribution<uint32_t> dist(0, MAX_UINT_VALUE); 

		random random_device;
		return dist(random_device);
	}
	
	inline float float32(float min, float max) {
		return (max - min) * value() + min;
	}

	inline int32_t int32(int32_t min, int32_t max) {
		return (rand() % (max - min)) + min;
	}
};
