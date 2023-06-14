#pragma once

#include <functional>
#include <string>
#include <vector>

#include "conquest/error.h"
#include "conquest/message.h"

namespace conquest {

	struct LoadResult
	{
		bool failure = false;
		MessageQueue messages = {};

		template<typename... Args>
		LoadResult& error(Args&&...args)
		{
			failure = true;
			messages.add(args...);
			return *this;
		}

		void guard(std::function<void()> func)
		{
			try {
				func();
			} catch(const ConquestError& err) {
				error(err.what());
			}
		}
	};

	/**
	   LOads all the assets.
	 */
	bool load();

	/**
	   Load data from an asset.

	   @param asset - The asset to load from.
	 */
	bool load(const std::string& asset);
}
