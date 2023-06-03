
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "conquest/assets.h"

Asset::Asset(const std::string& path, AssetType_e type)
{
	std::stringstream _path;
	_path << ASSET_FOLDER << path;
	std::stringstream buffer;

	std::fstream file(_path.str());
	std::string line;

	m_Type = type;
	m_Height = 0;
	m_Width = 0;
	std::vector<std::string> lines;
	while(getline(file, line)) {
		lines.push_back(line);
		int w = line.length();
		if(w > m_Width) {
			m_Width = w;
		}
		m_Height++;
	}

	for(std::string& l : lines) {
		buffer << l;
		if(type == ASSET_ART) {
			// Do padding
			for(int _ = 0; _ < m_Width - l.length(); _++) {
				buffer << " ";
			}
		} else {
			buffer << "\n";
		}
	}

	file.close();
	std::string dataString = buffer.str();
	m_Length = dataString.length();

	m_Buffer = new char[m_Length + 1];
	memcpy(m_Buffer, dataString.c_str(), m_Length);
	m_Buffer[m_Length] = 0;
}

Asset::~Asset(void)
{
	delete[] m_Buffer;
}
