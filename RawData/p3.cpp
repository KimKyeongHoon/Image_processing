#include <iostream>
#include <windows.h>
using namespace std;

#define size 512
#define uc unsigned char

#define input "out_raw.bmp"
#define output "lena_lotate.bmp"

int main()
{
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInf;
	RGBQUAD hRGB[256];
	uc bef[size * size], aft[size*size];

	FILE* in,*out;
	fopen_s(&in, input, "rb");
	fopen_s(&out,output,"wb");

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, in);
	fread(&hInf, sizeof(BITMAPINFOHEADER), 1, in);
	fread(bef, sizeof(uc), size * size, in);

	for (int i = 0; i < 256; i++)hRGB[i].rgbRed = hRGB[i].rgbGreen = hRGB[i].rgbBlue = i;
	for (int i = 0; i < size; i++)for (int j = 0; j < size; j++)aft[i*size+j] = bef[j*size+i];

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hInf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);

	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(aft, sizeof(uc), size * size, out);

}