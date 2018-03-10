#include <stdio.h>
#include <string.h>
#include <queue>
#include <SDL2/SDL.h>

// TODO(W3ndige): Implement better brush algorithm.
// TODO(W3ndige): Add option to choose between recursive and queue algorithms in the parameters.
// TODO(W3ndige): Optimization.

const size_t SCREEN_WIDTH  = 720;
const size_t SCREEN_HEIGHT = 480;
const size_t MENU_HEIGHT = 30;
const size_t NUMBER_OF_COLORS = 20;

const Uint32 COLORS[NUMBER_OF_COLORS] = {0xFF000000, 0xC0C0C0, 0x808080, 0xFFFFFF, 0x800000, 0xFF0000, 0xFFA500, 0x800080, 0xC969FF, 0xFF69B4, 0x008000, 0x08E100, 0x93FF2D, 0xC3C355, 0xFFFF00, 0xFFFF83, 0x000080, 0x0000FF, 0x008080, 0x00FFFF};

// Menu object that will work for the line of 30 pixels height, at the top of the window.
class Menu {
  public:
    void printCurrentColor(Uint32 *pixels, Uint32 currentColor) {
      for (size_t i = 0; i < MENU_HEIGHT; i++) {
        for (size_t j = 0; j < MENU_HEIGHT; j++) {
          pixels[j + i * SCREEN_WIDTH] = currentColor;
        }
      }
    }
    void printColorMenu(Uint32 *pixels) {
      for (size_t i = 0; i < sizeof(COLORS)/sizeof(COLORS[0]); i++) {
        size_t offset = MENU_HEIGHT * i;
        for (size_t j = 0; j < MENU_HEIGHT; j++) {
          for (size_t k = 720 - offset; k > (720 - MENU_HEIGHT - offset); k--) {
            pixels[k + j * SCREEN_WIDTH] = COLORS[i];
          }
        }
      }
    }
};

// TODO(W3ndige): Experiment.
// More efficient version of flood fill algorithm based on a queue.
// https://en.wikipedia.org/wiki/Flood_fill#Alternative_implementations
void queueFloodFill4(Uint32 *pixels, size_t mouseX, size_t mouseY, Uint32 oldColor, Uint32 newColor) {
  if (newColor == pixels[mouseY * SCREEN_WIDTH + mouseX]) {
    return;
  }

  struct coordinates { size_t mouseX, mouseY;};
  std::queue<coordinates> fill;
  fill.push({mouseX, mouseY});

  while (!fill.empty()) {
    coordinates top = fill.front();
    fill.pop();
    if (top.mouseX < SCREEN_WIDTH && top.mouseY >= MENU_HEIGHT && top.mouseY < SCREEN_HEIGHT && pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] == oldColor) {
        pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] = newColor;
        fill.push({top.mouseX + 1, top.mouseY});
        fill.push({top.mouseX, top.mouseY + 1});
        fill.push({top.mouseX - 1, top.mouseY});
        fill.push({top.mouseX, top.mouseY - 1});
    }
  }
}

// Paint pixels with according size.
void paintPixel(Uint32 *pixels, size_t mouseX, size_t mouseY, int brushSize, Uint32 currentColor) {
  if (brushSize < 0) {
    brushSize = 1;
  }
  if (brushSize > 10) {
    brushSize = 10; // Maximum size
  }

  for (size_t i = mouseY; i < mouseY + brushSize; i++) {
    if (i >= MENU_HEIGHT) {
      for (size_t j = mouseX; j < mouseX + brushSize; j++) {
        if ((j + i * SCREEN_WIDTH) <= SCREEN_HEIGHT * SCREEN_WIDTH && (j + i * SCREEN_WIDTH) < ((i + 1)* SCREEN_WIDTH)) {
          pixels[j + i * SCREEN_WIDTH] = currentColor;
        }
      }
    }
  }
}

