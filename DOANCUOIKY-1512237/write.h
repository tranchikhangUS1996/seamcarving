#ifndef Write
#define Write
#include "bmp.h" 
#include <stdio.h>
#include <stdlib.h>

void writeimage(FILE*f, PixelArray data, BmpHeader header, BmpDib dib);

#endif


