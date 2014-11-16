#ifndef _RENDER_TARGET_MANAGER_H_
#define _RENDER_TARGET_MANAGER_H_

#include "OpenGLHeader.h"

class CRenderTargetManager
{
protected:
	struct BufferInfo
	{
		GLint internalFormat;
		GLint format;
		GLint channelType;
		bool needMipMap;
		GLenum fbAttachment;
	};
protected:
	GLuint *pFrameBuffers;
	GLuint *pFBTextures;
	int w, h;
	int numBuffers;
	int numMultiSampling;
	bool bZBuffer;

	GLenum drawBufferFlags[10];
	BufferInfo buffersInfo[10];
protected:
	void GetTexFormat(GLint target, GLint internalFormat, GLint &format, GLint &type);
public:
	CRenderTargetManager(int numBuffers, bool hasZBuffer, int numMultiSampling);
	void AddBuffer(GLint internalFormat, GLint format, GLint type, GLenum fbAttachment, bool needMipMap, int id);
	int GenFrameBuffers(int width, int height);
	int Reshape(int width, int height);

	void BindForRead();
	void BindForWrite();
	void Unbind();

	void FlushBuffer();
	GLuint GetFrameBufferTex(int id) { return pFBTextures[id]; }

	~CRenderTargetManager(void);
};

#endif

