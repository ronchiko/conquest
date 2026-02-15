#pragma once

#include <filesystem>

#include "conquest/data/Buffer.h"
#include "conquest/data/Toml.h"


namespace conquest::package
{

class IPackage
{
public:
	virtual ~IPackage() = default;

	/**
		Checks if an asset exists at a given path.
	 */
	[[nodiscard]] virtual bool exists(const std::filesystem::path& path) const = 0;

	[[nodiscard]] virtual toml::Toml loadDocument(const std::filesystem::path& path) const = 0;
	[[nodiscard]] virtual Buffer loadBinary(const std::filesystem::path& path) const = 0;
};

}
