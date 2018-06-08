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

void cutPixels(Uint32 *pixels, Uint32 *copyPastePixels, size_t startMouseX,
               size_t startMouseY, size_t endMouseX, size_t endMouseY) {

  size_t tmp;
  if (startMouseX > endMouseX) {
    tmp = startMouseX;
    startMouseX = endMouseX;
    endMouseX = tmp;
  }

  if (startMouseY > endMouseY) {
    tmp = startMouseY;
    startMouseY = endMouseY;
    endMouseY = tmp;
  }

  for (size_t i = startMouseY; i < endMouseY; i++) {
    for (size_t j = startMouseX; j < endMouseX; j++) {
      if ((j + i * SCREEN_WIDTH) <= SCREEN_HEIGHT * SCREEN_WIDTH &&
          (j + i * SCREEN_WIDTH) < ((i + 1) * SCREEN_WIDTH) &&
          (j + i * SCREEN_WIDTH) >= (SCREEN_WIDTH * MENU_HEIGHT)) {
        copyPastePixels[j + i * SCREEN_WIDTH] = pixels[j + i * SCREEN_WIDTH];
        pixels[j + i * SCREEN_WIDTH] = 0xFFFFFFF;
      }
    }
  }
}

void pastePixels(Uint32 *pixels, Uint32 *copyPastePixels,
                 size_t *copyMouseCoordinates) {
  for (size_t i = copyMouseCoordinates[1]; i <  copyMouseCoordinates[3]; i++) {
    for (size_t j = copyMouseCoordinates[0]; j < copyMouseCoordinates[2]; j++) {
      pixels[j + i * SCREEN_WIDTH] = copyPastePixels[j + i * SCREEN_WIDTH];
    }
  }
}
