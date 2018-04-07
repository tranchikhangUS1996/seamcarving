#include "bmp.h"
#include "read.h"
#include "seamcaver.h"
#include "write.h"
#include <stdio.h>
#include <stdlib.h>

void creatmask(int**&mask,PixelArray data) {
	mask = new int*[data.rowCount];
	for (int i = 0; i < data.rowCount; i++) {
		mask[i] = new int[data.columnCount];
		for (int j = 0; j < data.columnCount; j++) {
			if (data.pixels[i][j].red == 0 && data.pixels[i][j].blue == 0 && data.pixels[i][j].green == 255) mask[i][j] = 0;
			else mask[i][j] = 1;
		}
	}
}

void main(int argv, char**argc) {
	BmpHeader header;
	BmpDib dib;
	PixelArray data;
	char soure[100] = "";
	int**mask = NULL;

	printf("nhap duog dan");
	gets(soure);
	FILE*f = fopen(soure,"rb");
	if(f==NULL) return;
	if(!isBmpFile(f)) {
		printf("Not BMP file!");
		return;
	}

	readBmpHeader(f, header);
	readBmpDib(f,dib);
	readBmpPixelArray(f,header,dib,data);
	fclose(f);
	int a = dib.imageHeight, b = dib.imageWidth;

	creatmask(mask, data); // tao anh mask
	removeobject(data, mask); // xoa doi tuong duoc danh dau

	printf("nhap duong dan luu\n");
	char out[100] = "";
	fflush(stdin);
	gets(out);
	FILE*f2 = fopen(out,"wb");
	writeimage(f2,data,header,dib);
	fclose(f2);

	data.rowCount = a,data.columnCount = b;
	for (int i = 0; i < data.rowCount; i++) {
		delete[]mask[i];
	}
	delete[]mask;

	releaseBmpPixelArray(data);
}