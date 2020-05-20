#pragma once

#include <Windows.h>  // for access to the windows APIs
#include <iostream>
#include <filesystem>
#include <String>
#include <map>

#include "GLSetup.h"

// texture information struct
struct Texture {

	Texture(std::string textureName, unsigned int textureID)
		: name(textureName) {

		id = textureID;
	};

	const std::string	name;
	unsigned int		id;
};

// Container of handles to textures loaded from file
class TextureManager {

public:

	// std::map<std::string, unsigned int> id;	// the map of texture handles

	TextureManager();
	~TextureManager();

	void loadAllTextures(); 

	Texture* GetTexture(std::string name);
	std::map<std::string, Texture*> GetAllTextures();

	unsigned int LoadTexture(std::experimental::filesystem::path filename);

private:

	std::map<std::string, Texture*> _textures;	// the map of textures

	std::wstring _PrepFilename(const std::string& filestring);
};

inline std::map<std::string, Texture*> TextureManager::GetAllTextures() {

	return _textures;
}
