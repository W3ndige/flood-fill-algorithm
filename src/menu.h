/**
 * @file
 * @date 8 June 2018
 * @brief File contains functions used while drawing the upper menu.
 *
 */


#ifndef MENU_H
#define MENU_H
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;
extern const Uint32 COLORS[];

/**
* @brief Function used to fill an area with specified color.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param Uint32 currentColor Value of the current color.
*/

void printCurrentColor(Uint32 *pixels, Uint32 currentColor);

/**
* @brief Print all the possible colors.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
*/

void printColorMenu(Uint32 *pixels);

/**
* @brief Print all the possible colors.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param int brushSize Current size of the brush
* @param Uint32 currentColor Value of the current color.
*/

void printCurrentBrush(Uint32 *pixels, int brushSize, Uint32 currentColor);

/**
* @brief Reset the field that is used to print the size of the current brush.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
*/

void resetCurrentBrushBackground(Uint32 *pixels);

#endif
