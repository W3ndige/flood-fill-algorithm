#ifndef MENU_H
#define MENU_H
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;
extern const Uint32 COLORS[];

void printCurrentColor(Uint32 *pixels, Uint32 currentColor);
void printColorMenu(Uint32 *pixels);
void printCurrentBrush(Uint32 *pixels, int brushSize, Uint32 currentColor);
void resetCurrentBrushBackground(Uint32 *pixels);

#endif
