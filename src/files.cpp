#include "files.h"

// Create a hexdump of an pixel array.
void saveImage(Uint32 *pixels, const char *fileName) {
  FILE *image = fopen(fileName, "wb");
  if (image) {
    fwrite(pixels, sizeof(Uint32),SCREEN_WIDTH * SCREEN_HEIGHT, image);
    fclose(image);
  }
  else {
    puts("Can't open specified sample. Please check if it's present in the program directory.");
  }
}

// Read the hexdump from the specified file.
void readImage(Uint32 *pixels, const char *fileName) {
  FILE *image = fopen(fileName, "rb");
  if (image) {
    fseek(image, 0, SEEK_END);
    size_t size = ftell(image);
    fseek(image, 0, SEEK_SET);
    if (size <= SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)) { // Protect from loading to big files.
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

void saveManager(Uint32 *pixels, int saveState, const char *fileName, const char *sampleName) {
  if (saveState == 1) {
    saveImage(pixels, fileName);
    saveState = false;
  }
  else if (saveState == 2) {
    readImage(pixels, fileName);
  }
  else if (saveState == 3) {
    readImage(pixels, sampleName);
  }
}

void printControls() {
    FILE *helpfile = fopen("controls.md","r");
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
