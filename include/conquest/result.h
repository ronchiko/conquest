#pragma once

#include <variant>

namespace conquest {

	template<typename OkT, typename ErrT>
	struct Result
	{
		constexpr Result(OkT value)
			: mVariant(std::move(value))
		{}

		constexpr Result(ErrT error)
			: mVariant(std::move(error))
		{}

		constexpr bool isError() const
		{
			return std::holds_alternative<ErrT>(mVariant);
		}

		constexpr OkT& value()
		{
			return std::get<OkT>(mVariant);
		}

		constexpr ErrT& error()
		{
			return std::get<ErrT>(mVariant);
		}

		std::variant<OkT, ErrT> mVariant;
	};
}
