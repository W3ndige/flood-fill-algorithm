#include "drawing.h"

// More efficient version of flood fill algorithm based on a queue.
// https://en.wikipedia.org/wiki/Flood_fill#Alternative_implementations
void queueFloodFill4(Uint32 *pixels, size_t mouseX, size_t mouseY,
                     Uint32 oldColor, Uint32 newColor) {
  if (newColor == pixels[mouseY * SCREEN_WIDTH + mouseX]) {
    return;
  }

  struct coordinates {
    size_t mouseX, mouseY;
  };
  std::queue<coordinates> fill;
  fill.push({mouseX, mouseY});

  while (!fill.empty()) {
    coordinates top = fill.front();
    fill.pop();
    if (top.mouseX < SCREEN_WIDTH && top.mouseY >= MENU_HEIGHT &&
        top.mouseY < SCREEN_HEIGHT &&
        pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] == oldColor) {

      pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] = newColor;
      fill.push({top.mouseX + 1, top.mouseY});
      fill.push({top.mouseX, top.mouseY + 1});
      fill.push({top.mouseX - 1, top.mouseY});
      fill.push({top.mouseX, top.mouseY - 1});
    }
  }
}

// Paint pixels with according size.
void paintPixel(Uint32 *pixels, size_t mouseX, size_t mouseY,
                int brushSize, Uint32 currentColor) {

  if (mouseY > MENU_HEIGHT) {
    for (size_t i = (mouseY - brushSize / 2); i < mouseY + brushSize / 2; i++) {
      for (size_t j = (int) mouseX - brushSize / 2 < 0 ? 0 : mouseX - brushSize / 2;
           j < mouseX + brushSize / 2; j++) {
        if ((j + i * SCREEN_WIDTH) <= SCREEN_HEIGHT * SCREEN_WIDTH &&
            (j + i * SCREEN_WIDTH) < ((i + 1) * SCREEN_WIDTH) &&
            (j + i * SCREEN_WIDTH) >= (SCREEN_WIDTH * MENU_HEIGHT)) {
          pixels[j + i * SCREEN_WIDTH] = currentColor;
        }
      }
    }
  }
}

// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void paintLine(Uint32 *pixels, size_t startMouseX, size_t startMouseY,
               size_t endMouseX, size_t endMouseY, int brushSize, Uint32 currentColor) {

  int deltaX = abs((int)(endMouseX - startMouseX));
  int deltaY = -abs((int)(endMouseY - startMouseY));
  int startX = startMouseX < endMouseX ? 1 : -1;
  int startY = startMouseY < endMouseY ? 1 : -1;
  int deltaError = deltaX + deltaY;
  bool end = false;

  while (!end) {
    paintPixel(pixels, startMouseX, startMouseY, brushSize, currentColor);
    if (startMouseX == endMouseX && startMouseY == endMouseY) {
      end = true;
    }
    int e2 = 2 * deltaError;
    if (e2 >= deltaY) {
      deltaError += deltaY;
      startMouseX += startX;
    }
    if (e2 <= deltaX) {
      deltaError += deltaX;
      startMouseY += startY;
    }
  }
}

void paintCircle(Uint32 *pixels, size_t startMouseX, size_t startMouseY,
                 size_t endMouseX, size_t endMouseY, int brushSize, Uint32 currentColor) {

	int circleRadius = (int)sqrt(pow(((int)endMouseX - (int)startMouseX), 2) +
                     pow((int)endMouseY - (int)startMouseY, 2));
	for (double alpha = 0; alpha <= (double)(2 * M_PI); alpha += 1 / (double)circleRadius) {
		size_t paintX = (size_t)(startMouseX + circleRadius * cos(alpha));
		size_t paintY = (size_t)(startMouseY + circleRadius * sin(alpha));
		if ((int)paintX - brushSize / 2 >= 0) {
      paintPixel(pixels, paintX, paintY, brushSize, currentColor);
    }
	}
}
