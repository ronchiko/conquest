#include <filesystem>

#include "conquest/game/battle/WeaponType.h"
#include "conquest/game/character/traits/Profession.h"
#include "conquest/game/character/traits/Race.h"

#include "conquest/data/packages/directory/DirectoryPackage.h"

#include "conquest/data/Package.h"


namespace conquest::package {

namespace {

constexpr std::string_view PACKAGE_ROOT_NAME = "package.toml";

std::vector<std::unique_ptr<IPackage>> g_LoadedPackages;


std::unique_ptr<IPackage> openPackage(const std::filesystem::path& path)
{
	if(std::filesystem::is_directory(path)) {
		return std::make_unique<DirectoryPackage>(path);
	}

	throw ConquestError::make("Failed to open package ", path);
}

template<typename T>
void loadFromDocument(const toml::Toml& document)
{
	Singleton<detail::Mapping<T>> singleton;

	for(const auto& [name, value] : document.as<::toml::table>()) {
		toml::Toml subDocument(::toml::node_view{&value});
		singleton->add(std::string(name.str()), subDocument);
	}
}

void loadPackageInternal(const IPackage& package)
{
	const auto packageRoot = package.loadDocument(PACKAGE_ROOT_NAME);

	loadFromDocument<WeaponType>(package.loadDocument(packageRoot["weapon_types"].as<std::string>()));
	loadFromDocument<Race>(package.loadDocument(packageRoot["races"].as<std::string>()));
	loadFromDocument<Profession>(package.loadDocument(packageRoot["professions"].as<std::string>()));
}

}

toml::Toml asset::document(const std::filesystem::path& path)
{
	for(const auto& package : g_LoadedPackages) {
		if(!package->exists(path)) {
			continue;
		}

		return package->loadDocument(path);
	}

	throw ConquestError("Did not find assets");
}

Buffer asset::raw(const std::filesystem::path& path)
{
	for(const auto& package : g_LoadedPackages)
	{
		if(!package->exists(path)) {
			continue;
		}

		return package->loadBinary(path);
	}

	throw ConquestError("Did not find assets");
}

void load(const std::filesystem::path& path, const bool basePackage)
{
	if(basePackage) {
		// TODO: Erase previous data
	}

	auto package = openPackage(path);
	loadPackageInternal(*package);

	g_LoadedPackages.emplace_back(std::move(package));
}

}
