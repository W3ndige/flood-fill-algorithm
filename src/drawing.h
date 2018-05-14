#ifndef DRAWING_H
#define DRAWING_H
#include <queue>
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;

void queueFloodFill4(Uint32 *pixels, size_t mouseX, size_t mouseY, Uint32 oldColor, Uint32 newColor);
void paintPixel(Uint32 *pixels, size_t mouseX, size_t mouseY, int brushSize, Uint32 currentColor);
void paintLine(Uint32 *pixels, size_t startMouseX, size_t startMouseY, size_t endMouseX, size_t endMouseY, int brushSize, Uint32 currentColor);
void paintCircle(Uint32 *pixels, size_t startMouseX, size_t startMouseY, size_t endMouseX, size_t endMouseY, int brushSize, Uint32 currentColor);


#endif
