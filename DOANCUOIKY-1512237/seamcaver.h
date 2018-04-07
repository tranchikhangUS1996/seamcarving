#ifndef seamcaver 
#define seamcaver
#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>

void removeobject(PixelArray&data, int**mask);
bool check(int**mask, int row, int col);
void removeseam(PixelArray&data, int**mask);
void findseammin(int**energy, int*&seam, int row, int col);
void findallverticalseams(int**&seammap, int**energy, int row, int col);
void computingenergy(PixelArray&data, int**&energy, int**mask);
int computing(Color c1, Color c2);
void removehorizontal(PixelArray&data, int**mask);
void findhorizontalseam(int**energy, int*&horizontalseam, int row, int col);
int choose_vertical_or_herizontal(int**mask, int row, int col);

#endif
