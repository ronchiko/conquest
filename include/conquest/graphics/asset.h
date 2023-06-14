#pragma once

#include <string>
#include <filesystem>

#include "conquest/buffer.h"
#include "conquest/types.h"

namespace conquest {
	inline const std::filesystem::path LOCAL_ASSETS_FOLDER = "assets/";


	enum class AssetType
	{
		Text,
		Art,
		Data,
		Other
	};

	struct Asset
	{
		static Asset load(const std::string& path, AssetType type);

		constexpr uint32 width() const
		{
			return mWidth;
		}

		constexpr uint32 height() const
		{
			return mHeight;
		}

		constexpr AssetType getType(void) const
		{
			return mType;
		}

		constexpr const char *getData(void) const
		{
			return mBuffer.data();
		}

		constexpr size_t length(void) const
		{
			return mBuffer.size();
		}

	private:
		explicit Asset(AssetType type, Buffer<char> data, uint32 width, uint32 height);

		AssetType mType;
		Buffer<char> mBuffer;

		uint32 mWidth;
		uint32 mHeight;
	};
}