#ifndef read
#define read
#include <stdio.h>
#include "bmp.h"
bool isBmpFile(FILE *f);
void readBmpHeader(FILE *f, BmpHeader &header);
void readBmpDib(FILE *f, BmpDib &dib);
void readBmpPixelArray(FILE *f, BmpHeader header, BmpDib dib, PixelArray &data);
void scanBmpPixelLine(FILE *f, Color *&line, uint32_t length);
void skipBmpPadding(FILE *f, char count);
void releaseBmpPixelArray(PixelArray data);
#endif
