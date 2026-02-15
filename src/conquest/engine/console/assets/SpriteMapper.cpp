#include "conquest/common/Unicode.h"
#include "conquest/data/Package.h"
#include "conquest/log/Log.h"

#include "conquest/engine/console/assets/SpriteMapper.h"

namespace conquest::engine::console {

namespace {

/**
	Loads a coordinate from a toml table.
 */
std::pair<uint32_t, uint32_t> loadCoordinate(const toml::Toml& coordinate)
{
	return std::make_pair(static_cast<uint32_t>(coordinate["x"].as<int64_t>()),
						  static_cast<uint32_t>(coordinate["y"].as<int64_t>()));
}

template<typename T>
std::pair<uint32_t, uint32_t> loadRawCoordinate(const T& value)
{
	if constexpr(!::toml::is_table<T>) {
		throw ConquestError::make("Coordinate is not a table");
	}

	const auto table = toml::Toml(::toml::node_view{ &value });
	return loadCoordinate(table);
}

SpriteMapper::SpriteIndex_t createSpriteIndex(const std::string& name)
{
	if(name.length() <= 1) {
		return static_cast<SpriteMapper::SpriteIndex_t>(name[0]);
	}

	const auto [value, length] = parseUnicodeString(name.c_str());
	if(1 < length) {
		return value;
	}

	const auto hash = std::hash<std::string>()(name);
	return 0xFF00000000ull | hash;
}

}

SpriteMapper::SpriteMapper(SpriteManager& manager)
	: m_Manager(&manager)
{}

void SpriteMapper::loadMapping(const std::filesystem::path& path)
{
	const auto definition = package::asset::document(path);
	auto [tileWidth, tileHeight] = loadCoordinate(definition["tile_size"]);

	const auto textureId = m_Manager->registerTexture(definition["image"].as<std::string>());

	definition["Tiles"].as<::toml::table>().for_each([&](const ::toml::key& key, const auto& child) {
		const auto spriteName = std::string(key.str());
		const auto [x, y] = loadRawCoordinate(child);

		const auto spriteId = m_Manager->registerSprite(textureId, x * tileWidth, y * tileHeight, tileWidth, tileHeight);
		m_Mapping.emplace(createSpriteIndex(spriteName), spriteId);
	});
}

void SpriteMapper::draw(const SpriteIndex_t name, const Rect& destination, const Color foreground, const Color background) const
{
	try {
		const auto sprite = m_Mapping.at(name);
		m_Manager->draw(sprite, destination, foreground, background);
	} catch(const std::out_of_range&) {
		log::warn("Sprite name not found").param("sprite_name", name);
	}
}

SpriteId SpriteMapper::fetch(const SpriteIndex_t name) const
{
	return m_Mapping.at(name);
}

}
