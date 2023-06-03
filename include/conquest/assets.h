#pragma once

#include <string>

#define ASSET_FOLDER "assets/"

enum AssetType_e { ASSET_TEXT, ASSET_ART, ASSET_DATA, ASSET_OTHER };

struct Asset {
  Asset(const std::string &path, AssetType_e type);
  ~Asset(void);

  int width(void) const { return m_Width; }
  int height(void) const { return m_Height; }
  int length(void) const { return m_Length; }

  inline AssetType_e getType(void) const { return m_Type; }
  inline const char *getData(void) const { return m_Buffer; }

private:
  char *m_Buffer;
  AssetType_e m_Type;

  int m_Width, m_Height, m_Length;
};
