#include "files.h"

using namespace cimg_library;

class CImgRGB {
	private:
		Uint16 R = 0, G = 0, B = 0;
	public:
		CImgRGB(Uint32);
		CImgRGB(Uint16, Uint16, Uint16);
		void getRGBToBitmap(CImg<Uint16>*, size_t, size_t);
		Uint32 getRGBcolor();
};

CImgRGB::CImgRGB(Uint32 color) {
	R = (Uint16)(color >> 16);
	G = (Uint16)(color >> 8);
	B = (Uint16)color;
}
CImgRGB::CImgRGB(Uint16 R, Uint16 G, Uint16 B) {
	this->R = R;
	this->G = G;
	this->B = B;
}

void CImgRGB::getRGBToBitmap(CImg<Uint16> *dest, size_t x, size_t y) {
	(*dest)(x, y, 0, 0) = R;
	(*dest)(x, y, 0, 1) = G;
	(*dest)(x, y, 0, 2) = B;
}

Uint32 CImgRGB::getRGBcolor() {
	return ((Uint32)R << 16) + ((Uint32)G << 8) + (Uint32)B;
}

// Create a bitmap of an pixel array.
void saveImage(Uint32 *pixels, const char *fileName) {
	FILE *image = fopen(fileName, "w");
	if (image) {
		fclose(image);
		CImg<Uint16> dest(SCREEN_WIDTH, SCREEN_HEIGHT - MENU_HEIGHT, 1, 3);
		for (size_t j = 0; j < SCREEN_HEIGHT - MENU_HEIGHT; j++) {
			for (size_t i = 0; i < SCREEN_WIDTH; i++) {
				CImgRGB color(pixels[i + (j + MENU_HEIGHT) * SCREEN_WIDTH]);
				color.getRGBToBitmap(&dest, i, j);
			}
		}
		dest.save(fileName);
	}
	else {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Saving file error", "An error occurred while saving the file.", NULL);
	}
}

// Read the bitmap from the specified file.
void readImage(Uint32 *pixels, const char *filePath) {
  if (cimg_library::cimg::is_file(filePath)) {
    CImg<Uint16> src(filePath);
    size_t width = src.width();
		size_t height = src.height();
		if (width * (height + MENU_HEIGHT) > SCREEN_HEIGHT*SCREEN_WIDTH) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Reading file error", "Input file is too large for the specified screen dimensions.", NULL);
			return;
		}
		else {
			for (size_t j = 0; j < height && j < SCREEN_HEIGHT - MENU_HEIGHT; j++) {
				for (size_t i = 0; i < width && i < SCREEN_WIDTH; i++) {
					if (i + (j + MENU_HEIGHT) * SCREEN_WIDTH <= SCREEN_HEIGHT * SCREEN_WIDTH) {
						CImgRGB color(src(i, j, 0, 0), src(i, j, 0, 1), src(i, j, 0, 2));
						pixels[i + (j + MENU_HEIGHT) * SCREEN_WIDTH] = color.getRGBcolor();
					}
				}
			}
		}
	}
	else {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing file", "Can't open specified file. Please check if it's present in the program directory.", NULL);
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
