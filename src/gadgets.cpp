#include "gadgets.h"

Uint32 colorPicker(Uint32 *pixels, size_t mouseX, size_t mouseY) {
  return pixels[mouseY * SCREEN_WIDTH + mouseX];
}

void setCanvasBackground(Uint32 *pixels, Uint32 color) {
  memset(pixels, color, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

void copyPixels(Uint32 *pixels, Uint32 *undoPixels) {
  memcpy(undoPixels, pixels, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

void swapPixels(Uint32 *pixels, Uint32 *undoPixels) {
  Uint32 *tmpPixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
  memcpy(tmpPixels, pixels, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
  memcpy(pixels, undoPixels, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
  memcpy(undoPixels, tmpPixels, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
  setCanvasBackground(tmpPixels, 255); // Overwrite the image from memory before making it free.
  delete[] tmpPixels;
}
