#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scale.h"
#include "rotate.h"

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef __int64             int64_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned __int64    uint64_t;

int main()
{
	uint8_t* parse_yuv11;
	int bufsize11 = 640 * 480 * 2;
	parse_yuv11 = (uint8_t*)malloc(bufsize11);
	memset(parse_yuv11, 0, bufsize11);


	uint8_t* parse_yuv;
	int bufsize = 640 * 480 * 2;
	parse_yuv = (uint8_t*)malloc(bufsize);
	memset(parse_yuv, 0, bufsize);

	uint8_t* parse_yuv_small;
	parse_yuv_small = (uint8_t*)malloc(bufsize);
	memset(parse_yuv_small, 0, bufsize);

	FILE* fp_yuv;
	FILE* fp_outyuv;
	fp_yuv = fopen("parser_y.yuv", "rb");
	fp_outyuv = fopen("outyuv.yuv", "ab+");
	for (int i = 0; i < 200; i++){
		fread(parse_yuv, sizeof(uint8_t), 640 * 480 * 3 / 2, fp_yuv);

		//memcpy(parse_yuv_small, parse_yuv, 640 * 480 * 3 / 2);

		//scale
		int big_width = 640;
		int big_height = 480;
		int small_width = 160;
		int small_height = 120;
		libyuv::I420Scale(parse_yuv, big_width,
			parse_yuv + big_width * big_height, big_width / 2,
			parse_yuv + (big_width * big_height * 5) / 4, big_width / 2,
			big_width, big_height,
			parse_yuv_small, small_width,
			parse_yuv_small + small_width * small_height, small_width / 2,
			parse_yuv_small + (small_width * small_height * 5) / 4, small_width / 2,
			small_width, small_height,
			libyuv::kFilterBox);

		//mix
		libyuv::RotatePlane(parse_yuv_small, small_width,
			parse_yuv + big_width *(big_height - small_height) + (big_width - small_width), big_width,
			small_width, small_height, libyuv::kRotate0);
		libyuv::RotatePlane(parse_yuv_small + small_width*small_height, small_width / 2,
			parse_yuv + big_width *big_height + big_width / 2 * (big_height - small_height) / 2 + (big_width - small_width) / 2, big_width / 2,
			small_width / 2, small_height / 2, libyuv::kRotate0);
		libyuv::RotatePlane(parse_yuv_small + small_width*small_height * 5 / 4, small_width / 2,
			parse_yuv + big_width *big_height * 5 / 4 + +big_width / 2 * (big_height - small_height) / 2 + (big_width - small_width) / 2, big_width / 2,
			small_width / 2, small_height / 2, libyuv::kRotate0);

		//memcpy(parse_yuv_small + 320*240*5, parse_yuv11, 320 * 240 / 4);
		//fwrite(parse_yuv_small, sizeof(uint8_t), (small_width * small_height * 3) / 2, fp_outyuv);
		fwrite(parse_yuv, sizeof(uint8_t), (big_width * big_height * 3) / 2, fp_outyuv);

	}
	fclose(fp_yuv);
	fclose(fp_outyuv);
	return 0;
}
