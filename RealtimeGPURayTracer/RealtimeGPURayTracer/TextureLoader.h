#ifndef _TEXTURE_LOADER_H_
#define _TEXTURE_LOADER_H_

class CTextureLoader
{
	int width, height;
	int bytesPerPixel;
	unsigned char *pixels;
public:
	CTextureLoader(void);
	~CTextureLoader(void);

	int LoadPNG(const char * filename);
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	int GetChannels() { return bytesPerPixel; }
	unsigned char *GetImage() { return pixels; }
};


#endif

