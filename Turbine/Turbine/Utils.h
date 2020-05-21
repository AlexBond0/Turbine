#pragma once
#include <Windows.h>
#include <vector>
#include <string>

inline void DisplayMessage(const char* msg)
{
  int len=strlen(msg)+1;
  LPSTR* text=(LPSTR*) malloc(2*len);
  // MultiByteToWideChar(CP_ACP, 0, msg, -1, text, len);
  MessageBox(NULL, *text, "OpenGL Message", MB_ICONINFORMATION);
  free(text);
}

inline std::vector<std::string> SplitToTokens(std::string line, char seperator) {

	std::vector<std::string> vec;
	std::string token = "";

	for (char t : line) {

		if (t == seperator) {

			vec.push_back(token);
			token = "";
		}
		else {

			token += t;
		}
	}

	if (token.compare("") != 0)
		vec.push_back(token);

	return vec;
}