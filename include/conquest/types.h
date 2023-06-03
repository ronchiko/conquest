#pragma once

#include <cstdint>

namespace conquest {
	using byte = unsigned char; // NOLINT

	using int32 = std::int32_t;	  // NOLINT
	using uint32 = std::uint32_t; // NOLINT

	template<typename StoredT>
	struct v2
	{
		StoredT x;
		StoredT y;
	};
}