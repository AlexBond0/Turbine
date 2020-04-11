#pragma once
#include <Windows.h>

inline void DisplayMessage(const char* msg)
{
  int len=strlen(msg)+1;
  LPSTR* text=(LPSTR*) malloc(2*len);
  // MultiByteToWideChar(CP_ACP, 0, msg, -1, text, len);
  MessageBox(NULL, *text, "OpenGL Message", MB_ICONINFORMATION);
  free(text);
}
