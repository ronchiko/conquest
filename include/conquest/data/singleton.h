#pragma once

namespace conquest {

	// clang-format off
	template<typename T>
	concept NamedType = requires(T value) {
		{ value.name } -> std::same_as<std::string>;
	};
	// clang-format on

	template<typename StoredT>
	class DataSingleton
	{
	public:
		static const StoredT& get(const std::string& name)
		{
			auto iterator = gInstances.find(name);
			if(gInstances.end() == iterator) {
				throw ConquestError::make("Failed to find ", typeid(StoredT).name(), " named ", name);
			}

			return iterator->second;
		}

	protected:

		static void emplace(StoredT data) {
			gInstances.emplace(std::make_pair(data.name, std::move(data)));
		}

		template<typename... Args> 
		static void emplace(Args...args)
		{
			emplace(StoredT{ DataSingleton<StoredT>{}, args... });
		}

	private:
		inline static std::unordered_map<std::string, StoredT> gInstances;
	};
}
