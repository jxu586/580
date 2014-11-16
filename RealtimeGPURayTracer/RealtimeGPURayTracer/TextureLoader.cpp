#include "TextureLoader.h"
#include "error.h"

#include <cstdlib>
#include <cstdio>

#include <png.h>

#pragma comment(lib, "libpng16.lib")

CTextureLoader::CTextureLoader(void)
{
	width = height = bytesPerPixel = 0;
}

CTextureLoader::~CTextureLoader(void)
{
}

int CTextureLoader::LoadPNG(const char * filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file)
	{
		printf("Error in loadPNG: Cannot open file %s.\n", filename);
		return GET_ERROR_RETURN(ET_TEXTURE_FILE_NO_FOUND);
	}

	// read the header of the image file
	const unsigned int pngHeaderBytes = 8;
	png_byte pngHeader[pngHeaderBytes];
	if (fread(pngHeader, 1, pngHeaderBytes, file) != pngHeaderBytes)
	{
		printf("Error in loadPNG: cannot read the header of the png file.\n");
		fclose(file);
		return GET_ERROR_RETURN(ET_TEXTURE_INVALID_FILE_FORMAT);
	}

	int pngFlag = !png_sig_cmp(pngHeader, 0, pngHeaderBytes);
	if (!pngFlag)
	{
		printf("Error in loadPNG: %s is not a png file.\n", filename);
		fclose(file);
		return GET_ERROR_RETURN(ET_TEXTURE_INVALID_FILE_FORMAT);
	}

	// initialize png_struct and png_info
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		printf("Error in loadPNG: Creating the internal structure failed.\n");
		fclose(file);
		return GET_ERROR_RETURN(ET_TEXTURE_LOADING_INTERNAL_ERROR);
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,
			(png_infopp)NULL, (png_infopp)NULL);
		printf("Error in loadPNG: Creating the information structure failed.\n");
		fclose(file);
		return GET_ERROR_RETURN(ET_TEXTURE_LOADING_INTERNAL_ERROR);
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(file);
		printf("Error in loadPNG: cannot setup the error handling.\n");
		return GET_ERROR_RETURN(ET_TEXTURE_LOADING_INTERNAL_ERROR);
	}

	png_init_io(png_ptr, file);
	png_set_sig_bytes(png_ptr, pngHeaderBytes);

	png_read_info(png_ptr, info_ptr); 

	int color_type;
	int bit_depth;

	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)(&width), (png_uint_32*)(&height), &bit_depth, &color_type, NULL, NULL, NULL);

	// !!ATTETNION!! The following transformations are designed in the order that they should occur.
	// DO NOT change the sequence!!

	bool setBackgroundNeeded = false;

	switch (color_type)
	{
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(png_ptr);  // change palette to RGB
		setBackgroundNeeded = true;
		break;

	case PNG_COLOR_TYPE_GRAY:
		if (bit_depth < 8)  // transforms grayscale images of less than 8 to 8 bits.
		{
			png_set_expand_gray_1_2_4_to_8(png_ptr);
			setBackgroundNeeded = true;
		}
		break; 
	}

	// adds a full alpha channel if there is transparency information in a tRNS chunk
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// The image has 16 bits per channel for every pixel. Strip the pixels down to 8 bits.
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	// PNG files pack pixels of bit depths 1,2, and 4 into bytes.
	// The following code expands to 1 pixel per byte without changing the value of the pixels
	if (bit_depth < 8)
		png_set_packing(png_ptr);

	// convert a grayscale image to be represented as RGB
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
		setBackgroundNeeded = true;
	}

	// set background color if needed
	if (setBackgroundNeeded)
	{
		png_color_16 myBackground = {0, 0, 0, 0, 0};
		png_color_16 * imageBackground;

		if (png_get_bKGD(png_ptr, info_ptr, &imageBackground))
			png_set_background(png_ptr, imageBackground, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
		else
			png_set_background(png_ptr, &myBackground, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
	}

	//  PNG files store 16 bit pixels in net work byte order (big-endian)
	//  The following code may be needed on PC as to change the storage to the little-endian.

	//	if (bit_depth == 16)
	//		png_set_swap(png_ptr);

	// After setting the transformations, update the png_info structure to reflect any transformations you have requested
	png_read_update_info(png_ptr, info_ptr);

	// get color type again. It may be changed if any transformation is applied ?? 
	color_type = png_get_color_type(png_ptr, info_ptr);
	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		bytesPerPixel = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		bytesPerPixel = 4;
		break;
	default:
		printf("Error in loadPNG: image transformation failed.\n");
		fclose(file);
		exit(0);  // comment this out after debugging, use return IO_ERROR
		//return (IO_ERROR);
		break;
	}

	// sanity check here
	if (png_get_rowbytes(png_ptr, info_ptr) != bytesPerPixel * width)
	{
		printf("Error in loadPNG: the number of bytes per row, which is %lu, does not match bytesPerPixel * width, which is %d.\n", png_get_rowbytes(png_ptr, info_ptr), bytesPerPixel * width);  
		fclose(file);
		exit(0); // comment this out after debugging, use return IO_ERROR
		return GET_ERROR_RETURN(ET_TEXTURE_LOADING_INTERNAL_ERROR);
	}

	unsigned int bytesPerRow = bytesPerPixel * width;
	pixels = (unsigned char *) malloc (sizeof(unsigned char) * bytesPerRow * height);

	png_bytep * row_pointers = (png_bytep*) malloc (sizeof(png_bytep) * height);
	for(int row = 0; row < height; row++)
		row_pointers[row] = (png_byte*)(&pixels[(height - row - 1) * bytesPerRow]);

	png_read_image(png_ptr, row_pointers);

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	free(row_pointers);

	fclose(file);

	return 0;
}
