#pragma once

#include <vector>
#include <string>

namespace conquest::save {

struct SavePreview
{
	std::string name;
};

/**
   Returns a preview of all the local saves.
 */
std::vector<SavePreview> previewAllSaves();

}
