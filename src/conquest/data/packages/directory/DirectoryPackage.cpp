#include <fstream>

#include "conquest/Error.h"
#include "conquest/data/packages/directory/DirectoryPackage.h"


namespace conquest::package {

DirectoryPackage::DirectoryPackage(std::filesystem::path root)
	: m_RootPath(std::move(root))
{}

bool DirectoryPackage::exists(const std::filesystem::path& path) const
{
	return std::filesystem::exists(m_RootPath / path);
}

toml::Toml DirectoryPackage::loadDocument(const std::filesystem::path& path) const
{
	auto parseResult = std::make_shared<::toml::parse_result>(::toml::parse_file(getFullPath(path).string()));
	if(!parseResult->is_table()) {
		throw ConquestError::make("Path ", path, " is not a TOML table");
	}

	return toml::Toml(std::move(parseResult));
}

Buffer DirectoryPackage::loadBinary(const std::filesystem::path& path) const
{
	const auto streamSize = std::filesystem::file_size(getFullPath(path));
	auto data = Buffer{ streamSize };

	std::ifstream stream(getFullPath(path), std::ios::in | std::ios::binary);
	stream.seekg(std::ios::beg);
	stream.read(reinterpret_cast<char*>(data.address()), data.length());

	return data;
}

std::filesystem::path DirectoryPackage::getFullPath(const std::filesystem::path& relativePath) const
{
	return m_RootPath / relativePath;
}

}
