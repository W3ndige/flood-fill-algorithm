#include "menu.h"

void printCurrentColor(Uint32 *pixels, Uint32 currentColor) {
  for (size_t i = 0; i < MENU_HEIGHT; i++) {
    for (size_t j = 0; j < MENU_HEIGHT; j++) {
      pixels[j + i * SCREEN_WIDTH] = currentColor;
    }
  }
}

void printColorMenu(Uint32 *pixels) {
  for (size_t i = 0; i < NUMBER_OF_COLORS; i++) {
    size_t offset = MENU_HEIGHT * i;
    for (size_t j = 0; j < MENU_HEIGHT; j++) {
      for (size_t k = SCREEN_WIDTH - offset; k > (SCREEN_WIDTH - MENU_HEIGHT - offset); k--) {
        if (k < SCREEN_WIDTH) {
          pixels[k + j * SCREEN_WIDTH] = COLORS[i];
        }
      }
    }
  }
}

void printCurrentBrush(Uint32 *pixels, int brushSize, Uint32 currentColor) {
  for (size_t i = MENU_HEIGHT / 2 - brushSize / 2; i <  MENU_HEIGHT / 2 - brushSize / 2 + brushSize - 1; i++) {
    for (size_t j = (3 * MENU_HEIGHT / 2) - brushSize / 2; j < brushSize / 2 + (3 * MENU_HEIGHT / 2); j++) {
      pixels[j + i * SCREEN_WIDTH] = currentColor;
    }
  }
}

void resetCurrentBrushBackground(Uint32 *pixels) {
  for (size_t i = 0; i < MENU_HEIGHT; i++) {
    for (size_t j = MENU_HEIGHT; j < 2* MENU_HEIGHT; j++) {
      pixels[j + i * SCREEN_WIDTH] = COLORS[3];
    }
  }
}
