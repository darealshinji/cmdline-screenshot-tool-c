/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * Contributors:
 *   2016 djcj <djcj@gmx.de>
 */

/*
 * screenshot-simple.c: Save a screenshot of the Windows desktop in .bmp format.
 *  The image will be saved as "screenshot.bmp" in the current working directory.
 *
 *  All linker dependencies are specified explicitly in this file, so you can
 *  compile screenshot-simple.exe by simply running:
 *    cl screenshot-simple.c
 *
 *  To compile with GCC run:
 *    gcc -Wall -Wextra -pedantic -O3 screenshot-simple.c -s -static -o screenshot-simple.exe -lgdi32
 */

#ifndef __GNUC__
#  pragma comment(lib, "user32.lib")
#  pragma comment(lib, "gdi32.lib")
#endif

#include <Windows.h>
#include <stdio.h>

#include "bitmap.c"
#include "take-screenshot.c"


int main(void)
{
	LPTSTR filename = "screenshot.bmp";

	if (takeScreenshot(filename) == 0)
	{
		printf("Screenshot successfully saved.");
	}
	else
	{
		printf("Problem saving screenshot.");
		return -1;
	}

	return 0;
}
