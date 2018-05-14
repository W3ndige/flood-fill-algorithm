#ifndef GADGETS_H
#define GADGETS_H
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;
extern const Uint32 COLORS[];

Uint32 colorPicker(Uint32 *pixels, size_t mouseX, size_t mouseY);
void setCanvasBackground(Uint32 *pixels, Uint32 color);
void copyPixels(Uint32 *pixels, Uint32 *undoPixels);
void swapPixels(Uint32 *pixels, Uint32 *undoPixels);
void saveManager(Uint32 *pixels, int saveState, const char *fileName, const char *sampleName);

#endif
