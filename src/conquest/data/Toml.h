#pragma once

#include <toml++/toml.hpp>

#include "conquest/Error.h"

namespace conquest::toml {

namespace detail {

template<typename T>
const T& unpack(const T *value)
{
	return *value;
}

template<typename T>
const T& unpack(const ::toml::value<T> *value)
{
	return value->get();
}

}

class Toml
{
public:
	explicit Toml(const ::toml::node_view<::toml::node> table)
		: m_Node(const_cast<const ::toml::node *>(table.node()))
	{}

	explicit Toml(const ::toml::node_view<const ::toml::node> table)
		: m_Node(table)
	{}

	explicit Toml(std::shared_ptr<::toml::parse_result> result)
		: m_Node(result->as_table())
		, m_Storage(std::move(result))
	{}

	bool has(const std::string_view& name) const
	{
		if(const auto table = m_Node.as_table(); nullptr != table) {
			return nullptr != table->get(name);
		}

		return false;
	}

	template<typename T>
	const T& as() const
	{
		const auto *value = m_Node.as<T>();
		assert(value != nullptr);
		return detail::unpack(value);
	}

	Toml operator[](const std::string_view& name) const
	{
		if(const auto table = m_Node.as_table(); nullptr != table)
		{
			return Toml((*table)[name]);
		}

		throw ConquestError("Cannot call operator[] on non table types");
	}

private:
	::toml::node_view<const ::toml::node> m_Node;
	std::shared_ptr<::toml::parse_result> m_Storage;
};

}
