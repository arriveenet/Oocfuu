#include "PngImage.h"
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _DEBUG
#pragma comment(lib, "libpng16d.lib")
#else
#pragma comment(lib, "libpng16.lib")
#endif // _DEBUG

#define SIGNATURE_SIZE	8


PngImage::PngImage()
{
}

PngImage::~PngImage()
{
}

bool PngImage::load(const char* _fileName)
{
	bool result = false;
	png_structp png_ptr = nullptr;
	png_infop info_ptr = nullptr;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type, compression_method, filter_method;
	png_byte type;
	int channel = 0;
	FILE* pFile = nullptr;

	errno_t err = fopen_s(&pFile, _fileName, "rb");
	if (err != 0) {
		return false;
	}

	png_byte signature[SIGNATURE_SIZE];
	const int readSigSize = (int)fread(signature, 1, SIGNATURE_SIZE, pFile);
	if (png_sig_cmp(signature, 0, SIGNATURE_SIZE)) {
		fclose(pFile);
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(pFile);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(pFile);
	}

	// png構造体のファイルポインターを設定
	png_init_io(png_ptr, pFile);
	// すでに読み取ったバイト数を設定
	png_set_sig_bytes(png_ptr, readSigSize);

	// png情報を読み込む
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, sizeof(png_bytepp) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
	}

	png_set_rows(png_ptr, info_ptr, row_pointers);

	png_read_image(png_ptr, row_pointers);

	png_read_end(png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);


	fclose(pFile);
	return result;
}
