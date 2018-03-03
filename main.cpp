#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

void floodFill(Uint32 *pixels, int mouseX, int mouseY, Uint32 old_color, Uint32 new_color) {
  if ((mouseX < SCREEN_WIDTH && mouseX > 0) && (mouseY < SCREEN_HEIGHT && mouseY > 0)) { // Check boundaries.
    if (old_color == pixels[mouseY * SCREEN_WIDTH + mouseX] && new_color != pixels[mouseY * SCREEN_WIDTH + mouseX]) { // Check if the old color matches the color in the location AND check if the color in the locations is not the same as the new one.
      pixels[mouseY * SCREEN_WIDTH + mouseX] = new_color;
      floodFill(pixels, mouseX - 1, mouseY, old_color, new_color);
      floodFill(pixels, mouseX + 1, mouseY, old_color, new_color);
      floodFill(pixels, mouseX, mouseY + 1, old_color, new_color);
      floodFill(pixels, mouseX, mouseY - 1, old_color, new_color);
    }
  }
}

int main() {

 if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
   printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
   SDL_Quit();
   return 1;
 }

 SDL_Window *window = SDL_CreateWindow("Flood Fill", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);

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

 SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, 480);

 Uint32 *pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; // Assign new set of pixels.
 memset(pixels, 255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); // Set the background color to white.

 bool end = false;
 bool leftMouseButton = false;
 SDL_Event event;

 while (!end) {
   SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
   while (SDL_PollEvent(&event)) {
     // Events handler
     if (event.type == SDL_QUIT) {
       end = true;
       break;
     }
     if (event.type == SDL_KEYDOWN) {
       if (event.key.keysym.sym == SDLK_SPACE) {
         puts("Save to Image");

       }
     }

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
         floodFill(pixels, mouseX, mouseY, old_color, 132);
       }
     }

     if (event.type == SDL_MOUSEMOTION) {
       if (leftMouseButton) {
         int mouseX = event.motion.x;
         int mouseY = event.motion.y;
         pixels[mouseY * SCREEN_WIDTH + mouseX] = 0;
       }
     }
     break;
   }

   SDL_RenderClear(renderer);
 	 SDL_RenderCopy(renderer, texture, NULL, NULL);
   SDL_RenderPresent(renderer);
  }

  free(pixels);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

}
