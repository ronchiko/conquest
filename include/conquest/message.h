#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace conquest {
	struct MessageQueue
	{
		std::vector<std::string> messages;

		template<typename... Parts>
		void add(Parts&&...parts)
		{
			std::ostringstream stream;
			(stream << ... << parts);
			messages.push_back(stream.str());
		}
	};
}
