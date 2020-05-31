#include <iostream>
using namespace std;
#define size 512
#define source map[size*i+j]
#define output "rawdata.raw"
int main()
{
	unsigned char map[size*size];
	FILE* out;
	fopen_s(&out, output, "wb");

	for (int i = 0; i < size; i++)for (int j = 0; j < size; j++)
		if (j < 100)source = 120;
		else if (j < 200)source = (15 * j / 100) + 105;
		else if (j < 280)source = (90 * j)/80 - 90;
		else if (j < 300)source = (15 * j)/20 + 15;
		else source = 240;

	fwrite(map, sizeof(unsigned char), size*size, out);
	fclose(out);
}