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

int createBitmapFile(LPCTSTR lpszFileName, HBITMAP hBitmap, HDC hdc)
{
  BITMAP            bitmap;
  BITMAPFILEHEADER  bitmapFileHeader;

  if (!GetObject(hBitmap, sizeof(BITMAP), &bitmap))
  {
    return -1;
  }

  int infoHeaderSize      = sizeof(BITMAPINFOHEADER);
  int rgbQuadSize         = sizeof(RGBQUAD);
  int bits                = bitmap.bmPlanes * bitmap.bmBitsPixel;
  int infoHeaderSizeImage = ((bitmap.bmWidth * bits + 31) &~31) / 8 * bitmap.bmHeight;

  PBITMAPINFO pBitmapInfo = (PBITMAPINFO) LocalAlloc(LPTR,
                                                     infoHeaderSize +
                                                     rgbQuadSize * (1 << bits));

  pBitmapInfo->bmiHeader.biSize         = infoHeaderSize;
  pBitmapInfo->bmiHeader.biBitCount     = bitmap.bmBitsPixel;
  pBitmapInfo->bmiHeader.biClrImportant = 0;
  pBitmapInfo->bmiHeader.biClrUsed      = 0;
  pBitmapInfo->bmiHeader.biCompression  = BI_RGB;
  pBitmapInfo->bmiHeader.biHeight       = bitmap.bmHeight;
  pBitmapInfo->bmiHeader.biWidth        = bitmap.bmWidth;
  pBitmapInfo->bmiHeader.biPlanes       = bitmap.bmPlanes;
  pBitmapInfo->bmiHeader.biSizeImage    = infoHeaderSizeImage;

  PBITMAPINFOHEADER pBitmapInfoHeader = (PBITMAPINFOHEADER) pBitmapInfo;
  LPBYTE mem = GlobalAlloc(GMEM_FIXED, pBitmapInfoHeader->biSizeImage);

  if (!mem)
  {
    return -1;
  }

  if (!GetDIBits(hdc,
                 hBitmap,
                 0,
                 pBitmapInfoHeader->biHeight,
                 mem,
                 pBitmapInfo,
                 DIB_RGB_COLORS))
  {
    return -1;
  }

  int fileHeaderSize        = sizeof(BITMAPFILEHEADER);
  int fileAndInfoHeaderSize = fileHeaderSize + infoHeaderSize +
                              pBitmapInfoHeader->biClrUsed * rgbQuadSize;

  bitmapFileHeader.bfType      = 'B' + ('M' << 8);
  bitmapFileHeader.bfOffBits   = fileAndInfoHeaderSize;
  bitmapFileHeader.bfSize      = fileAndInfoHeaderSize + pBitmapInfoHeader->biSizeImage;
  bitmapFileHeader.bfReserved1 = 0;
  bitmapFileHeader.bfReserved2 = 0;

  HANDLE handleFile = CreateFile(lpszFileName,
                                 GENERIC_WRITE,
                                 0,
                                 NULL,
                                 CREATE_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);

  if (handleFile == INVALID_HANDLE_VALUE)
  {
    return -1;
  }

  long unsigned int dwWritten = 0;

  if (!WriteFile(handleFile,
                 &bitmapFileHeader,
                 fileHeaderSize,
                 &dwWritten,
                 NULL))
  {
    return -1;
  }

  if (!WriteFile(handleFile,
                 pBitmapInfoHeader,
                 infoHeaderSize + pBitmapInfoHeader->biClrUsed * rgbQuadSize,
                 &dwWritten,
                 NULL))
  {
    return -1;
  }

  if (!WriteFile(handleFile,
                 mem,
                 pBitmapInfoHeader->biSizeImage,
                 &dwWritten,
                 NULL))
  {
    return -1;
  }

  if (!CloseHandle(handleFile))
  {
    return -1;
  }

  GlobalFree(mem);
  return 0;
}
