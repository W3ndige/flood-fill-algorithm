#include <stdio.h>
#include <string>
#include <queue>
#include <SDL2/SDL.h>

// TODO(W3ndige): Implement importing colouring pages.
// TODO(W3ndige): Implement better brush algorithm.
// TODO(W3ndige): Undo, redo option
// TODO(W3ndige): Optimization.

const int SCREEN_WIDTH  = 720;
const int SCREEN_HEIGHT = 480;
const int MENU_HEIGHT = 20;

// BLACK, WHITE, RED, GREEN, BLUE
const Uint32 COLORS[5] = {0xFF000000, 0xFFFFFFFF, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF};

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
void queueFloodFill4(Uint32 *pixels, int mouseX, int mouseY, Uint32 *old_color, Uint32 *new_color) {
  if (*new_color == pixels[mouseY * SCREEN_WIDTH + mouseX]) {
    return;
  }

  struct coordinates { int mouseX, mouseY;};
  std::queue<coordinates> fill;
  fill.push({mouseX, mouseY});

  while (!fill.empty()) {
    coordinates top = fill.front();
    fill.pop();
    if (top.mouseX >= 0 && top.mouseX < SCREEN_WIDTH && top.mouseY >= MENU_HEIGHT && top.mouseY < SCREEN_HEIGHT && pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] == *old_color) {
        pixels[top.mouseY * SCREEN_WIDTH + top.mouseX] = *new_color;
        fill.push({top.mouseX + 1, top.mouseY});
        fill.push({top.mouseX, top.mouseY + 1});
        fill.push({top.mouseX - 1, top.mouseY});
        fill.push({top.mouseX, top.mouseY - 1});
    }
  }
}

// Paint pixels with according size.
void paintPixel(Uint32 *pixels, int mouseX, int mouseY, int brush_size, Uint32 current_color) {
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

void printControls(void) {
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
        puts("Can't open help file! Make sure it's present in the program folder.");
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
 SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
 setCanvasBackground(pixels, 255);

 // Essential variables
 bool end = false;
 bool leftMouseButton = false;

 Uint32 current_color = 0;
 int brush_size = 5;

 SDL_Event event;
 Menu menu;
 menu.printColorMenu(pixels); // Don't need to update this part of menu.

 int mouseX = 0;
 int mouseY = 0;

 while (!end) {
   while (SDL_PollEvent(&event)) {

     // Possible optimization?
     menu.printCurrentColor(pixels, current_color);
     SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

     switch (event.type) {
       case SDL_QUIT:
         end = true;
         break;
       case SDL_MOUSEMOTION:
         mouseX = event.motion.x;
         mouseY = event.motion.y;
         if (leftMouseButton) {
           paintPixel(pixels, mouseX, mouseY, brush_size, current_color);
         }
         break;
       case SDL_MOUSEBUTTONDOWN:
         switch (event.button.button) {
           case SDL_BUTTON_LEFT:
             leftMouseButton = true;
             if (mouseX > 700 && mouseX < 720 && mouseY > 0 && mouseY < MENU_HEIGHT) {
               current_color = COLORS[0];
             }
             if (mouseX > 680 && mouseX < 700 && mouseY > 0 && mouseY < MENU_HEIGHT) {
               current_color = COLORS[1];
             }
              if (mouseX > 660 && mouseX < 680 && mouseY > 0 && mouseY < MENU_HEIGHT) {
               current_color = COLORS[2];
             }
              if (mouseX > 640 && mouseX < 660 && mouseY > 0 && mouseY < MENU_HEIGHT) {
               current_color = COLORS[3];
             }
              if (mouseX > 620 && mouseX < 640 && mouseY > 0 && mouseY < MENU_HEIGHT) {
               current_color = COLORS[4];
             }
             break;
           case SDL_BUTTON_RIGHT:
             Uint32 old_color = pixels[mouseY * SCREEN_WIDTH + mouseX];
             queueFloodFill4(pixels, mouseX, mouseY, &old_color, &current_color);
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
           brush_size++;
         }
         else if (event.wheel.y == -1) {
           brush_size--;
         }
         break;
       case SDL_KEYDOWN:
       switch (event.key.keysym.sym) {
         case SDLK_1:
           current_color = COLORS[0];
           break;
         case SDLK_2:
           current_color = COLORS[1];
           break;
         case SDLK_3:
           current_color = COLORS[2];
           break;
         case SDLK_4:
           current_color = COLORS[3];
           break;
         case SDLK_5:
           current_color = COLORS[4];
           break;
         case SDLK_SPACE: // TODO(W3ndige): Implement save image function.
           puts("Save to Image");
           break;
         case SDLK_RSHIFT: // Reset the canvas.
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
  delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
