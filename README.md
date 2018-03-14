Simple Paint
============

Presentation of the Flood Fill algorithm in a simple Paint program written in C++, using SDL2 library.

Description
-----------

Because of the drawbacks of using recursive version of algorithm, we decided to use the one based on the queue. First one made it impossible to fill the full version of screen, as the number of functions called was to much for the stack and the program crashed with `stack overflow` error. Instead the one using queue seemed to be much faster than the recursive version, as well as using much less resources.

Firstly, we interpret 2D array of pixels as 1D array. Then we can access each pixels (X, Y) with simple formula `Y * screenWidth + X`. That's because the pixels are stored in such a format `[firstRow, firstRow, firstRow, secondRow, SecondRow, SecondRow, ...]`. In addition to only presenting the flood fill algorithm, we decided to implement some elements of the basic paint program, like painting pixels, changing colors, saving files and special function to load some predefined coloring pages.

Usage
-----

* `RSHIFT` to clean the screen
* `LEFT_MOUSE_BUTTON` to draw pixels
* `RIGHT_MOUSE_BUTTON` to fill area with color
* `SCROLL_WHEEL`, together with `+` and `-` keys to change the brush size
* `U` to undo or redo last operations
* `S` to save the current image in a format of a hexdump
* `L` to load from save
* `1`, `2`, `3`, `4` to load the preset samples
* `UP` or `DOWN` keys make the color more or less blue

License
-------

Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
