#pragma once

#include <filesystem>

#include "conquest/data/Toml.h"
#include "conquest/data/Buffer.h"


namespace conquest::package {

namespace asset {

/**
	Loads a document using an asset from a path.
 */
toml::Toml document(const std::filesystem::path& path);

/**
	Loads an asset from any of the loaded packages.
 */
Buffer raw(const std::filesystem::path& path);

}

/**
   Loads a packages from a path.

   @param path: The path to the package.
   @param basePackage: It the package the base package? If this is true, all previous package data will be erased.
 */
void load(const std::filesystem::path& path, bool basePackage = false);

}
