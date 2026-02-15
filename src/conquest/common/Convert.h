#pragma once

#include <ranges>
#include <vector>

namespace conquest {

template<typename T, std::ranges::range R>
std::vector<T> toVector(R& range) {
	return std::vector<T>(range.begin(), range.end());
}

template<typename T, std::ranges::range R>
std::vector<T> toVector(R&& range)
{
	return std::vector<T>(range.begin(), range.end());
}

}
