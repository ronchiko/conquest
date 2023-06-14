#pragma once

#include <cstdint>

namespace conquest {
	using byte = unsigned char;   // NOLINT

	using int16 = std::int16_t; // NOLINT
	using uint16 = std::uint16_t; // NOLINT
	
	using int32 = std::int32_t;	  // NOLINT
	using uint32 = std::uint32_t; // NOLINT

	template<typename StoredT>
	struct v2
	{
		StoredT x;
		StoredT y;
	};

	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};

	template<typename... Ts>
	Overload(Ts...) -> Overload<Ts...>;
}
