
#include "conquest/graphics/asset.h"

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

namespace conquest {
	Asset Asset::load(const std::string& path, const AssetType type)
	{
		const auto assetPath = path;
		std::fstream fileStream(assetPath);

		// First we read all the lines from the files.
		std::string currentLine;
		std::vector<std::string> lines;
		while(std::getline(fileStream, currentLine)) {
			// For non art assets the newline is needed
			if(AssetType::Art != type) {
				currentLine += "\n";
			}

			lines.push_back(currentLine);
			
		}

		// If we are parsing an art asset we have to align all the lines to be same length
		uint32 width = 0;
		if(AssetType::Art == type) {
			const auto& maxLine = std::max_element(lines.begin(), lines.end(), [](const auto& lhs, const auto& rhs) {
				return lhs.size() < rhs.size();
			});

			for(auto& line : lines) {
				if(line.size() < maxLine->size()) {
					line += std::string(maxLine->size() - line.size(), ' ');
				}
			}

			width = static_cast<uint32>(maxLine->size());
		}

		// Calculate the assets size.
		const auto totalAssetSize = std::accumulate(lines.begin(),
													lines.end(),
													0Ull,
													[](const auto& acc, const auto& add) { return acc + add.size(); });

		// Now put the lines into a buffer.
		Buffer<char> buffer(totalAssetSize + 1);
		char *current = buffer.data();

		for(auto& line : lines) {
			current = std::uninitialized_copy_n(line.begin(), line.size(), current);
		}

		buffer[totalAssetSize] = '\0';

		return Asset(type, std::move(buffer), width, static_cast<uint32>(lines.size()));
	}

	Asset::Asset(const AssetType type, Buffer<char> data, const uint32 width, const uint32 height)
		: mType(type)
		, mBuffer(std::move(data))
		, mWidth(width)
		, mHeight(height)
	{}
}