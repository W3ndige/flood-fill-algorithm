#ifndef FILES_H
#define FILES_H
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;
extern const Uint32 COLORS[];



void saveImage(Uint32 *pixels, const char *fileName);
void readImage(Uint32 *pixels, const char *fileName);
void saveManager(Uint32 *pixels, int saveState, const char *fileName, const char *sampleName);
void printControls();

#endif
