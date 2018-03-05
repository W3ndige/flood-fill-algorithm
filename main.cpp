#include <stdio.h>
#include <string>
#include <queue>
#include <SDL2/SDL.h>

// TODO(W3ndige): Implement importing colouring pages.
// TODO(W3ndige): Optimization.

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int MENU_HEIGHT = 20;

const Uint32 COLOR_BLACK = 0xFF000000;
const Uint32 COLOR_WHITE = 0xFFFFFFFF;
const Uint32 COLOR_RED = 0xFFFF0000;
const Uint32 COLOR_GREEN = 0xFF00FF00;
const Uint32 COLOR_BLUE = 0xFF0000FF;

// Menu object that will work for the line of 20 pixels height, at the top of the window.
class Menu {
  public:
    void printCurrentColor(Uint32 *pixels, Uint32 current_color) {
      for (int i = 0; i < MENU_HEIGHT; i++) {
        for (int j = 0; j < MENU_HEIGHT; j++) {
          pixels[j + i * SCREEN_WIDTH] = current_color;
        }
      }
    }
};

// TODO(W3ndige): Investigate too big recursive depth, stack is filled to quickly and the program will crash on some instances.
// http://www.vioja.com/how-to-fix-a-recursive-flood-fill-segmentation-error-closed/
// https://stackoverflow.com/questions/36497014/recursive-floodfill-segmentation-fault/36497640
void recursiveFloodFill4(Uint32 *pixels, int mouseX, int mouseY, Uint32 old_color, Uint32 new_color) {
  if ((mouseX < SCREEN_WIDTH && mouseX >= 0) && (mouseY < SCREEN_HEIGHT && mouseY >= MENU_HEIGHT)) { // Check boundaries.
    if ((old_color == pixels[mouseY * SCREEN_WIDTH + mouseX]) && (new_color != pixels[mouseY * SCREEN_WIDTH + mouseX])) { // Check if the old color matches the color in the location AND check if the color in the locations is not the same as the new one.
      pixels[mouseY * SCREEN_WIDTH + mouseX] = new_color; // Add a new pixel, and call new instances of floodFill algorithm.
      //printf("X: %d, Y: %d\n", mouseX, mouseY);
      recursiveFloodFill4(pixels, mouseX - 1, mouseY, old_color, new_color);
      recursiveFloodFill4(pixels, mouseX + 1, mouseY, old_color, new_color);
      recursiveFloodFill4(pixels, mouseX, mouseY + 1, old_color, new_color);
      recursiveFloodFill4(pixels, mouseX, mouseY - 1, old_color, new_color);
    }
  }
}

// TODO(W3ndige): Experiment.
// More efficient version of flood fill algorithm based on a queue.
// https://en.wikipedia.org/wiki/Flood_fill#Alternative_implementations
void queueFloodFill4(Uint32 *pixels, int mouseX, int mouseY, Uint32 old_color, Uint32 new_color) {
  if (new_color == pixels[mouseY * SCREEN_WIDTH + mouseX]) {
    return;
  }

  struct coordinates { int mouseX, mouseY;};
  std::queue<coordinates> fill;
  fill.push({mouseX, mouseY});

  while (!fill.empty()) {
    coordinates top = fill.front();
    fill.pop();
    if (top.mouseX >= 0 && top.mouseX < SCREEN_WIDTH && top.mouseY >= MENU_HEIGHT && top.mouseY < SCREEN_HEIGHT && pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] == old_color) {
        pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] = new_color;
        fill.push({top.mouseX + 1, top.mouseY});
        fill.push({top.mouseX, top.mouseY + 1});
        fill.push({top.mouseX - 1, top.mouseY});
        fill.push({top.mouseX, top.mouseY - 1});
    }
  }
}

// Paint pixels with according size.
void paintPixel(Uint32 *pixels, int mouseX, int mouseY, int brush_size ,Uint32 current_color) {
  if (brush_size < 0) {
    brush_size = 1;
  }
  if (brush_size > 10) {
    brush_size = 10; // Maximum size
  }
  for (int i = mouseY; i < mouseY + brush_size; i++) {
    if (i >= MENU_HEIGHT) { // Leave space for menu.
      for (int j = mouseX; j < mouseX + brush_size; j++) {
        pixels[j + i * SCREEN_WIDTH] = current_color;
      }
    }
  }
}

void setCanvasBackground(Uint32 *pixels, Uint32 color) {
  memset(pixels, color, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
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

 Uint32 *pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; // Assign new set of pixels.
 SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
 setCanvasBackground(pixels, 255);

 // Essential variables
 bool end = false;
 bool leftMouseButton = false;

 Uint32 current_color = 0;
 int brush_size = 5;

 SDL_Event event;
 Menu menu;

 while (!end) {
   while (SDL_PollEvent(&event)) {

     // Experiment with this behaviour.
     SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
     menu.printCurrentColor(pixels, current_color);

     if (event.type == SDL_QUIT) {
       end = true;
       break;
     }

     // Keyboard events handler.
     if (event.type == SDL_KEYDOWN) {
       if (event.key.keysym.sym == SDLK_SPACE) {
         puts("Save to Image"); // TODO(W3ndige): Implement save image function.
       }

       // Reset the canvas
       if (event.key.keysym.sym == SDLK_BACKSPACE) {
         setCanvasBackground(pixels, 255);
       }

       // Use predefined colors
       if (event.key.keysym.sym == SDLK_1) {
         current_color = COLOR_BLACK;
       }
       if (event.key.keysym.sym == SDLK_2) {
         current_color = COLOR_WHITE;
       }
       if (event.key.keysym.sym == SDLK_3) {
         current_color = COLOR_RED;
       }
       if (event.key.keysym.sym == SDLK_4) {
         current_color = COLOR_GREEN;
       }
       if (event.key.keysym.sym == SDLK_5) {
         current_color = COLOR_BLUE;
       }
     }

     // Mouse events handler.
     if (event.type == SDL_MOUSEBUTTONUP) {
       if (event.button.button == SDL_BUTTON_LEFT) {
         leftMouseButton = false;
       }
     }

     if (event.type == SDL_MOUSEBUTTONDOWN) {
       if (event.button.button == SDL_BUTTON_LEFT) {
         leftMouseButton = true;
       }

       if (event.button.button == SDL_BUTTON_RIGHT) {
         int mouseX = event.motion.x;
         int mouseY = event.motion.y;
         Uint32 old_color = pixels[mouseY * SCREEN_WIDTH + mouseX];
         queueFloodFill4(pixels, mouseX, mouseY, old_color, current_color);
       }
     }

     if (event.type == SDL_MOUSEMOTION) {
       if (leftMouseButton) {
         int mouseX = event.motion.x;
         int mouseY = event.motion.y;
         paintPixel(pixels, mouseX, mouseY, brush_size, current_color);
       }
     }

     // Paint brush size change, incremented or decremented while rolling mouse wheel.
     if (event.type == SDL_MOUSEWHEEL) {
       if (event.wheel.y == 1) {
         brush_size++;
       }
       if (event.wheel.y == -1) {
         brush_size--;
       }
     }
     break;
   }
   SDL_RenderClear(renderer);
 	 SDL_RenderCopy(renderer, texture, NULL, NULL);
   SDL_RenderPresent(renderer);
  }

  // Essential cleanup
  delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
