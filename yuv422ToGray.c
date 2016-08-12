#include <stdio.h>

#define YUV422_SIZE 614400  // 640 * 480 * 2

typedef unsigned char u8;

int clamp(int val, int min, int max)
{
	if(val < min)
	{
		return min;
	}
	else if(val > max)
	{
		return max;
	}
	else
	{
		return val;
	}
}

void Yuv444ToRgb888(u8 y, u8 u, u8 v, u8 *r, u8 *g, u8 *b)
{
	int c = y - 16;
	int d = u - 128;
	int e = v - 128;

	*r = (u8)clamp((298 * c + 409 * e + 128) >> 8, 0, 255);
	*g = (u8)clamp((298 * c - 100 * d - 208 * e + 128) >> 8, 0, 255);
	*b = (u8)clamp((298 * c + 516 * d + 128) >> 8, 0, 255);
}

void Yuv422ToRgb888(u8 u, u8 y1, u8 v, u8 y2, u8 *r1, u8 *g1, u8 *b1, u8 *r2, u8 *g2, u8 *b2)
{
	Yuv444ToRgb888(y1, u, v, r1, g1, b1);
	Yuv444ToRgb888(y2, u, v, r2, g2, b2);
}

int main(int argc, char *argv[])
{
	u8 yuvBuffer[YUV422_SIZE];
	FILE *fp = NULL;
	FILE *fp2 = NULL;
	int i;
	u8 u, y1, v, y2;
	u8 rgb[6];
	u8 gray[2];

	if(argc != 3)
	{
		printf("Usage: <program> <input-yuv422-file> <output-grayscale-file>\n");
		return -1;
	}

	fp = fopen(argv[1], "r");
	if(!fp)
	{
		printf("Bad input file\n");
		return -2;
	}
	fp2 = fopen(argv[2], "w");
	if(!fp2)
	{
		printf("Bad output file\n");
		return -3;
	}

	fread(yuvBuffer, sizeof(u8), YUV422_SIZE, fp);
	for(i = 0; i < YUV422_SIZE; i += 4)
	{
		u = yuvBuffer[i];
		y1 = yuvBuffer[i + 1];
		v = yuvBuffer[i + 2];
		y2 = yuvBuffer[i + 3];
		Yuv422ToRgb888(u, y1, v, y2, &rgb[0], &rgb[1], &rgb[2], &rgb[3], &rgb[4], &rgb[5]);
		gray[0] = (rgb[0] + rgb[1] + rgb[2]) / 3;
		gray[1] = (rgb[3] + rgb[4] + rgb[5]) / 3;
		fwrite(gray, sizeof(u8), 2, fp2);
	}

	fclose(fp);
	fclose(fp2);

	printf("hello world!\n");
	return 0;
}
