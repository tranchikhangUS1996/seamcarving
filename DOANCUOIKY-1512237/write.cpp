#include "write.h"

void writeimage(FILE*f,PixelArray data,BmpHeader header,BmpDib dib) {
	if(f==NULL) return;
	dib.imageHeight = data.rowCount;
	dib.imageWidth = data.columnCount;
	int padding = (4-(data.columnCount*(dib.pixelSize/8))%4)%4;
	dib.bitmapByteCount = data.rowCount*(data.columnCount*(dib.pixelSize/8)+padding);
	header.fileSize = 54+dib.bitmapByteCount;
	char pad[3];
	fseek(f,0,0L);
	fwrite(&header,sizeof(BmpHeader),1,f);
	fseek(f,sizeof(BmpHeader),0L);
	fwrite(&dib,sizeof(BmpDib),1,f);
	fseek(f,header.dataOffset,0L);
	for(int i = data.rowCount-1;i>=0;i--) {
		fwrite(data.pixels[i],sizeof(Color),data.columnCount,f);
		fwrite(pad,sizeof(char),padding,f);
	}
}

