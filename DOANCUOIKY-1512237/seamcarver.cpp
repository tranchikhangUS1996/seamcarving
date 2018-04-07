#include "seamcaver.h"
#include <math.h>

int computing(Color c1, Color c2) {
	float red = (c1.red - c2.red);
	float green = (c1.green - c2.green);
	float blue = (c1.blue - c2.blue);
	return (int)sqrt((red*red + green*green + blue*blue));
}
 
void computingenergy(PixelArray&data, int**&energy, int**mask) {
	energy = NULL;
	energy = new int*[data.rowCount];
	for (int i = 0; i<data.rowCount; i++) {
		energy[i] = new int[data.columnCount];
		for (int j = 0; j<data.columnCount; j++) {
			if (mask[i][j] == 0)
				energy[i][j] = -255 * 255;
			else  {
				int left = (j == 0 ? data.columnCount - 1 : j - 1);
				int right = (j == data.columnCount - 1 ? 0 : j + 1);
				int below = (i == data.rowCount - 1 ? 0 : i + 1);
				int above = (i == 0 ? data.rowCount - 1 : i - 1);
				int dx = computing(data.pixels[i][right], data.pixels[i][left]);
				int dy = computing(data.pixels[below][j], data.pixels[above][j]);
				energy[i][j] = dx + dy;

			}
		}
	}
}

/*
ham tim tat ca lat cat(seam)
*/
void findallverticalseams(int**&seammap, int**energy, int row, int col) {
	seammap = new int*[col];
	for (int i = 0; i<row; i++) seammap[i] = new int[row];

	for (int i = row - 2; i >= 0; i--)
	{
		for (int j = 0; j<col; j++)
		{
			int min = energy[i][j] + energy[i + 1][j];
			for (int a = -1; a <= 1; a++)
			{
				int next = j + a;
				if (next >= 0 && next<col)
				{
					if (min>energy[i][j] + energy[i + 1][next])
						min = energy[i][j] + energy[i + 1][next];
				}
			}
			energy[i][j] = min;
		}
	}
	//
	// tim tat ca duong seam
	for (int i = 0; i<col; i++)
	{
		seammap[i][0] = i;
		for (int j = 1; j<row; j++)
		{
			int index = seammap[i][j - 1]; 
			for (int a = -1; a <= 1; a++) 
			{
				int next = seammap[i][j - 1] + a;
				if (next >= 0 && next<col)
				{
					if (energy[j][next]<energy[j][index]) index = next;
				}
			}
			seammap[i][j] = index;
		}
	}
}

void findhorizontalseam(int**energy, int*&horizontalseam, int row, int col) {
	horizontalseam = new int[col];
	for (int i = col - 2; i >= 0; i--) {
		for (int j = 0; j<row; j++) {
			int min = energy[j][i] + energy[j][i+1];
			for (int a = -1; a <= 1; a++) {
				int next = j + a;
				if (next >= 0 && next<row) {
					if (min>energy[j][i] + energy[next][i+1])
						min = energy[j][i] + energy[next][i+1];
				}
			}
			energy[j][i] = min;
		}
	}

	horizontalseam[0] = 0;
	for (int i = 0; i<row; i++) {
		if (energy[horizontalseam[0]][0]>energy[i][0]) horizontalseam[0] = i;
	}
	for (int i = 1; i<col; i++) {
		int index = horizontalseam[i - 1];
		for (int j = -1; j <= 1; j++) {
			int next = horizontalseam[i - 1] + j;
			if (next >= 0 && next<row) {
				if (energy[next][i]<energy[index][i]) index = next;
			}
		}
		horizontalseam[i] = index;
	}
}

void findseammin(int**energy, int*&seam, int row, int col) {
	seam = new int[row]; 
	for (int i = row - 2; i >= 0; i--) {
		for (int j = 0; j<col; j++) {
			int min = energy[i][j] + energy[i + 1][j];
			for (int a = -1; a <= 1; a++) {
				int next = j + a;
				if (next >= 0 && next<col) {
					if (min>energy[i][j] + energy[i + 1][next])
						min = energy[i][j] + energy[i + 1][next];
				}
			}
			energy[i][j] = min;
		}
	}

	seam[0] = 0;
	for (int i = 0; i<col; i++) {
		if (energy[0][seam[0]]>energy[0][i]) seam[0] = i;
	}
	for (int i = 1; i<row; i++) {
		int index = seam[i - 1];
		for (int j = -1; j <= 1; j++) {
			int next = seam[i - 1] + j;
			if (next >= 0 && next<col) {
				if (energy[i][next]<energy[i][index]) index = next;
			}
		}
		seam[i] = index;
	}
}

void removehorizontal(PixelArray&data, int**mask) {
	int temp = data.rowCount;
	int**energy = NULL;
	int*seammin = NULL;
	computingenergy(data, energy, mask);
	findhorizontalseam(energy, seammin, data.rowCount, data.columnCount);
	for (int i = 0; i<data.columnCount; i++) {
		for (int j = seammin[i]; j<data.rowCount - 1; j++) {
			data.pixels[j][i] = data.pixels[j+1][i];
			mask[j][i] = mask[j+1][i];
		}
	}
	data.rowCount--;
	for (int i = 0; i<temp; i++) {
		delete[]energy[i];
	}
	delete[]energy;
	delete[]seammin;
}

void removeseam(PixelArray&data, int**mask) {
	int**energy = NULL;
	int*seammin = NULL;
	computingenergy(data, energy, mask);
	findseammin(energy, seammin, data.rowCount, data.columnCount);
	for (int i = 0; i<data.rowCount; i++) {
		for (int j = seammin[i]; j<data.columnCount - 1; j++) {
			data.pixels[i][j] = data.pixels[i][j + 1];
			mask[i][j] = mask[i][j + 1];
		}
	}
	data.columnCount--;
	for (int i = 0; i<data.rowCount; i++) {
		delete[]energy[i];
	}
	delete[]energy;
	delete[]seammin;
}

bool check(int**mask, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (mask[i][j] == 0) return true;
		}
	}
	return false;
}

int choose_vertical_or_herizontal(int**mask,int row,int col) {
	int vertical = 0, horizontal = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (mask[i][j] == 0) {
				horizontal++;
			}
		}
	}
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			if (mask[j][i] == 0) {
				vertical++;
			}
		}
	}
	return horizontal > vertical;
}

void removeobject(PixelArray&data, int**mask) {
	while (check(mask, data.rowCount, data.columnCount)) {
		if (choose_vertical_or_herizontal(mask, data.rowCount, data.columnCount))
		removeseam(data, mask); 
		else 
		removehorizontal(data, mask);
	}
}





