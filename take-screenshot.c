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

#include <Windows.h>
#include "bitmap.h"

int takeScreenshot(LPTSTR filename)
{
	//HWND desktop = GetDesktopWindow();
	HDC desktopdc = GetDC(NULL);  //GetDC(desktop);
	HDC compatdc = CreateCompatibleDC(desktopdc);

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	HBITMAP bitmap = CreateCompatibleBitmap(desktopdc, width, height);
	HBITMAP bitmap_old = SelectObject(compatdc, bitmap);

	BitBlt(compatdc, 0, 0, width, height, desktopdc, 0, 0, SRCCOPY|CAPTUREBLT);
	bitmap = SelectObject(compatdc, bitmap_old);

	int ret = createBitmapFile(filename, bitmap, compatdc);

	DeleteDC(desktopdc);
	DeleteDC(compatdc);

	return ret;
}
