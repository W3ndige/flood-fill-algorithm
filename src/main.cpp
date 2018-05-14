#include <queue>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "menu.h"
#include "drawing.h"
#include "gadgets.h"
#include "files.h"

// TODO(W3ndige): Convert to 2D array.
// TODO(W3ndige): PPM or image converters.
// TODO(W3ndige): Modify straght line drawing in order to allow chaining lines.

const size_t SCREEN_WIDTH  = 720;
const size_t SCREEN_HEIGHT = 480;
const size_t MENU_HEIGHT = 30;
const size_t NUMBER_OF_COLORS = 20;

const Uint32 COLORS[NUMBER_OF_COLORS] = {0xFF000000, 0xC0C0C0, 0x808080, 0xFFFFFF, 0x800000, 0xFF0000, 0xFFA500, 0x800080, 0xC969FF, 0xFF69B4, 0x008000, 0x08E100, 0x93FF2D, 0xC3C355, 0xFFFF00, 0xFFFF83, 0x000080, 0x0000FF, 0x008080, 0x00FFFF};

int main(int argc, char *argv[]) {
  // Initialize SDL2.
 if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
 bool dKeyPressed = false;
 bool drawLineFirstPoint = false;
 bool drawLineEndPoint = false;
 bool drawCircleFirstPoint = false;
 bool drawCircleEndPoint = false;
 bool cKeyPressed = false;
 bool stopRender = false;
 int saveState = 3;
 Uint32 currentColor = 0;
 int brushSize = 5;

 SDL_Event event;
 printColorMenu(pixels); // Don't need to update this part of menu.
 copyPixels(pixels, undoPixels);

 size_t mouseX = 0;
 size_t mouseY = 0;
 size_t tmpMouseX = 0;
 size_t tmpMouseY = 0;

 while (!end) {
   if (SDL_WaitEvent(&event)) {

     // Possible optimization?
     printCurrentColor(pixels, currentColor);
     resetCurrentBrushBackground(pixels);
     printCurrentBrush(pixels,brushSize,currentColor);
     SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
     stopRender = false;

     switch (event.type) {
       case SDL_QUIT:
         end = true;
         break;
       case SDL_MOUSEMOTION:
         mouseX = event.motion.x;
         mouseY = event.motion.y;
         stopRender = true;
         if (leftMouseButton) {
           stopRender = false;
           paintPixel(pixels, mouseX, mouseY, brushSize, currentColor);
         }
         if (drawLineFirstPoint && (!drawLineEndPoint)) {
           stopRender = false;
             copyPixels(undoPixels, pixels);
             paintLine(pixels, tmpMouseX, tmpMouseY, mouseX, mouseY, brushSize, currentColor);
         }
         if (drawCircleFirstPoint && (!drawCircleEndPoint)) {
           stopRender = false;
           copyPixels(undoPixels, pixels);
           paintCircle(pixels, tmpMouseX, tmpMouseY, mouseX, mouseY, brushSize, currentColor);
         }
         break;
       case SDL_MOUSEBUTTONDOWN:
         switch (event.button.button) {
             case SDL_BUTTON_LEFT:
             if (dKeyPressed) {
                 tmpMouseX = mouseX;
                 tmpMouseY = mouseY;
                 drawLineFirstPoint = true;
                 dKeyPressed = false;
                 break;
             }
             if (drawLineFirstPoint) {
                 drawLineFirstPoint = false;
                 drawLineEndPoint = true;
                 break;
             }
             if (cKeyPressed) {
   						tmpMouseX = mouseX;
   						tmpMouseY = mouseY;
   						drawCircleFirstPoint = true;
   						cKeyPressed = false;
   						copyPixels(pixels, undoPixels);
   						break;
   					}
   					if (drawCircleFirstPoint) {
   						drawCircleFirstPoint = false;
   						drawCircleEndPoint = true;
   						break;
   					}
             leftMouseButton = true;
             copyPixels(pixels,undoPixels);
             paintPixel(pixels, mouseX, mouseY, brushSize, currentColor);
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
           if (brushSize < 10) {
             brushSize++;
          }
         }
         else if (event.wheel.y == -1) {
           if (brushSize > 3) {
            brushSize--;
           }
         }
         break;
       case SDL_KEYDOWN:
       switch (event.key.keysym.sym) {
         case SDLK_1:
            saveManager(pixels, saveState, "saves/save1.pix", "samples/sample1.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
         case SDLK_2:
            saveManager(pixels, saveState, "saves/save2.pix", "samples/sample2.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
         case SDLK_3:
            saveManager(pixels, saveState, "saves/save3.pix", "samples/sample3.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
        case SDLK_4:
            saveManager(pixels, saveState, "saves/save4.pix", "samples/sample4.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
        case SDLK_5:
            saveManager(pixels, saveState, "saves/save5.pix", "samples/sample5.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
        case SDLK_6:
            saveManager(pixels, saveState, "saves/save6.pix", "samples/sample6.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
        case SDLK_7:
            saveManager(pixels, saveState, "saves/save7.pix", "samples/sample7.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
        case SDLK_8:
            saveManager(pixels, saveState, "saves/save8.pix", "samples/sample8.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
        case SDLK_9:
            saveManager(pixels, saveState, "saves/save9.pix", "samples/sample9.pix");
            saveState = 3;
            printColorMenu(pixels);
            break;
         case SDLK_u:
            swapPixels(pixels, undoPixels);
            break;
         case SDLK_s:
            saveState = 1;
            break;
         case SDLK_l:
            saveState = 2;
            break;
         case SDLK_p:
            currentColor = colorPicker(pixels, mouseX, mouseY);
            break;
         case SDLK_c:
           if (drawCircleFirstPoint == true && drawCircleEndPoint == false) {
             drawCircleFirstPoint = false;
             swapPixels(pixels, undoPixels);
             break;
           }
           if (drawLineEndPoint) {
             drawCircleEndPoint = false;
           }
           if (!cKeyPressed) {
             cKeyPressed = true;
             break;
           }
           if (cKeyPressed) {
             cKeyPressed = false;
             break;
           }
           break;
         case SDLK_d:
            if (drawLineFirstPoint == true && drawLineEndPoint == false) {
              drawLineFirstPoint = false;
              swapPixels (pixels, undoPixels);
              break;
            }
            if (drawLineEndPoint) {
              drawLineEndPoint = false;
            }
            if (!dKeyPressed) {
              dKeyPressed = true;
              copyPixels(pixels,undoPixels);
              break;
            }
            if (dKeyPressed) {
              dKeyPressed = false;
              break;
            }
            break;
         case SDLK_UP:
            currentColor -= 0x000500;
            break;
         case SDLK_DOWN:
            currentColor += 0x000500;
            break;
         case SDLK_KP_PLUS:
            if (brushSize < 10) {
              brushSize++;
            }
            break;
         case SDLK_KP_MINUS:
             if (brushSize > 3) {
               brushSize--;
             }
            break;
         case SDLK_RSHIFT: // Reset the canvas.
           copyPixels(pixels, undoPixels);
           setCanvasBackground(pixels, 255);
           printColorMenu(pixels);
           break;
       }
       break;
     }
     if (!stopRender) {
       SDL_RenderClear(renderer);
       SDL_RenderCopy(renderer, texture, NULL, NULL);
       SDL_RenderPresent(renderer);
     }
    }
  }

  // Essential cleanup
  setCanvasBackground(pixels, 255); // Overwrite images from memory before making it free.
  setCanvasBackground(undoPixels, 255);
  delete[] pixels;
  delete[] undoPixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
