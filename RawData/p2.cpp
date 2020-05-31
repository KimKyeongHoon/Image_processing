#include <iostream>
#include <Windows.h>
using namespace std;
#define size 512
#define source map[size*i+j]
#define raw "rawdata.raw"
#define input "lena_bmp_512x512.bmp"
#define output "out_raw.bmp"
#define uc unsigned char
int main()
{
	uc map[size * size];
	FILE* in_raw, * in, * out;
	fopen_s(&in_raw, raw, "rb");
	fopen_s(&in, input, "rb");
	fopen_s(&out, output, "wb");

	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hlnf;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, in); //14바이트
	fread(&hlnf, sizeof(BITMAPINFOHEADER), 1, in); //40바이트
	hlnf.biBitCount = 8;

	RGBQUAD hRGB[256];
	for (int i = 0; i < 256; i++)
	{
		hRGB[i].rgbRed = i;
		hRGB[i].rgbGreen = i;
		hRGB[i].rgbBlue = i;
	}
	fread(map, sizeof(uc), size * size, in_raw);

	fwrite(&hf, sizeof(uc), sizeof(BITMAPFILEHEADER), out);
	fwrite(&hlnf, sizeof(uc), sizeof(BITMAPINFOHEADER), out);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out);
	fwrite(map, sizeof(uc), size * size, out);

	fclose(in_raw),fclose(in),fclose(out);
}