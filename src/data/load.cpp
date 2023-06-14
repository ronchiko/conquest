#include "conquest/data/load.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <numeric>

#include "conquest/graphics/asset.h"
#include "conquest/lang/lexer.h"
#include "conquest/lang/parser.h"

#include "conquest/data/element.h"
#include "conquest/data/race.h"
#include "conquest/data/weapon_type.h"
#include "conquest/data/profession.h"

namespace conquest {

	static bool gShouldPublish = true;
	static std::unordered_map<lang::Object::Type, std::vector<lang::Object>> gQueuedObjects;

	static const std::map<lang::Object::Type, std::function<LoadResult(lang::Object&)>> INITIALIZERS{
		// The order of stuff here is the order stuff will be loaded at.
		{ lang::Object::Type::Element, Element::loadFromObject },
		{ lang::Object::Type::WeaponType, WeaponType::loadFromObject },
		{ lang::Object::Type::Race, Race::loadFromObject },
		{ lang::Object::Type::Profession, Profession::loadFromObject },
	};

	static const std::filesystem::path SEARCH_FOLDERS[] = {
		LR"(data)",
		LR"(ext)",
	};

	static void displayMessages(const std::string& title, const MessageQueue& messages)
	{
		std::cout << title << std::endl;
		for(const auto& message : messages.messages) {
			std::cout << "   * " << message << std::endl;
		}
		std::cout << std::endl;
	}

	static bool loadDirectory(const std::filesystem::path& dir)
	{
		bool isOk = true;
		for(const auto& entry : std::filesystem::directory_iterator(LOCAL_ASSETS_FOLDER / dir)) {
			if(entry.is_directory()) {
				isOk &= loadDirectory(entry.path());
				continue;
			}

			std::cout << "Loading " << entry.path() << std::endl;
			isOk &= load(entry.path().string());
		}

		return isOk;
	}

	static bool publishObject(std::pair<const std::string, lang::Object>& objectPair)
	{
		auto& [name, object] = objectPair;
		auto iterator = INITIALIZERS.find(object.type);
		if(INITIALIZERS.end() == iterator) {
			std::cout << "Object type " << static_cast<uint32>(object.type) << " is unknown and cannot be initialized"
					  << std::endl;
			return false;
		}

		auto loadResult = iterator->second(object);
		if(loadResult.failure) {
			std::ostringstream objectTitle;
			objectTitle << "Errors while initializing object " << object.name << ": ";
			displayMessages(objectTitle.str(), loadResult.messages);
			return false;
		}

		return true;
	}

	static bool publishQueuedObjects()
	{
		bool isOk = true;
		for(const auto& [type, publisher] : INITIALIZERS) {
			auto iterator = gQueuedObjects.find(type);
			if(gQueuedObjects.end() == iterator) {
				continue;
			}

			for(auto& object : iterator->second) {
				auto result = publisher(object);
				if(result.failure) {
					std::ostringstream title;
					title << "Errors while initializing object " << object.name << ": ";

					displayMessages(title.str(), result.messages);
					isOk = false;
				}
			}
		}

		gQueuedObjects = {};

		return isOk;
	}

	bool load(const std::string& asset)
	{
		std::ostringstream title;
		title << "Errors while parsing " << asset << std::endl;

		// Lex the asset
		lang::Lexer lexer(Asset::load(asset, AssetType::Data).getData());
		auto lexResult = lexer.parseTokens();
		if(!lexResult.isOk) {
			displayMessages(title.str(), lexResult.messages);
			return false;
		}

		// Parse the asset
		lang::Parser parser(std::move(lexResult.tokens));
		auto parseResult = parser.parse();
		if(!parseResult.isOk) {
			displayMessages(title.str(), parseResult.messages);
			return false;
		}

		if(gShouldPublish) {
			// Publish
			return std::accumulate(parseResult.objects.begin(),
								   parseResult.objects.end(),
								   true,
								   [](const auto prev, auto& pair) { return prev && publishObject(pair); });
		} else {
			// Queue up all the loaded object into the list.
			for(auto& [name, object] : parseResult.objects) {
				auto iterator = gQueuedObjects.find(object.type);
				if(gQueuedObjects.end() == iterator) {
					gQueuedObjects[object.type] = { std::move(object) };
					continue;
				}

				iterator->second.push_back(std::move(object));
			}

			return true;
		}
	}

	bool load()
	{
		struct DelayedPublisher
		{
			DelayedPublisher()
			{
				gShouldPublish = false;
			}

			~DelayedPublisher()
			{
				try {
					publishQueuedObjects();
					gShouldPublish = true;
				} catch(...) {}
			}
		} publisher;

		bool isOk = true;
		for(const auto& directory : SEARCH_FOLDERS) {
			if(std::filesystem::exists(LOCAL_ASSETS_FOLDER / directory)) {
				isOk &= loadDirectory(directory);
			}
		}

		return isOk;
	}
}
