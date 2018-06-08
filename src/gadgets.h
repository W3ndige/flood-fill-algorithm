/**
 * @file
 * @date 8 June 2018
 * @brief File contains functions used as different tools.
 *
 */

#ifndef GADGETS_H
#define GADGETS_H
#include <string.h>
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;
extern const Uint32 COLORS[];

/**
* @brief Function used pick a color from a specified point.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param size_t mouseX Value of X axis of the mouse.
* @param size_t mouseY Value of Y axis of the mouse.
* @return Return a color that was at certain position.
*/

Uint32 colorPicker(Uint32 *pixels, size_t mouseX, size_t mouseY);

/**
* @brief Function used to set the color of the canvas.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param Uint32 color Color with which we fill the canvas.

*/

void setCanvasBackground(Uint32 *pixels, Uint32 color);

/**
* @brief Function used to copy the canvas to the undo pixels.
*
* @param[in] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param[out] Uint32 *undoPixels Pointer to pixels used as the backup.

*/

void copyPixels(Uint32 *pixels, Uint32 *undoPixels);

/**
* @brief Exchange the values between pixels and undoPixels.
*
* @param[in] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param[out] Uint32 *undoPixels Pointer to pixels used as the backup.

*/


void swapPixels(Uint32 *pixels, Uint32 *undoPixels);

/**
* @brief Copy smaller area of the canvas.
*
* @param[in] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param[out] Uint32 *undoPixels Pointer to pixels used as the backup.
* @param size_t startMouseX Value of X axis of the mouse in first point.
* @param size_t startMouseY Value of Y axis of the mouse in first point.
* @param size_t endMouseX Value of X axis of the mouse in second point.
* @param size_t endMouseY Value of Y axis of the mouse in second point.
*/


void cutPixels(Uint32 *pixels, Uint32 *copyPastePixels, size_t startMouseX,
               size_t startMouseY, size_t endMouseX, size_t endMouseY);

/**
* @brief Paste the smaller area of pixels to the canvas.
*
* @param[in] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param[out] Uint32 *undoPixels Pointer to pixels used as the backup.
*/

void pastePixels(Uint32 *pixels, Uint32 *copyPastePixels, size_t *copyMouseCoordinates);
#endif
