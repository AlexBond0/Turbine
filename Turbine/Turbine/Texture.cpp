#include "Texture.h"



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager() {

	for (const auto& texture : _textures)
		delete texture.second;
}

Texture* TextureManager::GetTexture(std::string name) {

	return _textures[name];
}

// load all BMP textures found in the /textures directory
void TextureManager::loadAllTextures() {

	// get the /textures filepath
	std::experimental::filesystem::path dirPath;
	dirPath = std::experimental::filesystem::current_path();
	dirPath = dirPath / "textures";

	// if the filepath exists
	if (std::experimental::filesystem::exists(dirPath)) {

		// for each file
		for (const auto & entry : std::experimental::filesystem::directory_iterator(dirPath)) {

			// get the file
			std::string fullname = entry.path().filename().u8string();
			size_t lastindex = fullname.find_last_of(".");
			std::string rawname = fullname.substr(0, lastindex);

			// load the texture and save it into the texture map
			_textures[rawname] = new Texture(rawname, LoadTexture(entry.path()));

			// id[rawname] = LoadTexture(entry.path());
			OutputDebugStringA(("\nLOADED TEXTURE : " + fullname + " > AS : " + rawname).c_str());
		}
	}
	else {

		OutputDebugStringA("\nNO TEXTURE FILES FOUND!");
	}

}

// load an individual texture into OpenGL
unsigned int TextureManager::LoadTexture(std::experimental::filesystem::path filename) {

	// ===============================================================
	// load texture from file

	// create object for bmp info
	BITMAP	bmpinfo;

	// load the image into a bmp handle
	std::string filepath = filename.string();
	HBITMAP bmpImgHandle = (HBITMAP) ::LoadImage(
		NULL,
		filepath.c_str(),
		IMAGE_BITMAP, 
		0,
		0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE
	);

	// assign the data of the handle to the bmpinfo
	GetObject(bmpImgHandle, sizeof(BITMAP), &bmpinfo);


	// ===============================================================
	// load texture into memory

	// define used byte structures
	BYTE r, g, b;

	// pointer to memory of destination image on the heap with malloc
	BYTE* destPointer = (BYTE*)malloc(bmpinfo.bmWidth * bmpinfo.bmHeight * 4);

	// can also use DWORD, using this structure
	DWORD* dwordDestPointer = (DWORD*)malloc(bmpinfo.bmWidth * bmpinfo.bmHeight * 4);
	DWORD* dwordDest = dwordDestPointer;

	for (int j = 0; j < bmpinfo.bmHeight; j++) {

		BYTE* srcOffset = ((BYTE*)bmpinfo.bmBits) + j * bmpinfo.bmWidthBytes;
		BYTE* destOffset = destPointer + j * 4 * bmpinfo.bmWidth;

		for (int i = 0; i < bmpinfo.bmWidth; i++) {

			b = *(srcOffset++);
			g = *(srcOffset++);
			r = *(srcOffset++);

			// BYTE allocation
			*(destOffset++) = b;
			*(destOffset++) = g;
			*(destOffset++) = r;
			*(destOffset++) = 0;

			// DWORD allocation
			*(dwordDest++) = (b << 16) | (g << 8) | (r << 0); // | 0xFF;

			if (bmpinfo.bmBitsPixel == 32)
				++srcOffset;

		}
	}

	// ===============================================================
	// prepare texture for openGL

	// generate an ID for a texture
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// bind the texture ID to openGL
	glBindTexture(GL_TEXTURE_2D, textureID);

	// clamp edges of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// send texture data to openGL
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		bmpinfo.bmWidth,
		bmpinfo.bmHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		dwordDestPointer
	);


	// ===============================================================
	// Cleanup

	// delete heap memory
	free(destPointer);
	free(dwordDestPointer);

	// delete handle (handles don't get deleted automatically)
	::DeleteObject(bmpImgHandle);

	return textureID;
}

// convert the file string into a wstring
std::wstring TextureManager::_PrepFilename(const std::string& filestring) {

	int size = (int)filestring.length() + 1;
	int length = MultiByteToWideChar(CP_ACP, 0, filestring.c_str(), size, 0, 0);

	wchar_t* buffer = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, filestring.c_str(), size, buffer, length);

	std::wstring filepath = std::wstring(buffer);

	free(buffer);
	return filepath;
}