/**
 * @file
 * @date 8 June 2018
 * @brief File contains functions used during drawing interactions with canvas.
 *
 */

#ifndef DRAWING_H
#define DRAWING_H
#include <queue>
#include <math.h>
#include <SDL2/SDL.h>

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;
extern const size_t MENU_HEIGHT;
extern const size_t NUMBER_OF_COLORS;


 /**
 * @brief Function used to fill an area with specified color.
 *
 * @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
 * @param size_t mouseX Value of X axis of the mouse.
 * @param size_t mouseY Value of Y axis of the mouse.
 * @param Uint32 oldColor Value of the color that is currently in the canvas at specified point.
 * @param Uint32 newColor Value of the color that is going to be used while filling.
 */

void queueFloodFill4(Uint32 *pixels, size_t mouseX, size_t mouseY,
                     Uint32 oldColor, Uint32 newColor);

/**
* @brief Function used to imitate the brush tool.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param size_t mouseX Value of X axis of the mouse.
* @param size_t mouseY Value of Y axis of the mouse.
* @param int brushSize Size of the brush.
* @param Uint32 currentColor Value of the color that is going to be used while painting.
*/

void paintPixel(Uint32 *pixels, size_t mouseX, size_t mouseY,
                int brushSize, Uint32 currentColor);

/**
* @brief Function used to paint the line between two points.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param size_t startMouseX Value of X axis of the mouse in first point.
* @param size_t startMouseY Value of Y axis of the mouse in first point.
* @param size_t endMouseX Value of X axis of the mouse in second point.
* @param size_t endMouseY Value of Y axis of the mouse in second point.
* @param int brushSize Size of the brush.
* @param Uint32 currentColor Value of the color that is going to be used while painting.
*/

void paintLine(Uint32 *pixels, size_t startMouseX, size_t startMouseY,
               size_t endMouseX, size_t endMouseY, int brushSize, Uint32 currentColor);

/**
* @brief Function used to paint the circle between two points.
*
* @param[out] Uint32 *pixels Pointer to array of pixels used as a canvas.
* @param size_t startMouseX Value of X axis of the mouse in first point.
* @param size_t startMouseY Value of Y axis of the mouse in first point.
* @param size_t endMouseX Value of X axis of the mouse in second point.
* @param size_t endMouseY Value of Y axis of the mouse in second point .
* @param int brushSize Size of the brush.
* @param Uint32 currentColor Value of the color that is going to be used while painting.
*/

void paintCircle(Uint32 *pixels, size_t startMouseX, size_t startMouseY,
                 size_t endMouseX, size_t endMouseY, int brushSize, Uint32 currentColor);

#endif
