/**
 * @file
 * @date 8 June 2018
 * @brief File contains functions used during the saving and reading images.
 *
 */

#ifndef FILES_H
#define FILES_H
#include <SDL2/SDL.h>
#include "CImg.h"

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;
extern const Uint32 COLORS[];

/**
* @brief Function used to save the image into the specified file.
*
* @param Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param const char *fileName Name of the save file.
*/

void saveImage(Uint32 *pixels, const char *fileName);

/**
* @brief Function used to read the image into from specified file.
*
* @param Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param const char *fileName Name of the save file.
*/

void readImage(Uint32 *pixels, const char *fileName);

/**
* @brief Function used to manage the state of save, with possible slots.
*
* @param Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param int saveState State of the save.
* @param const char *fileName Name of the save file.
* @param const char *sampleName Name of the sample.
*/

void saveManager(Uint32 *pixels, int saveState, const char *fileName,
                 const char *sampleName);

/**
* @brief Function used to print the controls.
*
*/

void printControls();

#endif
