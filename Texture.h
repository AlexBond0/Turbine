#pragma once

#include <Windows.h>  // for access to the windows APIs
#include <iostream>
#include <filesystem>
#include <String>
#include <map>

#include "GLSetup.h"

class Texture
{
public:

	std::map<std::string, unsigned int> id;	// the map of texture handles

	Texture();
	~Texture();

	void loadAllTextures(); 

	unsigned int LoadTexture(std::experimental::filesystem::path filename);

private:

	std::wstring _PrepFilename(const std::string& filestring);
};

