#pragma once

#include <Windows.h>

static COORD topLeft;

void registerTopLeft() {
  // Set the top left to the current console cursor position
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  topLeft = csbi.dwCursorPosition;
}

void clearConsole() {
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;

  GetConsoleScreenBufferInfo(console, &screen);
  FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y,
                              topLeft, &written);
  FillConsoleOutputAttribute(
      console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
      screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
  SetConsoleCursorPosition(console, topLeft);
}