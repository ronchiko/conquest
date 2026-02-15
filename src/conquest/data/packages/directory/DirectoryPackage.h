#pragma once

#include "conquest/data/packages/IPackage.h"

namespace conquest::package {

class DirectoryPackage final : public IPackage
{
public:
	explicit DirectoryPackage(std::filesystem::path root);

	bool exists(const std::filesystem::path& path) const override;

	[[nodiscard]] toml::Toml loadDocument(const std::filesystem::path& path) const override;
	[[nodiscard]] Buffer loadBinary(const std::filesystem::path& path) const override;

private:
	[[nodiscard]] std::filesystem::path getFullPath(const std::filesystem::path& relativePath) const;

	std::filesystem::path m_RootPath;
};

}