void setCanvasBackground(Uint32 *pixels, Uint32 color) {
  memset(pixels, color, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

void copyPixels(Uint32 *pixels, Uint32 *undoPixels) {
  memcpy(undoPixels, pixels, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

void swapPixels(Uint32 *pixels, Uint32 *undoPixels) {
  memcpy(pixels, undoPixels, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

// Create a hexdump of an pixel array.
void saveImage(Uint32 *pixels) {
  FILE *image = fopen("save.pix", "wb");
  if (image) {
    fwrite(pixels, sizeof(Uint32),SCREEN_WIDTH * SCREEN_HEIGHT, image);
    fclose(image);
  }
  else {
    puts("Can't open specified sample. Please check if it's present in the program directory.");
  }
}

// Read the hexdump from the specified file.
void readImage(Uint32 *pixels,const char *fileName) {
  FILE *image = fopen(fileName, "rb");
  if (image) {
    fseek(image, 0, SEEK_END);
    size_t size = ftell(image);
    fseek(image, 0, SEEK_SET);
    if (size == SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)) { // Protect from loading to big files.
      fread(pixels, sizeof(Uint32),SCREEN_WIDTH * SCREEN_HEIGHT, image);
      fclose(image);
    }
    else {
      puts("Input file is too large for the specified screen dimensions.");
    }
  }
  else {
    puts("Can't open specified sample. Please check if it's present in the program directory.");
  }
}

void printControls() {
    FILE *helpfile = fopen("README.md","r");
    if (helpfile) {
        char textline[300];
        while (!feof(helpfile)) {
            fgets(textline, 300, helpfile);
            puts(textline);
        }
    fclose(helpfile);
    }
    else {
        puts("Can't open help file! Make sure it's present in the program directory.");
    }
}

int main(int argc, char *argv[]) {
  // Initialize SDL2.
 if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
   printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
   SDL_Quit();
   return 1;
 }

 SDL_Window *window = SDL_CreateWindow("Flood Fill", SDL_WINDOWPOS_CENTERED, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
 if (window == nullptr) {
   printf( "SDL Window could not initialize! SDL_Error: %s\n", SDL_GetError() );
   SDL_Quit();
   return 1;
 }

 SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
 if (renderer == nullptr) {
   printf( "SDL Renderer could not initialize! SDL_Error: %s\n", SDL_GetError() );
   SDL_Quit();
   return 1;
 }

 printControls();
 Uint32 *pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; // Assign new set of pixels.
 Uint32 *undoPixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; // Assign a set of pixels used in undo/redo algorithm.
 SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
 setCanvasBackground(pixels, 255);

 // Essential variables
 bool end = false;
 bool leftMouseButton = false;
 Uint32 currentColor = 0;
 int brushSize = 5;

 SDL_Event event;
 Menu menu;
 menu.printColorMenu(pixels); // Don't need to update this part of menu.
 copyPixels(pixels, undoPixels);

 size_t mouseX = 0;
 size_t mouseY = 0;

 while (!end) {
   while (SDL_PollEvent(&event)) {

     // Possible optimization?
     menu.printCurrentColor(pixels, currentColor);
     SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

     switch (event.type) {
       case SDL_QUIT:
         end = true;
         break;
       case SDL_MOUSEMOTION:
         mouseX = event.motion.x;
         mouseY = event.motion.y;
         if (leftMouseButton) {
           paintPixel(pixels, mouseX, mouseY, brushSize, currentColor);
         }
         break;
       case SDL_MOUSEBUTTONDOWN:
         switch (event.button.button) {
           case SDL_BUTTON_LEFT:
             leftMouseButton = true;
             copyPixels(pixels,undoPixels);
             if (mouseX > SCREEN_WIDTH - MENU_HEIGHT && mouseX < SCREEN_WIDTH && mouseY > 0 && mouseY < MENU_HEIGHT) {
               currentColor = COLORS[0];
             }
             for (size_t i = 1; i <= NUMBER_OF_COLORS - 1; i++) {
                if (mouseX > SCREEN_WIDTH - (i + 1) * MENU_HEIGHT && mouseX < SCREEN_WIDTH - i * MENU_HEIGHT && mouseY > 0 && mouseY < MENU_HEIGHT) {
                    currentColor = COLORS[i];
                }
             }
             break;
           case SDL_BUTTON_RIGHT:
             copyPixels(pixels,undoPixels);
             Uint32 oldColor = pixels[mouseY * SCREEN_WIDTH + mouseX];
             queueFloodFill4(pixels, mouseX, mouseY, oldColor, currentColor);
             break;
         }
         break;
       case SDL_MOUSEBUTTONUP:
         if (event.button.button == SDL_BUTTON_LEFT) {
           leftMouseButton = false;
         }
         break;
       case SDL_MOUSEWHEEL: // Paint brush size change, incremented or decremented while rolling mouse wheel.
         if (event.wheel.y == 1) {
           brushSize++;
         }
         else if (event.wheel.y == -1) {
           brushSize--;
         }
         break;
       case SDL_KEYDOWN:
       switch (event.key.keysym.sym) {
         case SDLK_1:
            readImage(pixels,"samples/sample1.pix");
            menu.printColorMenu(pixels);
            break;
         case SDLK_2:
            readImage(pixels,"samples/sample2.pix");
            menu.printColorMenu(pixels);
            break;
         case SDLK_3:
            readImage(pixels,"samples/sample3.pix");
            menu.printColorMenu(pixels);
            break;
        case SDLK_4:
            readImage(pixels,"samples/sample4.pix");
            menu.printColorMenu(pixels);
            break;
         case SDLK_u:
            swapPixels(pixels, undoPixels);
            break;
         case SDLK_s:
            saveImage(pixels);
            break;
         case SDLK_l:
            readImage(pixels,"save.pix");
            break;
         case SDLK_UP:
            currentColor -= 0x000500;
            break;
         case SDLK_DOWN:
            currentColor += 0x000500;
            break;
         case SDLK_RSHIFT: // Reset the canvas.
           copyPixels(pixels, undoPixels);
           setCanvasBackground(pixels, 255);
           menu.printColorMenu(pixels);
           break;
       }
       break;
      }
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  // Essential cleanup
  setCanvasBackground(pixels, 255);
  setCanvasBackground(undoPixels, 255);
  delete[] pixels;
  delete[] undoPixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
