#pragma once

#include <optional>

namespace conquest {

template<typename T, typename = std::enable_if_t<std::is_invocable_r_v<void, T>>>
class FinalAction
{
public:
	explicit FinalAction(T operation)
		: m_Callback(std::move(operation))
	{}

	FinalAction(FinalAction&&) = default;
	FinalAction& operator=(FinalAction&&) = default;

	FinalAction(const FinalAction&) = default;
	FinalAction& operator=(const FinalAction&) = default;

	~FinalAction()
	{
		if(!m_Callback.has_value()) {
			return;
		}

		m_Callback.value()();
	}

	void cancel()
	{
		m_Callback = std::nullopt;
	}

private:
	std::optional<T> m_Callback;
};

auto finally(auto callback)
{
	return FinalAction(std::move(callback));
}

}
